#include "simage_rgb.h"

/*
 * This code is by David Blythe, SGI <blythe@sgi.com>
 * some minor bugfixes only
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#ifdef _WIN32
#include <windows.h> /* (sigh) */
#endif

#include <assert.h>

int 
simage_rgb_error(char * /*buffer*/, int /*buflen*/)
{
  assert(0 && "FIXME: Not implemented");
  return 0;
}


typedef struct {
  int sizeX, sizeY, sizeZ;
  unsigned char *data;
} RGBImageRec;

typedef struct _rawImageRec {
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short sizeX, sizeY, sizeZ;
    unsigned long min, max;
    unsigned long wasteBytes;
    char name[80];
    unsigned long colorMap;
    FILE *file;
    unsigned char *tmp, *tmpR, *tmpG, *tmpB, *tmpA;
    unsigned long rleEnd;
    unsigned int *rowStart;
    int *rowSize;
} rawImageRec;

/************************************************************************/

static void ConvertShort(unsigned short *array, long length)
{
  unsigned long b1, b2;
  unsigned char *ptr;
  
  ptr = (unsigned char *)array;
  while (length--) {
    b1 = *ptr++;
    b2 = *ptr++;
    *array++ = (b1 << 8) | (b2);
  }
}

static void ConvertLong(unsigned int *array, long length)
{
  unsigned long b1, b2, b3, b4;
  unsigned char *ptr;

  ptr = (unsigned char *)array;
  while (length--) {
    b1 = *ptr++;
    b2 = *ptr++;
    b3 = *ptr++;
    b4 = *ptr++;
    *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
  }
}

static rawImageRec *RawImageOpen(const char *fileName)
{
  union {
    int testWord;
    char testByte[4];
  } endianTest;
  rawImageRec *raw;
  int swapFlag;
  int x;
  
  endianTest.testWord = 1;
  if (endianTest.testByte[0] == 1) {
    swapFlag = 1;
  } else {
    swapFlag = 0;
  }
  
  raw = (rawImageRec *) malloc(sizeof(rawImageRec));
  if (raw == NULL) {
    return NULL;
  }
  
  if ((raw->file = fopen(fileName, "rb")) == NULL) {
    free(raw);
    return NULL;
  }
  
  fread(raw, 1, 12, raw->file);
  
  if (swapFlag) {
    ConvertShort(&raw->imagic, 6);
  }
  
  raw->tmp =  (unsigned char *)malloc(raw->sizeX*256);
  raw->tmpR = (unsigned char *)malloc(raw->sizeX*256);
  raw->tmpG = (unsigned char *)malloc(raw->sizeX*256);
  raw->tmpB = (unsigned char *)malloc(raw->sizeX*256);
  raw->tmpA = (unsigned char *)malloc(raw->sizeX*256);
  if (raw->tmp == NULL || raw->tmpR == NULL || raw->tmpG == NULL ||
      raw->tmpB == NULL || raw->tmpA == NULL) {
    return NULL;
  }
  raw->rowStart = NULL;
    raw->rowSize = NULL;

    if ((raw->type & 0xFF00) == 0x0100) {
	x = raw->sizeY * raw->sizeZ * sizeof(unsigned int);
	raw->rowStart = (unsigned int *)malloc(x);
	raw->rowSize = (int *)malloc(x);
	if (raw->rowStart == NULL || raw->rowSize == NULL) {
	    return NULL;
	}
	raw->rleEnd = 512 + (2 * x);
	fseek(raw->file, 512, SEEK_SET);
	fread(raw->rowStart, 1, x, raw->file);
	fread(raw->rowSize, 1, x, raw->file);
	if (swapFlag) {
	    ConvertLong(raw->rowStart, x/sizeof(unsigned int));
	    ConvertLong((unsigned int *)raw->rowSize, x/sizeof(int));
	}
    }
    return raw;
}

static void RawImageClose(rawImageRec *raw)
{
  fclose(raw->file);
  free(raw->tmp);
  free(raw->tmpR);
  free(raw->tmpG);
  free(raw->tmpB);
  free(raw->tmpA);
  if (raw->rowStart) free(raw->rowStart);
  if (raw->rowSize) free(raw->rowSize);
  free(raw);
}

