/*
 * An SGI RGB loader. By pederb@sim.no.
 */

#include <config.h>
#ifdef SIMAGE_RGB_SUPPORT

#include <simage_rgb.h>
#include <simage_private.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ERR_NO_ERROR          0
#define ERR_OPEN              1
#define ERR_READ              2
#define ERR_MEM               3
#define ERR_SIZEZ             4
#define ERR_OPEN_WRITE        5

static int rgberror = ERR_NO_ERROR;

typedef struct {
  FILE * in;
  int w;
  int h;
  int nc;
  int compressed;
  unsigned int * rowseek;
  int * rowlen;
  unsigned char * rlebuf;
  int rlebuflen;
  unsigned char * tmpbuf[4];
} simage_rgb_opendata;

unsigned char * 
simage_rgb_load(const char * filename,
                int * width,
                int * height,
                int * numcomponents)
{
  simage_rgb_opendata * od = (simage_rgb_opendata*) 
    simage_rgb_open(filename, width, height, numcomponents);
  
  if (od) {
    int i;
    int bpr = *width * *numcomponents;
    unsigned char * buf = (unsigned char *) malloc(bpr * *height);

    for (i = 0; i < *height; i++) {
      if (simage_rgb_read_line(od, i, buf+bpr*i) == 0) {
        /* rgberror will be set by simage_rgb_read_line() */
        free(buf);
        simage_rgb_close(od);
        return NULL;
      }
    }
    simage_rgb_close(od);
    return buf;
  }
  return NULL;
}

static int
write_short(FILE * fp, unsigned short val)
{
  unsigned char tmp[2];
  tmp[0] = (unsigned char)(val >> 8);
  tmp[1] = (unsigned char)(val & 0xff);
  return fwrite(&tmp, 2, 1, fp);
}

int 
simage_rgb_save(const char * filename,
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

int 
simage_rgb_identify(const char * filename,
                    const unsigned char * header,
                    int headerlen)
{
  static unsigned char rgbcmp[] = {0x01, 0xda};
  if (headerlen < 2) return 0;
  if (memcmp((const void*)header, 
	     (const void*)rgbcmp, 2) == 0) return 1;
  return 0;
}

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
  case ERR_OPEN_WRITE:
    strncpy(buffer, "RGB loader: Error opening file for writing", buflen);
  }
  return rgberror;
}

static int 
read_short(FILE * in, short * dst, int n, int swap)
{
  int i;
  unsigned char * ptr;
  unsigned char tmp;
  int num = fread(dst, sizeof(short), n, in);
  if (num == n && swap) {
    ptr = (unsigned char *) dst;
    for (i = 0; i < n; i++) {
      tmp = ptr[0];
      ptr[0] = ptr[1];
      ptr[1] = tmp;
      ptr += 2;
    }
  }
  return num == n;
}

static int 
read_ushort(FILE * in, unsigned short * dst, int n, int swap)
{
  return read_short(in, (short*) dst, n, swap);
}

static int 
read_int(FILE * in, int * dst, int n, int swap)
{
  int i;
  unsigned char tmp;
  unsigned char * ptr;
  int num = fread(dst, sizeof(int), n, in);
  if (num == n && swap) {
    ptr = (unsigned char *) dst;
    for (i = 0; i < n; i++) {
      tmp = ptr[0];
      ptr[0] = ptr[3];
      ptr[3] = tmp;
      tmp = ptr[1];
      ptr[1] = ptr[2];
      ptr[2] = tmp;
      ptr += 4;
    }
  }
  return num == n;
}

static int 
read_uint(FILE * in, unsigned int * dst, int n, int swap)
{
  return read_int(in, (int*)dst, n, swap);
}

