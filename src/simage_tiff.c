/*
 * based on example code found in libtiff
 * 
 */

#include <config.h>
#ifdef HAVE_TIFFLIB

#include <simage_tiff.h>
#include <stdio.h>

#include <tiffio.h>

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#define ERR_NO_ERROR    0
#define ERR_OPEN        1
#define ERR_READ        2
#define ERR_MEM         3
#define ERR_UNSUPPORTED 4
#define ERR_TIFFLIB     5
#define ERR_OPEN_WRITE  6
#define ERR_WRITE       7

static int tifferror = ERR_NO_ERROR;

int
simage_tiff_error(char * buffer, int buflen)
{
  switch (tifferror) {
  case ERR_OPEN:
    strncpy(buffer, "TIFF loader: Error opening file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "TIFF loader: Out of memory error", buflen);
    break;
  case ERR_UNSUPPORTED:
    strncpy(buffer, "TIFF loader: Unsupported image type", buflen);
    break;    
  case ERR_TIFFLIB:
    strncpy(buffer, "TIFF loader: Illegal tiff file", buflen);
    break;
  case ERR_OPEN_WRITE:
    strncpy(buffer, "TIFF saver: Error opening file", buflen);
    break;
  case ERR_WRITE:
    strncpy(buffer, "TIFF loader: Error writing file", buflen);
    break;
  }
  return tifferror;
}


static void 
tiff_error(const char* module, const char* fmt, va_list list)
{
  /* FIXME: store error message ? */
}

static void 
tiff_warn(const char * module, const char * fmt, va_list list)
{
  /* FIXME: notify? */
}

static int
checkcmap(int n, uint16* r, uint16* g, uint16* b)
{
  while (n-- > 0)
    if (*r++ >= 256 || *g++ >= 256 || *b++ >= 256)
      return (16);
  /* Assuming 8-bit colormap */
  return (8);
}

static void 
invert_row(unsigned char *ptr, unsigned char *data, int n, int invert)
{
  while (n--) {
    if (invert) *ptr++ = 255 - *data++;
    else *ptr++ = *data++;
  }
}


static void 
remap_row(unsigned char *ptr, unsigned char *data, int n,
	  unsigned short *rmap, unsigned short *gmap, unsigned short *bmap,unsigned char *amap)
{
  unsigned int ix;
  while (n--) {
    ix = *data++;
    *ptr++ = (unsigned char) rmap[ix];
    *ptr++ = (unsigned char) gmap[ix];
    *ptr++ = (unsigned char) bmap[ix];
    if (amap) *ptr++ = (unsigned char) amap[ix];
  }
}

static void 
copy_row(unsigned char *ptr, unsigned char *data, int n, int numcomponents)
{
  memcpy(ptr, data, n*numcomponents);
}

static void 
interleave_row(unsigned char *ptr,
	       unsigned char *red, 
               unsigned char *blue, 
               unsigned char *green,
               unsigned char *alpha,
	       int n)
{
  while (n--) {
    *ptr++ = *red++;
    *ptr++ = *green++;
    *ptr++ = *blue++;
    if (alpha) *ptr++ = *alpha++;
  }
}

static int
tiff_try_read_rgba(TIFF *in, int w, int h, int format, 
                      unsigned char * buffer)
{
  unsigned char * newbuffer = NULL;
  if (format != 4) {
    newbuffer = (unsigned char*) malloc(w*h*4);
  }
  else {
    newbuffer = buffer;
  }
  if (!TIFFReadRGBAImage(in, w, h, 
                         (unsigned int*) newbuffer, 1)) {
    free(newbuffer);
    return ERR_READ;
  }
  if (format != 4) {
    unsigned char * src = newbuffer;
    unsigned char * dst = buffer;
    int i, n = w*h;
    for (i = 0; i < n; i++) {
      switch (format) {
      case 1:
        *dst++ = src[0];
        break;
      case 2:
        *dst++ = src[0];
        *dst++ = src[3];
        break;
      case 3:
        *dst++ = src[0];
        *dst++ = src[1];
        *dst++ = src[2];
        break;
      default:
        break;

      }
      src += 4;
    }
    free(newbuffer);
  }

  return ERR_NO_ERROR;
}


int 
simage_tiff_identify(const char *ptr,
		     const unsigned char *header,
		     int headerlen)
{
  static unsigned char tifcmp[] = {0x4d, 0x4d, 0x0, 0x2a};
  static unsigned char tifcmp2[] = {0x49, 0x49, 0x2a, 0}; 
  
  if (headerlen < 4) return 0;
  if (memcmp((const void*)header, (const void*)tifcmp, 4) == 0) return 1;
  if (memcmp((const void*)header, (const void*)tifcmp2, 4) == 0) return 1;
  return 0;
}

/* useful defines (undef'ed below) */
#define	CVT(x)		(((x) * 255L) / ((1L<<16)-1))
#define	pack(a,b)	((a)<<8 | (b))

unsigned char *
simage_tiff_load(const char *filename,
		  int *width_ret,
		  int *height_ret,
		  int *numComponents_ret)
{
  TIFF *in;
  uint16 samplesperpixel;
  uint16 bitspersample;
  uint16 photometric;
  uint32 w, h;
  uint16 config;
  uint16* red;
  uint16* green;
  uint16* blue;
  unsigned char *inbuf = NULL;
  tsize_t rowsize;
  uint32 row;
  int format;
  unsigned char *buffer;
  int width;
  int height;
  unsigned char *currPtr;


  TIFFSetErrorHandler(tiff_error);
  TIFFSetWarningHandler(tiff_warn);
  
  in = TIFFOpen(filename, "r");
  if (in == NULL) {
    tifferror = ERR_OPEN;
    return NULL;
  }  
  if (TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &photometric) == 1) {
    if (photometric != PHOTOMETRIC_RGB && photometric != PHOTOMETRIC_PALETTE &&
	photometric != PHOTOMETRIC_MINISWHITE && 
	photometric != PHOTOMETRIC_MINISBLACK) {
      /*Bad photometric; can only handle Grayscale, RGB and Palette images :-( */
      TIFFClose(in);
      tifferror = ERR_UNSUPPORTED;
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    TIFFClose(in);
    return NULL;
  }
  
  if (TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel) == 1) {
    if (samplesperpixel < 1 || samplesperpixel > 4) {
      /* Bad samples/pixel */
      tifferror = ERR_UNSUPPORTED;
      TIFFClose(in);
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    TIFFClose(in);
    return NULL;
  }
	
  if (TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample) == 1) {
    if (bitspersample != 8) {
      /* can only handle 8-bit samples. */
      TIFFClose(in);
      tifferror = ERR_UNSUPPORTED;
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    TIFFClose(in);
    return NULL;
  }

  if (TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &w) != 1 ||
      TIFFGetField(in, TIFFTAG_IMAGELENGTH, &h) != 1 ||
      TIFFGetField(in, TIFFTAG_PLANARCONFIG, &config) != 1) {
    TIFFClose(in);
    tifferror = ERR_READ;
    return NULL;
  }
  
  if (photometric == PHOTOMETRIC_MINISWHITE || 
      photometric == PHOTOMETRIC_MINISBLACK)
    format = 1;
  else {
    if (photometric == PHOTOMETRIC_PALETTE) format = 3;
    else format = samplesperpixel;
  }
  buffer = (unsigned char*)malloc(w*h*format);
  
  if (!buffer) {
    tifferror = ERR_MEM;
    TIFFClose(in);
    return NULL;
  }

  width = w;
  height = h;
  
  currPtr = buffer + (h-1)*w*format;
  
  tifferror = ERR_NO_ERROR;

  switch (pack(photometric, config)) {
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_SEPARATE):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_SEPARATE):

    inbuf = (unsigned char *)malloc(TIFFScanlineSize(in));
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	tifferror = ERR_READ;
	break;
      }
      invert_row(currPtr, inbuf, w, photometric == PHOTOMETRIC_MINISWHITE);  
      currPtr -= format*w;
    }
    if (tifferror == ERR_READ) {
      tifferror = tiff_try_read_rgba(in, w, h, format, buffer);
    }

    break;
    
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_SEPARATE):
    if (TIFFGetField(in, TIFFTAG_COLORMAP, &red, &green, &blue) != 1)
      tifferror = ERR_READ;
    /* */
    /* Convert 16-bit colormap to 8-bit (unless it looks */
    /* like an old-style 8-bit colormap). */
    /* */
    if (!tifferror && checkcmap(1<<bitspersample, red, green, blue) == 16) {
      int i;
      for (i = (1<<bitspersample)-1; i >= 0; i--) {
	red[i] = CVT(red[i]);
	green[i] = CVT(green[i]);
	blue[i] = CVT(blue[i]);
      }
    }

    inbuf = (unsigned char *)malloc(TIFFScanlineSize(in));
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	tifferror = ERR_READ;
	break;
      }
      remap_row(currPtr, inbuf, w, red, green, blue, NULL);
      currPtr -= format*w;
    }
    if (tifferror == ERR_READ) {
      tifferror = tiff_try_read_rgba(in, w, h, format, buffer);
    }

    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_CONTIG):
    inbuf = (unsigned char *)malloc(TIFFScanlineSize(in));
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	tifferror = ERR_READ;
	break;
      }
      copy_row(currPtr, inbuf, w, format);  
      currPtr -= format*w;
    }
    if (tifferror == ERR_READ) {
      tifferror = tiff_try_read_rgba(in, w, h, format, buffer);
    }
    
    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_SEPARATE):
    rowsize = TIFFScanlineSize(in);
    inbuf = (unsigned char *)malloc(format*rowsize);
    for (row = 0; !tifferror && row < h; row++) {
      int s;
      for (s = 0; s < format; s++) {
	if (TIFFReadScanline(in, (tdata_t)(inbuf+s*rowsize), (uint32)row, (tsample_t)s) < 0) {
	  tifferror = ERR_READ; break;
	}
      }
      if (tifferror != ERR_READ) {
	interleave_row(currPtr, inbuf, inbuf+rowsize, inbuf+2*rowsize, 
                       format == 4 ? inbuf+3*rowsize : NULL, w);
	currPtr -= format*w;
      }
    }
    if (tifferror == ERR_READ) {
      tifferror = tiff_try_read_rgba(in, w, h, format, buffer);
    }

    break;
  default:
    tifferror = ERR_UNSUPPORTED;
    break;
  }
  
  if (inbuf) free(inbuf);
  TIFFClose(in);
  
  if (tifferror) {
    if (buffer) free(buffer);
    return NULL;
  }
  *width_ret = width;
  *height_ret = height;
  *numComponents_ret = format;
  return buffer;
}

