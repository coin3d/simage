#include <config.h>
#ifdef SIMAGE_PIC_SUPPORT

#include <simage_pic.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR_NO_ERROR         0
#define ERROR_READING_HEADER   1
#define ERROR_READING_PALETTE  2
#define ERROR_MEMORY           3
#define ERROR_READ_ERROR       4

static int picerror = ERROR_NO_ERROR;


int
simage_pic_error(char *buffer, int bufferlen)
{
  switch (picerror) {
  case ERROR_READING_HEADER:
    strncpy(buffer, "PIC loader: Error reading header", bufferlen); 
    break;
  case ERROR_READING_PALETTE:
    strncpy(buffer, "PIC loader: Error reading palette", bufferlen); 
    break;
  case ERROR_MEMORY:
    strncpy(buffer, "PIC loader: Out of memory error", bufferlen); 
    break;
  case ERROR_READ_ERROR:
    strncpy(buffer, "PIC loader: Read error", bufferlen); 
    break;
  }
  return picerror;
}

/* byte order workaround *sigh* */

static int 
readint16(FILE *fp, int * res)
{
  unsigned char tmp = 0;
  unsigned int tmp2;
  if (fread(&tmp, 1, 1, fp) != 1) return 0;
  *res = tmp;
  if (fread(&tmp, 1, 1, fp) != 1) return 0;
  tmp2 = tmp;
  tmp2 <<= 8;
  *res |= tmp2;
  return 1;
}


int 
simage_pic_identify(const char * ptr,
		    const unsigned char *header,
		    int headerlen)
{
  static unsigned char piccmp[] = {0x19, 0x91};
  if (headerlen < 2) return 0;
  if (memcmp((const void*)header, 
	     (const void*)piccmp, 2) == 0) return 1;
  return 0;
}

unsigned char *
simage_pic_load(const char *filename,
		 int *width_ret,
		 int *height_ret,
		 int *numComponents_ret)
{
  int w, h, width, height, i, j, format;
  unsigned char palette[256][3];
  unsigned char * tmpbuf, * buffer, * ptr;

  FILE *fp = fopen(filename, "rb");
  if (!fp) return NULL;

  picerror = ERROR_NO_ERROR;
  
  fseek(fp, 2, SEEK_SET); 
  if (!readint16(fp, &w)) {
    picerror = ERROR_READING_HEADER;
    fclose(fp);
    return NULL;
  }

  fseek(fp, 4, SEEK_SET);
  if (!readint16(fp, &h)) {
    picerror = ERROR_READING_HEADER;
    fclose(fp);
    return NULL;
  }

  width = w;
  height = h;

  if (width <= 0 || height <= 0) {
    fclose(fp);
    return NULL;
  }
  fseek(fp, 32, SEEK_SET);

  if (fread(&palette, 3, 256, fp) != 256) {
    picerror = ERROR_READING_PALETTE;
  }

  tmpbuf = (unsigned char *)malloc(width);
  buffer = (unsigned char*) malloc(3*width*height);
  if (tmpbuf == NULL || buffer == NULL) {
    picerror = ERROR_MEMORY;
    if (tmpbuf) free(tmpbuf);
    if (buffer) free(buffer);
    fclose(fp);
    return NULL;
  }
  ptr = buffer;
  for (i = 0; i < height; i++) {
    if (fread(tmpbuf, 1, width, fp) != (size_t) width) {
      picerror = ERROR_READ_ERROR;
      fclose(fp);
      if (tmpbuf) free(tmpbuf);
      if (buffer) free(buffer);
      buffer = NULL;
      width = height = 0;
      return NULL;
    }
    for (j = 0; j < width; j++) {
      int idx = tmpbuf[j];
      *ptr++ = palette[idx][0];
      *ptr++ = palette[idx][1];
      *ptr++ = palette[idx][2];
    }
  }
  format = 3;
  fclose(fp);

  *width_ret = width;
  *height_ret = height;
  *numComponents_ret = format;
  return buffer;
}

#endif /* SIMAGE_PIC_SUPPORT */
