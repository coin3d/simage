#include "simage_tga.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//
// Supported types:
//  
//  1 (Uncompressed, color-mapped images)
//  2 (RGB uncompressed)
//  9 RLE color-mapped
// 10 RLE RGB
//


// TODO:
// - bottom-up images
// - huffman, delta encoding
static void 
convert_16_to_24(const unsigned char * const src, unsigned char * const dest)
{
  // RGB for opengl, lo-hi 16 bit for TGA
  unsigned int t0 = src[0];
  unsigned int t1 = src[1];
  dest[0] = (unsigned char) (t0 & 0x1f) << 2; // r
  dest[1] = (unsigned char) (t1 & 0x7c) >> 2; // g
  dest[2] = (unsigned char) ((t1 & 0x3)<<3) | ((t0&0xe)>>5); //b
}

static void 
convert_16_to_32(const unsigned char * const src, unsigned char * const dest)
{
  // RGBA for opengl, lo-hi 16 bit for TGA
  unsigned int t0 = src[0];
  unsigned int t1 = src[1];
  dest[0] = (unsigned char) (t0 & 0x1f) << 2; // r
  dest[1] = (unsigned char) (t1 & 0x7c) >> 2; // g
  dest[2] = (unsigned char) ((t1 & 0x3)<<3) | ((t0&0xe)>>5); //b
  dest[3] = (t1&0x70)?255:0; // a
}

static void 
convert_24_to_24(const unsigned char * const src, unsigned char * const dest)
{
  // RGB for opengl
  // BGR for TGA
  dest[0] = src[2];
  dest[1] = src[1];
  dest[2] = src[0];
}