int 
simage_tiff_save(const char *filename,
                 const unsigned char * bytes,
                 int width,
                 int height,
                 int numcomponents)
{
  uint16 photometric;
  TIFF * out;
  int y, bytesperrow;
  short config = PLANARCONFIG_CONTIG;
  int16 compression = COMPRESSION_PACKBITS; /* RLE */

  out = TIFFOpen(filename, "w");
  if (out == NULL) {
    tifferror = ERR_OPEN_WRITE;
    return 0;
  }

  TIFFSetField(out, TIFFTAG_IMAGEWIDTH, (uint32) width);
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, (uint32) height);
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
  if (numcomponents <= 2)
    photometric = PHOTOMETRIC_MINISBLACK;
  else
    photometric = PHOTOMETRIC_RGB;
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric);

  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, numcomponents);
  if (numcomponents == 2 || numcomponents == 4) {
    uint16 v[1];
    v[0] = EXTRASAMPLE_UNASSALPHA;
    TIFFSetField(out, TIFFTAG_EXTRASAMPLES, 1, v);
  }
  TIFFSetField(out, TIFFTAG_MINSAMPLEVALUE, (uint16) 0);
  TIFFSetField(out, TIFFTAG_MAXSAMPLEVALUE, (uint16) 255);
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, config);
  /* force 1 row/strip for library limitation */
  TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, 1L);
  TIFFSetField(out, TIFFTAG_IMAGEDESCRIPTION, filename);
  
  bytesperrow = width * numcomponents;

  for (y = 0; y < height; y++) {
    if (TIFFWriteScanline(out, (tdata_t) (bytes + bytesperrow * (height-y-1)), y, 0) < 0) {
      TIFFClose(out);
      tifferror = ERR_WRITE;
      return 0;
    }
  }

  TIFFClose(out);
  return 1;
}