void * 
simage_rgb_open(const char * filename,
                int * width,
                int * height,
                int * numcomponents)
{
  int i;
  int swap;
  FILE * in;
  unsigned short type;
  unsigned short size[3];
  simage_rgb_opendata * od;

  union {
    int data;
    char bytedata[4];
  } endiantest;
  
  endiantest.data = 1;
  /* need to swap shorts and integers on little endian systems  */
  swap = endiantest.bytedata[0] == 1;
  
  in = fopen(filename, "rb");
  if (!in) {
    rgberror = ERR_OPEN;
    return NULL;
  }
  /* skip imagic */
  (void) fseek(in, 2, SEEK_SET);
  if (!read_ushort(in, &type, 1, swap)) {
    rgberror = ERR_READ;
    fclose(in);
    return NULL;
  }
  /* skip dim */
  (void) fseek(in, 6, SEEK_SET);
  if (!read_ushort(in, size, 3, swap)) {
    rgberror = ERR_READ;
    fclose(in);
    return NULL;
  }
  
  od = (simage_rgb_opendata*) malloc(sizeof(simage_rgb_opendata));
  memset(od, 0, sizeof(simage_rgb_opendata));
  od->in = in;
  od->w = (int) size[0];
  od->h = (int) size[1];
  od->nc = (int) size[2];
  od->compressed = (type & 0xFF00) == 0x0100;
  od->rlebuf = (unsigned char*) malloc(od->w * 2);
  od->rlebuflen = od->w * 2;

  for (i = 0; i < od->nc; i++) {
    od->tmpbuf[i] = (unsigned char *) malloc(od->w);
  }
  if (od->compressed) {
    /* create row lookup table */
    int numlookup = od->h * od->nc;
    od->rowseek = (unsigned int*) malloc(numlookup * 4);
    od->rowlen = (int*) malloc(numlookup * 4);

    (void) fseek(in, 512, SEEK_SET);
    (void) read_uint(in, od->rowseek, numlookup, swap);
    if (!read_int(in, od->rowlen, numlookup, swap)) {
      rgberror = ERR_READ;
      simage_rgb_close(od);
      return NULL;
    }
  }
  *width = od->w;
  *height = od->h;
  *numcomponents = od->nc;
  return (void*) od;
}

void 
simage_rgb_close(void * opendata)
{
  int i;
  simage_rgb_opendata * od = 
    (simage_rgb_opendata*) opendata;
  
  fclose(od->in);
  for (i = 0; i < od->nc; i++) {
    free(od->tmpbuf[i]);
  }
  if (od->rowseek) free(od->rowseek);
  if (od->rowlen) free(od->rowlen);
  if (od->rlebuf) free(od->rlebuf);
  free(od);
}

static int
read_rgb_row_component(simage_rgb_opendata * od, int y, int c)
{
  if (od->compressed) {
    unsigned char * src, * dst;
    unsigned char * srcstop, * dststop;
    unsigned char pixel;
    int count;
    int rowlen;

    if (fseek(od->in, od->rowseek[y+c*od->h], SEEK_SET) != 0) {
      rgberror = ERR_READ;
      return 0;
    }
    rowlen = od->rowlen[y+c*od->h];
    if (rowlen > od->rlebuflen) {
      free(od->rlebuf);
      od->rlebuflen = rowlen;
      od->rlebuf = (unsigned char*) malloc(od->rlebuflen);
    }
    if (fread(od->rlebuf, 1, rowlen, od->in) != rowlen) {
      rgberror = ERR_READ;
      return 0;
    }

    src = od->rlebuf;
    dst = od->tmpbuf[c];
    srcstop = src + rowlen;
    dststop = dst + od->w;

    pixel = *src++;
    count = (int)(pixel & 0x7F);
    
    while (count) {
      if (dst + count > dststop) { rgberror = ERR_READ; return 0; }
      if (pixel & 0x80) {
        if (src + count > srcstop) { rgberror = ERR_READ; return 0; }
	while (count--) {
	  *dst++ = *src++;
	}
      } 
      else {
        if (src >= srcstop) { rgberror = ERR_READ; return 0; }
	pixel = *src++;
	while (count--) {
	  *dst++ = pixel;
	}
      }
      pixel = *src++;
      count = (int)(pixel & 0x7F);
    }
  }
  else {
    if (fseek(od->in, 512+(y*od->w)+(c*od->w*od->h), SEEK_SET) != 0) {
      rgberror = ERR_READ;
      return 0;
    }
    if (fread(od->tmpbuf[c], 1, od->w, od->in) != od->w) {
      rgberror = ERR_READ;
      return 0;
    }
  }
  return 1;
}

int 
simage_rgb_read_line(void * opendata, int y, unsigned char * buf)
{
  int i, c;
  unsigned char * ptr;

  simage_rgb_opendata * od = 
    (simage_rgb_opendata*) opendata;

  /* read each component into tmpbufs */
  for (c = 0; c < od->nc; c++) {
    if (!read_rgb_row_component(od, y, c)) {
      rgberror = ERR_READ;
      return 0;
    }
  }
  
  ptr = buf;
  /* merge components into pixels */
  for (i = 0; i < od->w; i++) {
    for (c = 0; c < od->nc; c++) {
      *ptr++ = od->tmpbuf[c][i];
    }
  }
  return 1;
}

#endif /* SIMAGE_RGB_SUPPORT */

