/*
 * based on example code found in libtiff
 * 
 */

#include "simage_tiff.h"
#include <stdio.h>

#include <tiffio.h>

#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>

int
simage_tiff_error(char * /*buffer*/, int /*buflen*/)
{
  assert(0 && "FIXME: Not implemented");
  return 0;
}


static void 
tiff_error(const char* /*module*/, const char* /*fmt*/, va_list)
{
  // FIXME: notify
}

static void 
tiff_warn(const char * /*module*/, const char * /*fmt*/, va_list)
{
  // FIXME: notify
}

static int
checkcmap(int n, uint16* r, uint16* g, uint16* b)
{
  while (n-- > 0)
    if (*r++ >= 256 || *g++ >= 256 || *b++ >= 256)
      return (16);
  // Assuming 8-bit colormap
  return (8);
}

static void 
convertrow(unsigned char *ptr,
	   unsigned char *data, int n, int invert)
{
  while (n--) {
    if (invert) *ptr++ = 255 - *data++;
    else *ptr++ = *data++;
  }
}


static void 
convertrow(unsigned char *ptr,
	   unsigned char *data, int n, unsigned short *rmap,
	   unsigned short *gmap, unsigned short *bmap)
{
  unsigned int ix;
  while (n--) {
    ix = *data++;
    *ptr++ = rmap[ix];
    *ptr++ = gmap[ix];
    *ptr++ = bmap[ix];
  }
}

static void 
convertrow(unsigned char *ptr, unsigned char *data, int n)
{
  while (n--) {
    *ptr++ = *data++;
    *ptr++ = *data++;
    *ptr++ = *data++;
  }
}

static void 
convertrow(unsigned char *ptr, 
	   unsigned char *red, unsigned char *blue, 
	   unsigned char *green, int n)
{
  while (n--) {
    *ptr++ = *red++;
    *ptr++ = *green++;
    *ptr++ = *blue++;
  }
}

int 
simage_tiff_identify(const char *,
		      const unsigned char *header,
		      int headerlen)
{
  if (headerlen < 6) return 0;
  static unsigned char tifcmp[] = {0x4d, 0x4d, 0x0, 0x2a, 0, 0};
  static unsigned char tifcmp2[] = {0x49, 0x49, 0x2a, 0}; 
  if (memcmp((const void*)header, (const void*)tifcmp, 6) == 0) return 1;
  if (memcmp((const void*)header, (const void*)tifcmp2, 4) == 0) return 1;
  return 0;
}

// useful defines (undef'ed below)
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

  TIFFSetErrorHandler(tiff_error);
  TIFFSetWarningHandler(tiff_warn);
  
  in = TIFFOpen(filename, "r");
  if (in == NULL) return NULL;
  
  if (TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &photometric) == 1) {
    if (photometric != PHOTOMETRIC_RGB && photometric != PHOTOMETRIC_PALETTE &&
	photometric != PHOTOMETRIC_MINISWHITE && 
	photometric != PHOTOMETRIC_MINISBLACK) {
      //Bad photometric; can only handle Grayscale, RGB and Palette images :-(
      TIFFClose(in);
      return NULL;
    }
  }
  else {
    TIFFClose(in);
    return NULL;
  }
  
  if (TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel) == 1) {
    if (samplesperpixel != 1 && samplesperpixel != 3) {
      // Bad samples/pixel
      TIFFClose(in);
      return NULL;
    }
  }
  else {
    TIFFClose(in);
    return NULL;
  }
	
  if (TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample) == 1) {
    if (bitspersample != 8) {
      // can only handle 8-bit samples.
      TIFFClose(in);
      return NULL;
    }
  }
  else {
    TIFFClose(in);
    return NULL;
  }

  uint32 w, h;
  uint16 config;
  uint16* red;
  uint16* green;
  uint16* blue;
  unsigned char *inbuf = NULL;
  tsize_t rowsize;
  uint32 row;
  int ok = 1;
  int format;

  if (TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &w) != 1 ||
      TIFFGetField(in, TIFFTAG_IMAGELENGTH, &h) != 1 ||
      TIFFGetField(in, TIFFTAG_PLANARCONFIG, &config) != 1) {
    TIFFClose(in);
    return NULL;
  }
  
  if (photometric == PHOTOMETRIC_MINISWHITE || 
      photometric == PHOTOMETRIC_MINISBLACK)
    format = 1;
  else
    format = 3;

  unsigned char *buffer = (unsigned char*)
    malloc(w*h*format);
  if (!buffer) {
    TIFFClose(in);
    return NULL;
  }
  int width = w;
  int height = h;
  
  unsigned char *currPtr = buffer + (h-1)*w*format;

  switch (pack(photometric, config)) {
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_MINISWHITE, PLANARCONFIG_SEPARATE):
  case pack(PHOTOMETRIC_MINISBLACK, PLANARCONFIG_SEPARATE):

    inbuf = new unsigned char[TIFFScanlineSize(in)];
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	ok = 0;
	break;
      }
      convertrow(currPtr, inbuf, w, photometric == PHOTOMETRIC_MINISWHITE);  
      currPtr -= format*w;
    }
    break;
    


  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_CONTIG):
  case pack(PHOTOMETRIC_PALETTE, PLANARCONFIG_SEPARATE):
    if (TIFFGetField(in, TIFFTAG_COLORMAP, &red, &green, &blue) != 1)
      ok = 0;
    //
    // Convert 16-bit colormap to 8-bit (unless it looks
    // like an old-style 8-bit colormap).
    //
    if (ok && checkcmap(1<<bitspersample, red, green, blue) == 16) {
      int i;
      for (i = (1<<bitspersample)-1; i >= 0; i--) {
	red[i] = CVT(red[i]);
	green[i] = CVT(green[i]);
	blue[i] = CVT(blue[i]);
      }
    }

    inbuf = new unsigned char[TIFFScanlineSize(in)];
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	ok = 0;
	break;
      }
      convertrow(currPtr, inbuf, w, red, green, blue);
      currPtr -= format*w;
    }
    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_CONTIG):
    inbuf = new unsigned char[TIFFScanlineSize(in)];
    for (row = 0; row < h; row++) {
      if (TIFFReadScanline(in, inbuf, row, 0) < 0) {
	ok = 0;
	break;
      }
      convertrow(currPtr, inbuf, w);  
      currPtr -= format*w;
    }
    break;

  case pack(PHOTOMETRIC_RGB, PLANARCONFIG_SEPARATE):
    rowsize = TIFFScanlineSize(in);
    inbuf = new unsigned char[3*rowsize];
    for (row = 0; ok && row < h; row++) {
      for (int s = 0; s < 3; s++) {
	if (TIFFReadScanline(in, inbuf+s*rowsize, row, s) < 0) {
	  ok = 0; break;
	}
      }
      if (ok) {
	convertrow(currPtr, inbuf, inbuf+rowsize, inbuf+2*rowsize, w);
	currPtr -= format*w;
      }
    }
    break;
  default:
    ok = 0;
    break;
  }
  
  if (inbuf) delete [] inbuf;
  TIFFClose(in);
  
  if (!ok) {
    delete [] buffer;
    return NULL;
  }
  *width_ret = width;
  *height_ret = height;
  *numComponents_ret = format;
  return buffer;
}

#undef CVT
#undef pack
