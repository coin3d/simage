/* *************************************************************************
 * simage_xwd.c -- xwd support without Xlib.
 * Written by Lars J. Aas <larsa@sim.no>
 * See include/X11/XWDFile.h for format information.
 * This implementation is far from complete and kind of "ad hoc".
 */

#include <config.h>
#ifdef SIMAGE_XWD_SUPPORT

#include <simage_xwd.h>

#include <sys/stat.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <errno.h> */

/* ********************************************************************** */

#define XWD_HEADER_SIZE              100
#define XWD_COLOR_SIZE                12

/* header offsets - all 32 bit */
#define XWD_HOFF_HEADER_SIZE           0
#define XWD_HOFF_FILE_VERSION          4 /* current was 7 */
#define XWD_HOFF_FORMAT                8
#define XWD_HOFF_DEPTH                12
#define XWD_HOFF_WIDTH                16
#define XWD_HOFF_HEIGHT               20
#define XWD_HOFF_XOFFSET              24
#define XWD_HOFF_BYTEORDER            28
#define XWD_HOFF_BITMAP_UNIT          32
#define XWD_HOFF_BITMAP_BITORDER      36
#define XWD_HOFF_BITMAP_PAD           40
#define XWD_HOFF_BITS_PER_PIXEL       44
#define XWD_HOFF_BYTES_PER_LINE       48
#define XWD_HOFF_VISUAL_CLASS         52
#define XWD_HOFF_RED_MASK             56
#define XWD_HOFF_GREEN_MASK           60
#define XWD_HOFF_BLUE_MASK            64
#define XWD_HOFF_BITS_PER_RGB         68
#define XWD_HOFF_COLORMAP_ENTRIES     72
#define XWD_HOFF_NUM_COLORS           76
#define XWD_HOFF_WINDOW_WIDTH         80
#define XWD_HOFF_WINDOW_HEIGHT        84
#define XWD_HOFF_WINDOW_X             88
#define XWD_HOFF_WINDOW_Y             92
#define XWD_HOFF_WINDOW_BORDERWIDTH   96

/* color offsets */
#define XWD_COFF_PIXEL                 0
#define XWD_COFF_RED                   4
#define XWD_COFF_GREEN                 6
#define XWD_COFF_BLUE                  8
#define XWD_COFF_FLAGS                10 /* 8 bit */

/* ********************************************************************** */

/* internal error codes */
#define XWD_NO_ERROR                   0
#define XWD_FILE_STAT_ERROR            1
#define XWD_FILE_OPEN_ERROR            2
#define XWD_FILE_READ_ERROR            3
#define XWD_MALLOC_ERROR               4
#define XWD_NO_SUPPORT_ERROR           5

static int xwderror = XWD_NO_ERROR;
/* static int xwderrno = 0; */

/* ********************************************************************** */

#define StaticGray                     0
#define GrayScale                      1
#define StaticColor                    2
#define PseudoColor                    3
#define TrueColor                      4
#define DirectColor                    5

#define LSBFirst                       0 /* little-endian */
#define MSBFirst                       1 /* big-endian */

#define XYBitmap                       0 /* depth 1, XYFormat */
#define XYPixmap                       1 /* depth == drawable depth */
#define ZPixmap                        2 /* depth == drawable depth */

/* ********************************************************************** */

#define INT32BE(ptr) ((ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3])
#define INT16BE(ptr) ((ptr[0] << 8) | ptr[1])

unsigned long getuint32be( const unsigned char * data ) { return INT32BE(data); }
unsigned long getuint16be( const unsigned char * data ) { return INT16BE(data); }

unsigned long swap32( unsigned long value ) {
  value = ((value & 0xff00ff00) >>  8) | ((value & 0x00ff00ff) <<  8);
  return  ((value & 0xffff0000) >> 16) | ((value & 0x0000ffff) << 16);
} /* swap32() */

/* ********************************************************************** */