typedef struct {
  TIFF * in;
  uint16 samplesperpixel;
  uint16 bitspersample;
  uint16 photometric;
  uint32 w, h;
  uint16 config;
  uint16 * red;
  uint16 * green;
  uint16 * blue;
  int format;
  int rowsize;
  unsigned char * inbuf;
} simage_tiff_opendata;

void * 
simage_tiff_open(const char * filename,
                 int * width,
                 int * height,
                 int * numcomponents)
{
  TIFF * in;
  simage_tiff_opendata * od;

  tifferror = ERR_NO_ERROR;

  TIFFSetErrorHandler(tiff_error);
  TIFFSetWarningHandler(tiff_warn);

  in = TIFFOpen(filename, "r");
  if (in == NULL) {
    tifferror = ERR_OPEN;
    return NULL;
  }
  od = (simage_tiff_opendata*) malloc(sizeof(simage_tiff_opendata));
  od->in = in;

  /* random access of lines is not be supported for palette images */
  if (TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &od->photometric) == 1) {
    if (od->photometric != PHOTOMETRIC_RGB && 
        /* od->photometric != PHOTOMETRIC_PALETTE && */
	od->photometric != PHOTOMETRIC_MINISWHITE && 
	od->photometric != PHOTOMETRIC_MINISBLACK) {
      /* Bad photometric; can only handle Grayscale and RGB images */
      TIFFClose(in);
      tifferror = ERR_UNSUPPORTED;
      free(od);
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    free(od);
    TIFFClose(in);
    return NULL;
  }
  
  if (TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &od->samplesperpixel) == 1) {
    if (od->samplesperpixel < 1 || od->samplesperpixel > 4) {
      /* Bad samples/pixel */
      tifferror = ERR_UNSUPPORTED;
      free(od);
      TIFFClose(in);
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    free(od);
    TIFFClose(in);
    return NULL;
  }
	
  if (TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &od->bitspersample) == 1) {
    if (od->bitspersample != 8) {
      /* can only handle 8-bit samples. */
      TIFFClose(in);
      tifferror = ERR_UNSUPPORTED;
      free(od);
      return NULL;
    }
  }
  else {
    tifferror = ERR_READ;
    TIFFClose(in);
    free(od);
    return NULL;
  }

  if (TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &od->w) != 1 ||
      TIFFGetField(in, TIFFTAG_IMAGELENGTH, &od->h) != 1 ||
      TIFFGetField(in, TIFFTAG_PLANARCONFIG, &od->config) != 1) {
    TIFFClose(in);
    tifferror = ERR_READ;
    free(od);
    return NULL;
  }
  
  if (od->photometric == PHOTOMETRIC_MINISWHITE || 
      od->photometric == PHOTOMETRIC_MINISBLACK)
    od->format = 1;
  else {
    if (od->photometric == PHOTOMETRIC_PALETTE) od->format = 3;
    else od->format = od->samplesperpixel;
  }
  switch (pack(od->photometric, od->config)) {
  default:
    break;
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_SEPARATE):
    if (TIFFGetField(in, TIFFTAG_COLORMAP, &od->red, &od->green, &od->blue) != 1)
      tifferror = ERR_READ;
    /* */
    /* Convert 16-bit colormap to 8-bit (unless it looks */
    /* like an old-style 8-bit colormap). */
    /* */
    if (!tifferror && checkcmap(1<<od->bitspersample, od->red, od->green, od->blue) == 16) {
      int i;
      for (i = (1<<od->bitspersample)-1; i >= 0; i--) {
	od->red[i] = CVT(od->red[i]);
	od->green[i] = CVT(od->green[i]);
	od->blue[i] = CVT(od->blue[i]);
      }
    }    
  }
  od->rowsize = TIFFScanlineSize(in);
  od->inbuf = (unsigned char *) malloc(od->rowsize * 4); /* *4 to support all formats */

  *width = od->w;
  *height = od->h;
  *numcomponents = od->format;

  return (void*) od;
}