static void RawImageGetRow(rawImageRec *raw, unsigned char *buf, int y, int z)
{
  unsigned char *iPtr, *oPtr, pixel;
  int count;

  if ((raw->type & 0xFF00) == 0x0100) {
    fseek(raw->file, raw->rowStart[y+z*raw->sizeY], SEEK_SET);
    fread(raw->tmp, 1, (unsigned int)raw->rowSize[y+z*raw->sizeY],
	  raw->file);
    
    iPtr = raw->tmp;
    oPtr = buf;
    while (1) {
      pixel = *iPtr++;
      count = (int)(pixel & 0x7F);
      if (!count) {
	return;
      }
      if (pixel & 0x80) {
	while (count--) {
	  *oPtr++ = *iPtr++;
	}
      } else {
	pixel = *iPtr++;
	while (count--) {
	  *oPtr++ = pixel;
	}
      }
    }
  } else {
    fseek(raw->file, 512+(y*raw->sizeX)+(z*raw->sizeX*raw->sizeY),
	  SEEK_SET);
    fread(buf, 1, raw->sizeX, raw->file);
  }
}

static void 
RawImageGetData(rawImageRec *raw, RGBImageRec *final)
{
  unsigned char *ptr;
  int i, j;

  final->data = (unsigned char*) 
    malloc(raw->sizeX*raw->sizeY*raw->sizeZ);
  if (final->data == NULL) {
    return;
  }
  
  ptr = final->data;
  if (raw->sizeZ <= 2) {
    for (i = 0; i < raw->sizeY; i++) {
      RawImageGetRow(raw, raw->tmpR, i ,0);
      if (raw->sizeZ == 2) RawImageGetRow(raw, raw->tmpA, i, 1);
      for (j = 0; j < raw->sizeX; j++) {
	*ptr++ = *(raw->tmpR + j);
	if (raw->sizeZ==2) *ptr++ = *(raw->tmpA + j);
      }
    }
  }
  else {
    for (i = 0; i < raw->sizeY; i++) {
      RawImageGetRow(raw, raw->tmpR, i, 0);
      RawImageGetRow(raw, raw->tmpG, i, 1);
      RawImageGetRow(raw, raw->tmpB, i, 2);
      if (raw->sizeZ == 4) RawImageGetRow(raw, raw->tmpA, i, 3);
      for (j = 0; j < raw->sizeX; j++) {
	*ptr++ = *(raw->tmpR + j);
	*ptr++ = *(raw->tmpG + j);
	*ptr++ = *(raw->tmpB + j);
	if(raw->sizeZ==4) *ptr++ = *(raw->tmpA + j);
      }
    }
  }
}

static RGBImageRec *
rgbImageLoad(const char *fileName, RGBImageRec *final )
{
  rawImageRec *raw;
  
  raw = RawImageOpen(fileName);
  if (raw == NULL) {
    return NULL;
  }
  
  final->sizeX = raw->sizeX;
  final->sizeY = raw->sizeY;
  final->sizeZ = raw->sizeZ;

  RawImageGetData(raw, final);
  RawImageClose(raw);

  if (final->data)
    return final;
  return NULL;
}


int 
simage_rgb_identify(const char *,
		     const unsigned char *header,
		     int headerlen)
{
  if (headerlen < 2) return 0;
  static unsigned char rgbcmp[] = {0x01, 0xda};
  if (memcmp((const void*)header, 
	     (const void*)rgbcmp, 2) == 0) return 1;
  return 0;
}

unsigned char * 
simage_rgb_load(const char *filename,
		 int *width,
		 int *height,
		 int *numComponents)
{
  unsigned char *buffer = NULL;
  RGBImageRec image;
  if (filename) {
    if (rgbImageLoad(filename, &image)) {
      *width = image.sizeX;
      *height = image.sizeY;
      buffer = image.data;
      *numComponents = image.sizeZ;
    }
  }
  return buffer;
}
