/*
 * A simple TGA loader
 *
 */

#include <config.h>
#ifdef SIMAGE_TGA_SUPPORT

#include <simage_tga.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* */
/* Supported types: */
/*   */
/*  1 (Uncompressed, color-mapped images) */
/*  2 (RGB uncompressed) */
/*  9 RLE color-mapped */
/* 10 RLE RGB */
/* */


#define ERR_NO_ERROR     0
#define ERR_OPEN         1
#define ERR_READ         2
#define ERR_MEM          3
#define ERR_UNSUPPORTED  4

static int tgaerror = ERR_NO_ERROR;
int
simage_tga_error(char * buffer, int buflen)
{
  switch (tgaerror) {
  case ERR_OPEN:
    strncpy(buffer, "TGA loader: Error opening file", buflen);
    break;
  case ERR_READ:
    strncpy(buffer, "TGA loader: Error reading file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "TGA loader: Out of memory error", buflen);
    break;
  }
  return tgaerror;
}

/* TODO: */
/* - bottom-up images */
/* - huffman, delta encoding */
static void 
convert_16_to_24(const unsigned char * const src, unsigned char * const dest)
{
  /* RGB for opengl, lo-hi 16 bit for TGA */
  unsigned int t0 = src[0];
  unsigned int t1 = src[1];
  dest[0] = (unsigned char) (t0 & 0x1f) << 2; /* r */
  dest[1] = (unsigned char) (t1 & 0x7c) >> 2; /* g */
  dest[2] = (unsigned char) ((t1 & 0x3)<<3) | ((t0&0xe)>>5); /*b */
}

static void 
convert_16_to_32(const unsigned char * const src, unsigned char * const dest)
{
  /* RGBA for opengl, lo-hi 16 bit for TGA */
  unsigned int t0 = src[0];
  unsigned int t1 = src[1];
  dest[0] = (unsigned char) (t0 & 0x1f) << 2; /* r */
  dest[1] = (unsigned char) (t1 & 0x7c) >> 2; /* g */
  dest[2] = (unsigned char) ((t1 & 0x3)<<3) | ((t0&0xe)>>5); /*b */
  dest[3] = (t1&0x70)?255:0; /* a */
}

static void 
convert_24_to_24(const unsigned char * const src, unsigned char * const dest)
{
  /* RGB for opengl */
  /* BGR for TGA */
  dest[0] = src[2];
  dest[1] = src[1];
  dest[2] = src[0];
}

static void 
convert_32_to_32(const unsigned char * const src, unsigned char * const dest)
{
  /* opengl image format is RGBA, not ARGB */
  /* TGA image format is BGRA for 32 bit */
  dest[0] = src[2];
  dest[1] = src[1];
  dest[2] = src[0];
  dest[3] = src[3];
}

static void 
convert_data(const unsigned char * const src, unsigned char * const dest, 
	     const int x, const int srcformat, 
	     const int destformat)
{
  if (srcformat == 2) {
    if (destformat == 3) 
      convert_16_to_24(src+x*srcformat,
		       dest+x*destformat);
    else {
      assert(destformat == 4);
      convert_16_to_32(src+x*srcformat,
		       dest+x*destformat);
    }
  }
  else if (srcformat == 3) {
    assert(destformat == 3);
    convert_24_to_24(src+x*srcformat,
		     dest+x*destformat);
  }
  else {
    assert(srcformat == 4 && destformat == 4);
    convert_32_to_32(src+x*srcformat,
		     dest+x*destformat);
  }
}

/* Intel byte order workaround */
static int getInt16(unsigned char *ptr)
{
  int res = ptr[0];
  int tmp = ptr[1];
  return res | (tmp<<8);
}

/* */
/* decode a new rle packet */
/* */
static unsigned char * 
rle_new_packet(unsigned char * src,
	       int * rleRemaining, 
	       int * rleIsCompressed,
	       unsigned char *rleCurrent,
	       const int rleEntrySize)
{
  int i;
  unsigned char code;
  
  code = *src++;

  *rleRemaining = (code & 127) + 1; /* number of bytes left in this packet */
  if (code & 128) { /* rle */
    *rleIsCompressed = 1;
    for (i = 0; i < rleEntrySize; i++)
      rleCurrent[i] = *src++;
  }
  else { /* uncompressed */
    *rleIsCompressed = 0;
  }
  return src;
}

/* */
/* decode the # of specified bytes */
/* */
static unsigned char * 
rle_decode(unsigned char * src, 
	   unsigned char *dest, 
	   const int numbytes,
	   int * rleRemaining,
	   int * rleIsCompressed,
	   unsigned char *rleCurrent,
	   const int rleEntrySize)
{
  int i;
  int remain, compress, size;
  unsigned char *stop = dest + numbytes;
  
  size = rleEntrySize;
  remain = *rleRemaining;
  compress = *rleIsCompressed;
  
  while (dest < stop) {
    if (remain == 0) {/* start new packet */
      src = rle_new_packet(src, &remain, &compress,
                           rleCurrent, rleEntrySize);
    }
    if (compress) {
      for (i = 0; i < size; i++) {
	*dest++ = rleCurrent[i];
      }
    }
    else {
      for (i = 0; i < size; i++) {
	*dest++ = *src++;
      }
    }
    remain--;
  }
  *rleRemaining = remain;
  *rleIsCompressed = compress;
  return src;
}

unsigned char *
simage_tga_load(const char *filename,
		 int *width_ret,
		 int *height_ret,
		 int *numComponents_ret)
{
  FILE * fp;
  unsigned char header[18];
  int type;
  int width;
  int height;
  int depth;
  int flags;
  int format;
  unsigned char *colormap;
  int indexsize;
  int rleIsCompressed;
  int rleRemaining;
  int rleEntrySize;
  unsigned char rleCurrent[4];
  unsigned char *buffer;
  unsigned char *dest;
  int bpr;
  unsigned char *linebuf;

  tgaerror = ERR_NO_ERROR; /* clear error */

  fp = fopen(filename, "rb");
  if (!fp) {
    tgaerror = ERR_OPEN;
    return NULL;
  }
  
  if (fread(header, 1, 18, fp) != 18) {
    tgaerror = ERR_READ;
    fclose(fp);
    return NULL;
  }

  type = header[2];
  width = getInt16(&header[12]);
  height = getInt16(&header[14]);
  depth = header[16] >> 3;
  flags = header[17];

  /* check for reasonable values in case this is not a tga file */
  if ((type != 2 && type != 10) ||
      (width < 0 || width > 4096) ||
      (height < 0 || height > 4096) ||
      (depth < 2 || depth > 4)) {
    tgaerror = ERR_UNSUPPORTED;
    fclose(fp);
    return NULL;
  }

  if (header[0]) /* skip identification field */
    fseek(fp, header[0], SEEK_CUR);

  colormap = NULL;
  if (header[1] == 1) { /* there is a colormap */
    int len = getInt16(&header[5]);
    indexsize = header[7]>>3;
    colormap = (unsigned char *)malloc(len*indexsize);
    fread(colormap, 1, len*indexsize, fp);
  }

  if (depth == 2) { /* 16 bits */
    if (flags & 1) format = 4;
    else format = 3;
  }
  else format = depth;

  /*    SoDebugError::postInfo("simage_tga_load", "TARGA file: %d %d %d %d %d\n",  */
  /*  			 type, width, height, depth, format); */

  rleIsCompressed = 0;
  rleRemaining = 0;
  rleEntrySize = depth;
  buffer = (unsigned char*)malloc(width*height*format);
  dest = buffer;
  bpr = format * width;
  linebuf = (unsigned char *)malloc(width*depth);
  
  switch(type) {
  case 1: /* colormap, uncompressed */
    {
      /* FIXME: write code */
      /* should never get here because simage_tga_identify returns 0 */
      /* for this filetype */
      /* I need example files in this format to write the code... */
      tgaerror = ERR_UNSUPPORTED;
    }
    break;
  case 2: /* RGB, uncompressed */
    {
      int x, y;
      for (y = 0; y < height; y++) {
	if (fread(linebuf, 1, width*depth, fp) != (unsigned int)width*depth) {
	  tgaerror = ERR_READ;
	  break;
	}
	for (x = 0; x < width; x++) {
	  convert_data(linebuf, dest, x, depth, format); 
	}
	dest += bpr;
      }
    }
    break;
  case 9: /* colormap, compressed */
    {
      /* FIXME: write code */
      /* should never get here because simage_tga_identify returns 0 */
      /* for this filetype */
      /* I need example files in this format to write the code... */
      tgaerror = ERR_UNSUPPORTED;
    }
    break;
  case 10: /* RGB, compressed */
    {
      int size, x, y;
      unsigned char *buf;
      unsigned char *src;
      int pos = ftell(fp);
      fseek(fp, 0, SEEK_END);
      size = ftell(fp) - pos;
      fseek(fp, pos, SEEK_SET);
      buf = (unsigned char *)malloc(size);
      if (buf == NULL) {
	tgaerror = ERR_MEM;
	break;
      }
      src = buf;
      if (fread(buf, 1, size, fp) != (unsigned int) size) {
	tgaerror = ERR_READ;
	break;
      }
      for (y = 0; y < height; y++) {
	src = rle_decode(src, linebuf, width*depth, &rleRemaining,
                         &rleIsCompressed, rleCurrent, rleEntrySize);
	assert(src <= buf + size);
	for (x = 0; x < width; x++) {
	  convert_data(linebuf, dest, x, depth, format); 
	}
	dest += bpr;
      }
      if (buf) free(buf);
    }
    break;
  default:
    tgaerror = ERR_UNSUPPORTED;
  }
  
  if (linebuf) free(linebuf);
  fclose(fp);

  if (tgaerror) {
    if (buffer) free(buffer);
    return NULL;
  }

  *width_ret = width;
  *height_ret = height;
  *numComponents_ret = format;

  return buffer;
}


int 
simage_tga_identify(const char *filename,
		     const unsigned char *buf,
		     int headerlen)
{
  const char * ptr;
  if (headerlen < 18) return 0;
  ptr = strrchr(filename, '.');
  if (!ptr) return 0; /* TGA files must end with .tga|.TGA */
  
  if (strcmp(ptr, ".tga") && strcmp(ptr, ".TGA")) return 0;
  
  if (buf[1] == 1 && buf[2] == 1 && buf[17] < 64) {
    /*      SoDebugError::postInfo("simage_tga_identify", */
    /*  			   "TARGA colormap file: %s\n", filename); */
    return 0;
  }
  if ((buf[1] == 0 || buf[1] == 1) && buf[2] == 2 && buf[17] < 64) return 1;
  if (buf[1] == 1 && buf[2] == 9 && buf[17] < 64) {
    /*      SoDebugError::postInfo("simage_tga_identity", */
    /*  			   "TARGA RLE and colormap file: %s\n", filename);  */

    /* will soon be supported */
    return 0;
  }
  if ((buf[1] == 0 || buf[1] == 1) && buf[2] == 10 && buf[17] < 64) {
    /* RLE and RGB */
    return 1;
  }
  else { /* unsupported */
    /*      SoDebugError::postInfo("simage_tga_identify", */
    /*  			   "Unsupported TARGA type.\n"); */
  }
  /* not a TGA, or not supported type */
  return 0;
}

#endif /* SIMAGE_TGA_SUPPORT */