void 
simage_tiff_close(void * opendata)
{
  simage_tiff_opendata * od = (simage_tiff_opendata*) opendata;
  TIFFClose(od->in);
  free(od->inbuf);
  free(od);
}

int 
simage_tiff_read_line(void * opendata, int y, unsigned char * buf)
{
  int s, row;
  simage_tiff_opendata * od;
  tifferror = ERR_NO_ERROR;
  
  od = (simage_tiff_opendata*) opendata;
  row = (od->h-1)-y;
  
  switch (pack(od->photometric, od->config)) {
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_SEPARATE):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_SEPARATE):
    if (TIFFReadScanline(od->in, od->inbuf, row, 0) < 0) {
      tifferror = ERR_READ;
      break;
    }
    invert_row(buf, od->inbuf, od->w, od->photometric == PHOTOMETRIC_MINISWHITE);  
    break;

  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_SEPARATE):
    if (TIFFReadScanline(od->in, od->inbuf, row, 0) < 0) {
      tifferror = ERR_READ;
      break;
    }
    remap_row(buf, od->inbuf, od->w, od->red, od->green, od->blue, NULL);
    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_CONTIG):
    if (TIFFReadScanline(od->in, od->inbuf, row, 0) < 0) {
      tifferror = ERR_READ;
      break;
    }
    copy_row(buf, od->inbuf, od->w, od->format);  
    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_SEPARATE):
    for (s = 0; s < od->format; s++) {
      if (TIFFReadScanline(od->in, (tdata_t)(od->inbuf+s*od->rowsize), 
                           (uint32)row, (tsample_t)s) < 0) {      
        tifferror = ERR_READ; break;
      }
    }
    if (!tifferror) {
      interleave_row(buf, od->inbuf, od->inbuf+od->rowsize, od->inbuf + 2*od->rowsize, 
                     od->format == 4 ? od->inbuf + 3*od->rowsize: NULL, od->w);
    }
    break;
  default:
    tifferror = ERR_UNSUPPORTED;
    break;
  }
  return tifferror == ERR_NO_ERROR;
}

#undef CVT
#undef pack

#endif /* HAVE_TIFFLIB */