int
simage_xwd_error(
  char * buffer,
  int bufferlen )
{
  switch ( xwderror ) {
  case XWD_FILE_STAT_ERROR:
    strncpy( buffer, "XWD loader: file stat error", bufferlen );
    break;
  case XWD_FILE_OPEN_ERROR:
    strncpy( buffer, "XWD loader: file open error", bufferlen );
    break;
  case XWD_FILE_READ_ERROR:
    strncpy( buffer, "XWD loader: file read error", bufferlen );
    break;
  case XWD_MALLOC_ERROR:
    strncpy( buffer, "XWD loader: malloc error", bufferlen );
    break;
  case XWD_NO_SUPPORT_ERROR:
    strncpy( buffer, "XWD loader: unsupported operation", bufferlen );
    break;
  }
  return xwderror;
} /* simage_xwd_error() */

/* ********************************************************************** */

int
simage_xwd_identify(
  const char * filename,
  const unsigned char * header,
  int headerlen )
{
  /* this test could certainly be improved */

  if ( headerlen < 12 ) return 0;
  if ( getuint32be( header + XWD_HOFF_HEADER_SIZE ) < XWD_HEADER_SIZE ) {
    /* FIXME: should store error message somewhere for the client app
       to read. Should anyway *not* print to stdout/stderr, as that
       can cause a crash when simage is built as a DLL under
       MSWin. 20020820 mortene. */
    /* fprintf( stdout, "headersize failure\n" ); */
    return 0;
  }
  if ( getuint32be( header + XWD_HOFF_FILE_VERSION ) != 7 ) {
    /* FIXME: should store error message somewhere for the client app
       to read. Should anyway *not* print to stdout/stderr, as that
       can cause a crash when simage is built as a DLL under
       MSWin. 20020820 mortene. */
    /* fprintf( stdout, "version failure\n" ); */
    return 0;
  }
  if ( getuint32be( header + XWD_HOFF_FORMAT ) != ZPixmap ) {
    /* FIXME: should store error message somewhere for the client app
       to read. Should anyway *not* print to stdout/stderr, as that
       can cause a crash when simage is built as a DLL under
       MSWin. 20020820 mortene. */
    /* fprintf( stdout, "format failure\n" ); */
    return 0;
  }
  return 1;
} /* simage_xwd_identify() */

/* ********************************************************************** */

