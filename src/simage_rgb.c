#include <simage_rgb.h>

/*
 * This loader is based on code by Mark J. Kilgard. The original source
 * was not public domain, but was Copyright 1994, 1995, 1996 by 
 * Mark J. Kilgard. It was freely distributable without licensing fees, 
 * and was provided without guarantee or warrantee expressed or implied.
 * Some bugfixes and modifications by pederb
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_WINDOWS_H
/* FIXME: what the heck do we need this for? Code compiles just fine
   and dandy without this include under MSVC++ 6.0. 20001026
   mortene. */
#include <windows.h> /* sigh */
#endif /* HAVE_WINDOWS_H */

#define ERR_NO_ERROR          0
#define ERR_OPEN              1
#define ERR_READ              2
#define ERR_MEM               3
#define ERR_SIZEZ             4
#define ERR_OPEN_WRITE        5

static int rgberror = ERR_NO_ERROR;


int 
simage_rgb_error(char * buffer, int buflen)
{
  switch (rgberror) {
  case ERR_OPEN:
    strncpy(buffer, "RGB loader: Error opening file", buflen);
    break;
  case ERR_READ:
    strncpy(buffer, "RGB loader: Error reading file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "RGB loader: Out of memory error", buflen);
    break;
  case ERR_SIZEZ:
    strncpy(buffer, "RGB loader: Unsupported zsize", buflen);
    break;
  } 
  return rgberror;
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
  unsigned int tmpAllocSize;
} rawImageRec;

/************************************************************************/

static void 
ConvertShort(unsigned short *array, long length)
{
  unsigned long b1, b2;
  unsigned char *ptr;
  
  ptr = (unsigned char *)array;
  while (length--) {
    b1 = *ptr++;
    b2 = *ptr++;
    *array++ = (unsigned short) ((b1 << 8) | (b2));
  }
}

static void 
ConvertLong(unsigned int *array, long length)
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

static rawImageRec *
RawImageOpen(const char *fileName)
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
    rgberror = ERR_MEM;
    return NULL;
  }
  
  if ((raw->file = fopen(fileName, "rb")) == NULL) {
    rgberror = ERR_OPEN;
    free(raw);
    return NULL;
  }
  
  fread(raw, 1, 12, raw->file);
  
  if (swapFlag) {
    ConvertShort(&raw->imagic, 6);
  }

  /* only 1-4 bpp is supported */
  if (raw->sizeZ < 1 || raw->sizeZ > 4) {
    rgberror = ERR_SIZEZ;
    fclose(raw->file);
    free(raw);
    return NULL;
  }
  
  /* just in case a rle-row is bigger than an uncompressed row */
  raw->tmpAllocSize = raw->sizeX * 2; /* just mul by 2 to avoid reallocs*/
  raw->tmp =  (unsigned char *)malloc(raw->tmpAllocSize);
  
  raw->tmpR = (unsigned char *)malloc(raw->sizeX);
  raw->tmpG = (unsigned char *)malloc(raw->sizeX);
  raw->tmpB = (unsigned char *)malloc(raw->sizeX);
  raw->tmpA = (unsigned char *)malloc(raw->sizeX);
  if (raw->tmp == NULL || raw->tmpR == NULL || raw->tmpG == NULL ||
      raw->tmpB == NULL || raw->tmpA == NULL) {
    rgberror = ERR_MEM;
    fclose(raw->file);
    free(raw);
    return NULL;
  }
  raw->rowStart = NULL;
  raw->rowSize = NULL;
  
  if ((raw->type & 0xFF00) == 0x0100) {
    x = raw->sizeY * raw->sizeZ * sizeof(unsigned int);
    raw->rowStart = (unsigned int *)malloc(x);
    raw->rowSize = (int *)malloc(x);
    if (raw->rowStart == NULL || raw->rowSize == NULL) {
      rgberror = ERR_MEM;
      free(raw->tmp);
      free(raw->tmpR);
      free(raw->tmpG);
      free(raw->tmpB);
      free(raw->tmpA);
      fclose(raw->file);
      free(raw);
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

static void 
RawImageClose(rawImageRec *raw)
{
  fclose(raw->file);
  if (raw->tmp) free(raw->tmp);
  free(raw->tmpR);
  free(raw->tmpG);
  free(raw->tmpB);
  free(raw->tmpA);
  if (raw->rowStart) free(raw->rowStart);
  if (raw->rowSize) free(raw->rowSize);
  free(raw);
}

static int 
RawImageGetRow(rawImageRec *raw, unsigned char *buf, int y, int z)
{
  unsigned char *iPtr, *oPtr, pixel;
  int count;
  unsigned int rowsize;
  
  if ((raw->type & 0xFF00) == 0x0100) {
    if (fseek(raw->file, raw->rowStart[y+z*raw->sizeY], SEEK_SET) != 0) {
      rgberror = ERR_READ;
      return 0;
    }

    /* testing just in case, will probably never happen */
    rowsize = (unsigned int) raw->rowSize[y+z*raw->sizeY];
    if (rowsize > raw->tmpAllocSize) {
      free(raw->tmp);
      raw->tmpAllocSize = rowsize;
      raw->tmp = (unsigned char*) malloc(raw->tmpAllocSize);
      if (raw->tmp == NULL) {
	rgberror = ERR_MEM;
	return 0;
      }
    }
    
    if (fread(raw->tmp, 1, rowsize, raw->file) != rowsize) {
      rgberror = ERR_READ;
      return 0;
    }
    
    iPtr = raw->tmp;
    oPtr = buf;
    
    /* FIXME: check if oPtr > buf + raw->sizeX */
    while (1) {
      pixel = *iPtr++;
      count = (int)(pixel & 0x7F);
      if (!count) {
	return 1;
      }
      if (pixel & 0x80) {
	while (count--) {
	  *oPtr++ = *iPtr++;
	}
      } 
      else {
	pixel = *iPtr++;
	while (count--) {
	  *oPtr++ = pixel;
	}
      }
    }
  }
  else {
    if (fseek(raw->file, 512+(y*raw->sizeX)+(z*raw->sizeX*raw->sizeY),
	      SEEK_SET) != 0 ||
	fread(buf, 1, raw->sizeX, raw->file) != raw->sizeX) {
      rgberror = ERR_READ;
      return 0;
    }
  }
  return 1;
}

static int 
RawImageGetData(rawImageRec *raw, RGBImageRec *final)
{
  unsigned char *ptr;
  int i, j;

  final->data = (unsigned char*) 
    malloc(raw->sizeX*raw->sizeY*raw->sizeZ);
  if (final->data == NULL) {
    rgberror = ERR_MEM;
    return 0;
  }
  
  ptr = final->data;
  if (raw->sizeZ <= 2) {
    for (i = 0; i < raw->sizeY; i++) {
      if (!RawImageGetRow(raw, raw->tmpR, i ,0)) return 0;
      if (raw->sizeZ == 2) {
	if (!RawImageGetRow(raw, raw->tmpA, i, 1)) return 0;
      }
      for (j = 0; j < raw->sizeX; j++) {
	*ptr++ = *(raw->tmpR + j);
	if (raw->sizeZ==2) *ptr++ = *(raw->tmpA + j);
      }
    }
  }
  else {
    for (i = 0; i < raw->sizeY; i++) {
      if (!RawImageGetRow(raw, raw->tmpR, i, 0)) return 0;
      if (!RawImageGetRow(raw, raw->tmpG, i, 1)) return 0;
      if (!RawImageGetRow(raw, raw->tmpB, i, 2)) return 0;
      if (raw->sizeZ == 4) {
	if (!RawImageGetRow(raw, raw->tmpA, i, 3)) return 0;
      }
      for (j = 0; j < raw->sizeX; j++) {
	*ptr++ = *(raw->tmpR + j);
	*ptr++ = *(raw->tmpG + j);
	*ptr++ = *(raw->tmpB + j);
	if(raw->sizeZ==4) *ptr++ = *(raw->tmpA + j);
      }
    }
  }
  return 1;
}

static RGBImageRec *
rgbImageLoad(const char *fileName, RGBImageRec *final)
{
  rawImageRec *raw;
  int ret;

  raw = RawImageOpen(fileName);
  if (raw == NULL) {
    return NULL;
  }
  
  final->sizeX = raw->sizeX;
  final->sizeY = raw->sizeY;
  final->sizeZ = raw->sizeZ;

  ret = RawImageGetData(raw, final);

  RawImageClose(raw);

  if (final->data && ret)
    return final;
  
  /* loading failed for some reason */
  if (final->data) {
    free(final->data);
    final->data = NULL; 
  }
  return NULL;
}

int 
simage_rgb_identify(const char * ptr,
		     const unsigned char *header,
		     int headerlen)
{
  static unsigned char rgbcmp[] = {0x01, 0xda};
  if (headerlen < 2) return 0;
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

  rgberror = ERR_NO_ERROR; /* clear error flag */

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

/*
 * avoid endian problems (little endian sucks, right? :)
 */
static int
write_short(FILE * fp, unsigned short val)
{
  unsigned char tmp[2];
  tmp[0] = (unsigned char)(val >> 8);
  tmp[1] = (unsigned char)(val & 0xff);
  return fwrite(&tmp, 2, 1, fp);
}

int 
simage_rgb_save(const char *filename,
                const unsigned char * bytes,
                int width,
                int height,
                int comp)
{
  int x, y, c;
  unsigned char * tmpbuf;
  unsigned char buf[500];

  FILE * fp = fopen(filename, "wb");
  if (!fp) {
    rgberror = ERR_OPEN_WRITE;
    return 0;
  }

  write_short(fp, 0x01da); /* imagic */
  write_short(fp, 0x0001); /* raw (no rle yet) */

  if (comp == 1)
    write_short(fp, 0x0002); /* 2 dimensions (heightmap) */
  else
    write_short(fp, 0x0003); /* 3 dimensions */

  write_short(fp, (unsigned short) width);
  write_short(fp, (unsigned short) height);
  write_short(fp, (unsigned short) comp);

  memset(buf, 0, 500);
  buf[7] = 255; /* set maximum pixel value to 255 */
  strcpy((char *)buf+8, "http://www.coin3d.org");
  fwrite(buf, 1, 500, fp);

  tmpbuf = (unsigned char *) malloc(width);

  for (c = 0; c < comp; c++) {
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        tmpbuf[x] = bytes[x * comp + y * comp * width + c];
      }
      fwrite(tmpbuf, 1, width, fp);
    }
  }
  free(tmpbuf);
  fclose(fp);
  return 1;
}