static void 
convert_32_to_32(const unsigned char * const src, unsigned char * const dest)
{
  // opengl image format is RGBA, not ARGB
  // TGA image format is BGRA for 32 bit
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

// Intel byte order workaround
static int getInt16(unsigned char *ptr)
{
  int res = ptr[0];
  int tmp = ptr[1];
  return res | (tmp<<8);
}

//
// decode a new rle packet
//
static void 
rle_new_packet(unsigned char *&src, 
	       int &rleRemaining, 
	       int &rleIsCompressed,
	       unsigned char *rleCurrent,
	       const int rleEntrySize)
{
  unsigned char code = *src++;
  rleRemaining = (code & 127) + 1; // number of bytes left in this packet
  if (code & 128) { // rle
    rleIsCompressed = 1;
    for (int i = 0; i < rleEntrySize; i++)
      rleCurrent[i] = *src++;
  }
  else { // uncompressed
    rleIsCompressed = 0;
  }
}

//
// decode the # of specified bytes
//
static void 
rle_decode(unsigned char *&src, 
	   unsigned char *dest, 
	   const int numbytes,
	   int &rleRemaining,
	   int &rleIsCompressed,
	   unsigned char *rleCurrent,
	   const int rleEntrySize)
{
  int size = rleEntrySize;
  unsigned char *stop = dest + numbytes;
  while (dest < stop) {
    if (rleRemaining == 0) // start new packet
      rle_new_packet(src, rleRemaining, rleIsCompressed, 
		     rleCurrent, rleEntrySize);
    
    if (rleIsCompressed) {
      for (int i = 0; i < size; i++) {
	*dest++ = rleCurrent[i];
      }
    }
    else {
      for (int i = 0; i < size; i++) {
	*dest++ = *src++;
      }
    }
    rleRemaining--;
  }
}

int
simage_tga_error(char * /*buffer*/, int /*buflen*/)
{
  assert(0 && "FIXME: Not implemented");
  return 0;
}

unsigned char *
simage_tga_load(const char *filename,
		 int *width_ret,
		 int *height_ret,
		 int *numComponents_ret)
{
  FILE *fp = fopen(filename, "rb");
  if (!fp) return NULL;

  unsigned char header[18];
  
  if (fread(header, 1, 18, fp) != 18) {
    fclose(fp);
    return NULL;
  }

  int type = header[2];
  int width = getInt16(&header[12]);
  int height = getInt16(&header[14]);
  int depth = header[16] >> 3;
  int flags = header[17];
  int format;
  
  assert(width > 0 && height > 0);
  assert(width < 4096 && height < 4096);
  assert(depth >= 2 && depth <= 4);

  if (header[0]) // skip identification field
    fseek(fp, header[0], SEEK_CUR);

  unsigned char *colormap = NULL;
  int firstindex = getInt16(&header[3]);
  int indexsize;
  if (header[1] == 1) { // there is a colormap
    int len = getInt16(&header[5]);
    indexsize = header[7]>>3;
    colormap = new unsigned char[len*indexsize];
    fread(colormap, 1, len*indexsize, fp);
  }

  if (depth == 2) { // 16 bits
    if (flags & 1) format = 4;
    else format = 3;
  }
  else format = depth;

//    SoDebugError::postInfo("simage_tga_load", "TARGA file: %d %d %d %d %d\n", 
//  			 type, width, height, depth, format);


  int rleIsCompressed = 0;
  int rleRemaining = 0;
  int rleEntrySize = depth;
  unsigned char rleCurrent[4];
  unsigned char *buffer = (unsigned char*)
    malloc(width*height*format);
  unsigned char *dest = buffer;
  int bpr = format * width;
  unsigned char *linebuf = new unsigned char[width*depth];
  
  switch(type) {
  case 1: // colormap, uncompressed
    {
      // FIXME: write code
      // will never get here because simage_tga_identify returns 0
      // for this filetype
      assert(0); 
    }
    break;
  case 2: // RGB, uncompressed
    {
      for (int y = 0; y < height; y++) {
	fread(linebuf, 1, width*depth, fp);
	for (int x = 0; x < width; x++) {
	  convert_data(linebuf, dest, x, depth, format); 
	}
	dest += bpr;
      }
    }
    break;
  case 9: // colormap, compressed
    {
      // FIXME: write code
      // will never get here because simage_tga_identify returns 0
      // for this filetype
      assert(0);
    }
    break;
  case 10: // RGB, compressed
    {
      int pos = ftell(fp);
      fseek(fp, 0, SEEK_END);
      int size = ftell(fp) - pos;
      fseek(fp, pos, SEEK_SET);
      unsigned char *buf = new unsigned char[size];
      unsigned char *src = buf;
      fread(buf, 1, size, fp);
      for (int y = 0; y < height; y++) {
	rle_decode(src, linebuf, width*depth, rleRemaining,
		   rleIsCompressed, rleCurrent, rleEntrySize);
	assert(src <= buf + size);
	for (int x = 0; x < width; x++) {
	  convert_data(linebuf, dest, x, depth, format); 
	}
	dest += bpr;
      }
      delete [] buf;
//        SoDebugError::postInfo("simage_tga_load",
//  			     "post RLE: %p (%p)\n", src, buf+size);
    }
    break;
  default:
    assert(0);
    return NULL;
  }
  
  delete [] linebuf;

  fclose(fp);
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
  if (headerlen < 18) return 0;
  char *ptr = strrchr(filename, '.');
  if (!ptr) return 0; // TGA files must end with .tga|.TGA
  
  if (strcmp(ptr, ".tga") && strcmp(ptr, ".TGA")) return 0;
  
  if (buf[1] == 1 && buf[2] == 1 && buf[17] < 64) {
//      SoDebugError::postInfo("simage_tga_identify",
//  			   "TARGA colormap file: %s\n", filename);
    return 0;
  }
  if ((buf[1] == 0 || buf[1] == 1) && buf[2] == 2 && buf[17] < 64) return 1;
  if (buf[1] == 1 && buf[2] == 9 && buf[17] < 64) {
//      SoDebugError::postInfo("simage_tga_identity",
//  			   "TARGA RLE and colormap file: %s\n", filename); 

    // will soon be supported
    return 0;
  }
  if ((buf[1] == 0 || buf[1] == 1) && buf[2] == 10 && buf[17] < 64) {
    // RLE and RGB
    return 1;
  }
  else { // unsupported
//      SoDebugError::postInfo("simage_tga_identify",
//  			   "Unsupported TARGA type.\n");
  }
  // not a TGA, or not supported type
  return 0;
}







/* END OF FILE ********************************************************/