unsigned char *
simage_xwd_load(
  const char * filename,
  int * width,
  int * height,
  int * components )
{
  unsigned char * buf, * image, * ptr, * line, * imageptr;
  unsigned long * palette;
  unsigned int w, h, c, /* i, */ x, y, num_colors; /* , colormap_entries; */
  unsigned int /* bits_per_rgb, */ bits_per_pixel, bytes_per_line;
  unsigned int pixel, /* red, green, blue, */ bits, got_bits, value;
  unsigned int swap;
  FILE * xwdfile;
  struct stat statdata;

  if ( stat( filename, &statdata ) == -1 ) {
    xwderror = XWD_FILE_STAT_ERROR;
    /* xwderrno = errno; */
    return NULL;
  }
  if ( (buf = (unsigned char *) malloc( statdata.st_size )) == NULL ) {
    xwderror = XWD_MALLOC_ERROR;
    /* xwderrno = errno; */
    return NULL;
  }
  xwdfile = fopen( filename, "rb" );
  if ( (xwdfile = fopen( filename, "rb" )) == NULL ) {
    free( buf );
    xwderror = XWD_FILE_OPEN_ERROR;
    /* xwderrno = errno; */
    return NULL;
  }
  if ( fread( buf, 1, statdata.st_size, xwdfile ) != statdata.st_size ) {
    free( buf );
    fclose( xwdfile );
    xwderror = XWD_FILE_READ_ERROR;
    /* xwderrno = errno; */
    return NULL;
  }
  fclose( xwdfile );

  w = getuint32be( buf + XWD_HOFF_WIDTH );
  h = getuint32be( buf + XWD_HOFF_HEIGHT );
  c = 3;
  if ( (image = (unsigned char *) malloc( w * h * c )) == NULL ) {
    free( buf );
    xwderror = XWD_MALLOC_ERROR;
    /* xwderrno = errno; */
    return NULL;
  }
  /* bits_per_rgb = getuint32be( buf + XWD_HOFF_BITS_PER_RGB ); */
  bytes_per_line = getuint32be( buf + XWD_HOFF_BYTES_PER_LINE );
  bits_per_pixel = getuint32be( buf + XWD_HOFF_BITS_PER_PIXEL );
  num_colors = getuint32be( buf + XWD_HOFF_NUM_COLORS );
  /* colormap_entries = getuint32be( buf + XWD_HOFF_COLORMAP_ENTRIES ); */
  swap = (getuint32be( buf + XWD_HOFF_BYTEORDER ) == LSBFirst ) ? 1 : 0;
  palette = NULL;

  /* check that format is supported and set
     xwderror = XWD_NO_SUPPORT_ERROR;
  if problem is detected */

  /*
  fprintf( stdout, "xwd image: %dx%d\n", w, h );
  fprintf( stdout, "bpp: %d  pbrgb: %d  bpl: %d\n", bits_per_pixel, bits_per_rgb, bytes_per_line );
  fprintf( stdout, "colors: %d  entries: %d\n", num_colors, colormap_entries );
  fprintf( stdout, "red: %08x  green: %08x  blue: %08x\n",
           getuint32be( buf + XWD_HOFF_RED_MASK ),
           getuint32be( buf + XWD_HOFF_GREEN_MASK ),
           getuint32be( buf + XWD_HOFF_BLUE_MASK ) );
  */

  /* check visual class */

  /* FIXME:
     if visual needs palette (I figure it's visual that decides this), read the palette data.
     leave it at NULL if else, since palette != NULL will lead to palette lookup
  if ( (palette = (unsigned long *) malloc( sizeof(long) * getuint32be( buf + XWD_HOFF_COLORMAP_ENTRIES ))) == NULL ) {
    free( buf );
    free( image );
    xwderror = XWD_MALLOC_ERROR;
    xwderrno = errno;
    return NULL;
  }
  ptr = buf + getuint32be( buf + XWD_HOFF_HEADER_SIZE );
  for ( i = 0; i < num_colors; i++ ) {
    pixel = getuint32be( ptr + XWD_COFF_PIXEL );
    red = (getuint16be( ptr + XWD_COFF_RED ) / 256) & 0xff;
    green = (getuint16be( ptr + XWD_COFF_GREEN ) / 256) & 0xff;
    blue = (getuint16be( ptr + XWD_COFF_BLUE ) / 256) & 0xff;
    palette[i] = ((red << 16) | (green << 8) | blue);
    fprintf( stdout, "color:  %08x (pixel)  %d (red)  %d (green)  %d (blue)  = %08x\n",
             pixel, red, green, blue, palette[i] );
    ptr += XWD_COLOR_SIZE;
  } */

  ptr = buf + getuint32be( buf + XWD_HOFF_HEADER_SIZE ) + (num_colors * XWD_COLOR_SIZE);
  imageptr = image;
  for ( y = 0; y < h; y++ ) {
    line = ptr + ((h-(y+1)) * bytes_per_line);
    got_bits = 0;
    bits = 0;
    for ( x = 0; x < w; x++ ) {
      while ( got_bits < bits_per_pixel ) {
        bits = (bits << 8) | line[0];
        line++;
        got_bits += 8;
      }
      if ( got_bits > bits_per_pixel ) {
        got_bits -= bits_per_pixel;
        value = bits >> got_bits;
        bits = bits & ((1 << got_bits) - 1);
      } else {
        value = bits;
        bits = 0;
        got_bits = 0;
      }
      if ( palette != NULL )
        pixel = palette[value];
      else if ( swap != 0 )
        pixel = swap32(value);
      else
        pixel = value;
      /* fprintf( stdout, "pixel (%3d, %3d) [%3d]:  %08x\n", x, y, value, pixel ); */
      /* FIXME - use red/green/blue masks and shift values if it's pixel data */
      *imageptr++ = (pixel >> 16) & 0xff;
      *imageptr++ = (pixel >> 8) & 0xff;
      *imageptr++ = pixel & 0xff;
    }
  }
  free( palette );
  free( buf );
  *width = w;
  *height = h;
  *components = c;
  return image;
} /* simage_xwd_load() */

/* ********************************************************************** */

int
simage_xwd_save(
  const char * filename,
  const unsigned char * bytes,
  int width,
  int height,
  int components )
{
  xwderror = XWD_NO_SUPPORT_ERROR;
  return 0;
} /* simage_xwd_save() */

#endif /* SIMAGE_XWD_SUPPORT */
/* EOF ****************************************************************** */
