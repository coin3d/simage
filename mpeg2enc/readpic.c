/* readpic.c, read source pictures                                          */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"

#ifndef USE_CLIPTABLE
#define USE_CLIPTABLE 0
#endif

/* private prototypes */
static void read_y_u_v(simpeg_encode_context * context, char *fname, unsigned char *frame[]);
static void read_yuv(simpeg_encode_context * context,char *fname, unsigned char *frame[]);
static void read_ppm(simpeg_encode_context * context,char *fname, unsigned char *frame[]);
static void border_extend(unsigned char *frame, int w1, int h1,
                          int w2, int h2);
static void conv444to422(simpeg_encode_context * context,unsigned char *src, unsigned char *dst);
static void conv422to420(simpeg_encode_context * context,unsigned char *src, unsigned char *dst);

static void SimpegWrite_read_buffer(simpeg_encode_context * context, unsigned char *buffer, unsigned char *frame[]);

/* pbm_getc() and pbm_getint() are essentially taken from
 * PBMPLUS (C) Jef Poskanzer
 * but without error/EOF checking
 */
static char pbm_getc(FILE *file)
{
  char ch;

  ch = getc(file);

  if (ch=='#')
  {
    do
    {
      ch = getc(file);
    }
    while (ch!='\n' && ch!='\r');
  }

  return ch;
}

static int pbm_getint(FILE *file)
{
  char ch;
  int i;

  do
  {
    ch = pbm_getc(file);
  }
  while (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r');

  i = 0;
  do
  {
    i = i*10 + ch-'0';
    ch = pbm_getc(file);
  }
  while (ch>='0' && ch<='9');

  return i;
}

void 
simpeg_encode_readframe(simpeg_encode_context * context, char *fname, unsigned char **frame)
{
  switch (context->inputtype)
  {
  case T_Y_U_V:
    read_y_u_v(context,fname,frame);
    break;
  case T_YUV:
    read_yuv(context,fname,frame);
    break;
  case T_PPM:
    read_ppm(context,fname,frame);
    break;
  case T_RGB_BUFFER:
    /* fname is bitmap buffer, not filename.
       See putseq.c SimpegWrite_putseq_encode_bitmap().
    */
    SimpegWrite_read_buffer(context,(unsigned char *)fname, frame);  
    break;
  default:
    break;
  }
}

static void 
read_y_u_v(simpeg_encode_context * context, char *fname, unsigned char **frame)
{
  int i;
  int chrom_hsize, chrom_vsize;
  char name[128];
  FILE *fd;
  int width, height, chroma_format, vertical_size, horizontal_size;

  width = context->width;
  height = context->height;
  chroma_format = context->chroma_format;
  vertical_size = context->vertical_size;
  horizontal_size = context->horizontal_size;
  
  chrom_hsize = (chroma_format==CHROMA444) ? horizontal_size : horizontal_size>>1;
  chrom_vsize = (chroma_format!=CHROMA420) ? vertical_size : vertical_size>>1;
  
  sprintf(name,"%s.Y",fname);
  if (!(fd = fopen(name,"rb")))
  {
    sprintf(context->errortext,"Couldn't open %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  for (i=0; i<vertical_size; i++)
    fread(frame[0]+i*width,1,horizontal_size,fd);
  fclose(fd);
  border_extend(frame[0],horizontal_size,vertical_size,width,height);
  
  sprintf(name,"%s.U",fname);
  if (!(fd = fopen(name,"rb")))
  {
    sprintf(context->errortext,"Couldn't open %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  for (i=0; i<chrom_vsize; i++)
    fread(frame[1]+i*context->chrom_width,1,chrom_hsize,fd);
  fclose(fd);
  border_extend(frame[1],chrom_hsize,chrom_vsize,context->chrom_width,context->chrom_height);

  sprintf(name,"%s.V",fname);
  if (!(fd = fopen(name,"rb")))
  {
    sprintf(context->errortext,"Couldn't open %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  for (i=0; i<chrom_vsize; i++)
    fread(frame[2]+i*context->chrom_width,1,chrom_hsize,fd);
  fclose(fd);
  border_extend(frame[2],chrom_hsize,chrom_vsize,context->chrom_width,context->chrom_height);
}

static void read_yuv(simpeg_encode_context * context,char *fname, unsigned char **frame)
{
  int i;
  int chrom_hsize, chrom_vsize;
  char name[128];
  FILE *fd;

  int width, height, chroma_format, vertical_size, horizontal_size;

  width = context->width;
  height = context->height;
  chroma_format = context->chroma_format;
  vertical_size = context->vertical_size;
  horizontal_size = context->horizontal_size;

  chrom_hsize = (chroma_format==CHROMA444) ? horizontal_size
                                           : horizontal_size>>1;
  chrom_vsize = (chroma_format!=CHROMA420) ? vertical_size
                                           : vertical_size>>1;

  sprintf(name,"%s.yuv",fname);
  if (!(fd = fopen(name,"rb")))
  {
    sprintf(context->errortext,"Couldn't open %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }

  /* Y */
  for (i=0; i<vertical_size; i++)
    fread(frame[0]+i*width,1,horizontal_size,fd);
  border_extend(frame[0],horizontal_size,vertical_size,width,height);

  /* Cb */
  for (i=0; i<chrom_vsize; i++)
    fread(frame[1]+i*context->chrom_width,1,chrom_hsize,fd);
  border_extend(frame[1],chrom_hsize,chrom_vsize,context->chrom_width,context->chrom_height);

  /* Cr */
  for (i=0; i<chrom_vsize; i++)
    fread(frame[2]+i*context->chrom_width,1,chrom_hsize,fd);
  border_extend(frame[2],chrom_hsize,chrom_vsize,context->chrom_width,context->chrom_height);

  fclose(fd);
}

static void read_ppm(simpeg_encode_context * context,char *fname, unsigned char **frame)
{
  int i, j;
  int r, g, b;
  double y, u, v;
  double cr, cg, cb, cu, cv;
  char name[128];
  FILE *fd;
  unsigned char *yp, *up, *vp;

  static double coef[7][3] = {
    {0.2125,0.7154,0.0721}, /* ITU-R Rec. 709 (1990) */
    {0.299, 0.587, 0.114},  /* unspecified */
    {0.299, 0.587, 0.114},  /* reserved */
    {0.30,  0.59,  0.11},   /* FCC */
    {0.299, 0.587, 0.114},  /* ITU-R Rec. 624-4 System B, G */
    {0.299, 0.587, 0.114},  /* SMPTE 170M */
    {0.212, 0.701, 0.087}}; /* SMPTE 240M (1987) */

  int width, height, chroma_format, vertical_size, horizontal_size;

  width = context->width;
  height = context->height;
  chroma_format = context->chroma_format;
  vertical_size = context->vertical_size;
  horizontal_size = context->horizontal_size;

  i = context->matrix_coefficients;
  if (i>8)
    i = 3;

  cr = coef[i-1][0];
  cg = coef[i-1][1];
  cb = coef[i-1][2];
  cu = 0.5/(1.0-cb);
  cv = 0.5/(1.0-cr);

  if (chroma_format==CHROMA444)
  {
    context->read_ppm_u444 = frame[1];
    context->read_ppm_v444 = frame[2];
  }
  else
  {
    if (!context->read_ppm_u444)
    {
      if (!(context->read_ppm_u444 = (unsigned char *)malloc(width*height)))
        simpeg_encode_error(context,"malloc failed");
      if (!(context->read_ppm_v444 = (unsigned char *)malloc(width*height)))
        simpeg_encode_error(context,"malloc failed");
      if (chroma_format==CHROMA420)
      {
        if (!(context->read_ppm_u422 = (unsigned char *)malloc((width>>1)*height)))
          simpeg_encode_error(context,"malloc failed");
        if (!(context->read_ppm_v422 = (unsigned char *)malloc((width>>1)*height)))
          simpeg_encode_error(context,"malloc failed");
      }
    }
  }

  sprintf(name,"%s.ppm",fname);

  if (!(fd = fopen(name,"rb")))
  {
    sprintf(context->errortext,"Couldn't open %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }

  /* skip header */
  getc(fd); getc(fd); /* magic number (P6) */
  pbm_getint(fd); pbm_getint(fd); pbm_getint(fd); /* width height maxcolors */

  for (i=0; i<vertical_size; i++)
  {
    yp = frame[0] + i*width;
    up = context->read_ppm_u444 + i*width;
    vp = context->read_ppm_v444 + i*width;

    for (j=0; j<horizontal_size; j++)
    {
      r=getc(fd); g=getc(fd); b=getc(fd);
      /* convert to YUV */
      y = cr*r + cg*g + cb*b;
      u = cu*(b-y);
      v = cv*(r-y);
      yp[j] = (219.0/256.0)*y + 16.5;  /* nominal range: 16..235 */
      up[j] = (224.0/256.0)*u + 128.5; /* 16..240 */
      vp[j] = (224.0/256.0)*v + 128.5; /* 16..240 */
    }
  }
  fclose(fd);

  border_extend(frame[0],horizontal_size,vertical_size,width,height);
  border_extend(context->read_ppm_u444,horizontal_size,vertical_size,width,height);
  border_extend(context->read_ppm_v444,horizontal_size,vertical_size,width,height);

  if (chroma_format==CHROMA422)
  {
    conv444to422(context,context->read_ppm_u444,frame[1]);
    conv444to422(context,context->read_ppm_v444,frame[2]);
  }

  if (chroma_format==CHROMA420)
  {
    conv444to422(context,context->read_ppm_u444,context->read_ppm_u422);
    conv444to422(context,context->read_ppm_v444,context->read_ppm_v422);
    conv422to420(context,context->read_ppm_u422,frame[1]);
    conv422to420(context,context->read_ppm_v422,frame[2]);
  }
}

static void SimpegWrite_read_buffer(simpeg_encode_context * context, unsigned char *buffer, unsigned char *frame[])
{
  int i, j;
  int r, g, b;
  double y, u, v;
  double cr, cg, cb, cu, cv;
  int heightsum, totsum;
  int iy, iu, iv;
  unsigned char *yp, *up, *vp;
  static double coef[7][3] = {
    {0.2125,0.7154,0.0721}, /* ITU-R Rec. 709 (1990) */
    {0.299, 0.587, 0.114},  /* unspecified */
    {0.299, 0.587, 0.114},  /* reserved */
    {0.30,  0.59,  0.11},   /* FCC */
    {0.299, 0.587, 0.114},  /* ITU-R Rec. 624-4 System B, G */
    {0.299, 0.587, 0.114},  /* SMPTE 170M */
    {0.212, 0.701, 0.087}}; /* SMPTE 240M (1987) */

  int width, height, chroma_format, vertical_size, horizontal_size;

  width = context->width;
  height = context->height;
  chroma_format = context->chroma_format;
  vertical_size = context->vertical_size;
  horizontal_size = context->horizontal_size;

  i = context->matrix_coefficients;
  if (i>8)
    i = 3;

  cr = coef[i-1][0];
  cg = coef[i-1][1];
  cb = coef[i-1][2];
  cu = 0.5/(1.0-cb);
  cv = 0.5/(1.0-cr);

  if (chroma_format==CHROMA444)
  {
    context->read_ppm_u444 = frame[1];
    context->read_ppm_v444 = frame[2];
  }
  else
  {
    if (!context->read_ppm_u444)
    {
      if (!(context->read_ppm_u444 = (unsigned char *)malloc(width*height)))
        simpeg_encode_error(context,"malloc failed");
      if (!(context->read_ppm_v444 = (unsigned char *)malloc(width*height)))
        simpeg_encode_error(context,"malloc failed");
      if (chroma_format==CHROMA420)
      {
        if (!(context->read_ppm_u422 = (unsigned char *)malloc((width>>1)*height)))
          simpeg_encode_error(context,"malloc failed");
        if (!(context->read_ppm_v422 = (unsigned char *)malloc((width>>1)*height)))
          simpeg_encode_error(context,"malloc failed");
      }
    }
  }

  totsum = vertical_size * horizontal_size *3;
  for (i=0; i<vertical_size; i++)
  {
    yp = frame[0] + i*width;
    up = context->read_ppm_u444 + i*width;
    vp = context->read_ppm_v444 + i*width;

    heightsum = totsum-(i+1)*horizontal_size*3; 

    for (j=0; j<horizontal_size; j++)
    {
      r=buffer[heightsum + j*3 + 0]; 
      g=buffer[heightsum + j*3 + 1]; 
      b=buffer[heightsum + j*3 + 2];
      /* convert to YUV */
      y = cr*r + cg*g + cb*b;
      u = cu*(b-y);
      v = cv*(r-y);

      iy = (219.0/256.0)*y + 16.5;  /* nominal range: 16..235 */
      iu = (224.0/256.0)*u + 128.5; /* 16..240 */
      iv = (224.0/256.0)*v + 128.5; /* 16..240 */
#if USE_CLIPTABLE
      yp[j] = context->clp[iy];
      up[j] = context->clp[iu];
      vp[j] = context->clp[iv];
#else
      yp[j] = iy < 0 ? 0 : (iy > 255 ? 255 : iy);
      up[j] = iu < 0 ? 0 : (iu > 255 ? 255 : iu);
      vp[j] = iv < 0 ? 0 : (iv > 255 ? 255 : iv);
#endif
    }
  }

  border_extend(frame[0],horizontal_size,vertical_size,width,height);
  border_extend(context->read_ppm_u444,horizontal_size,vertical_size,width,height);
  border_extend(context->read_ppm_v444,horizontal_size,vertical_size,width,height);

  if (chroma_format==CHROMA422)
  {
    conv444to422(context,context->read_ppm_u444,frame[1]);
    conv444to422(context,context->read_ppm_v444,frame[2]);
  }

  if (chroma_format==CHROMA420)
  {
    conv444to422(context,context->read_ppm_u444,context->read_ppm_u422);
    conv444to422(context,context->read_ppm_v444,context->read_ppm_v422);
    conv422to420(context,context->read_ppm_u422,frame[1]);
    conv422to420(context,context->read_ppm_v422,frame[2]);
  }
}


static void border_extend(unsigned char *frame, int w1, int h1, int w2, int h2)
{
  int i, j;
  unsigned char *fp;

  /* horizontal pixel replication (right border) */

  for (j=0; j<h1; j++)
  {
    fp = frame + j*w2;
    for (i=w1; i<w2; i++)
      fp[i] = fp[i-1];
  }

  /* vertical pixel replication (bottom border) */

  for (j=h1; j<h2; j++)
  {
    fp = frame + j*w2;
    for (i=0; i<w2; i++)
      fp[i] = fp[i-w2];
  }
}

/* horizontal filter and 2:1 subsampling */
static void 
conv444to422(simpeg_encode_context * context, unsigned char *src, unsigned char *dst)
{
  int i, j, im5, im4, im3, im2, im1, ip1, ip2, ip3, ip4, ip5, ip6;

  int width, height;
  int idx;

  width = context->width;
  height = context->height;

  if (context->mpeg1)
  {
    for (j=0; j<height; j++)
    {
      for (i=0; i<width; i+=2)
      {
        im5 = (i<5) ? 0 : i-5;
        im4 = (i<4) ? 0 : i-4;
        im3 = (i<3) ? 0 : i-3;
        im2 = (i<2) ? 0 : i-2;
        im1 = (i<1) ? 0 : i-1;
        ip1 = (i<width-1) ? i+1 : width-1;
        ip2 = (i<width-2) ? i+2 : width-1;
        ip3 = (i<width-3) ? i+3 : width-1;
        ip4 = (i<width-4) ? i+4 : width-1;
        ip5 = (i<width-5) ? i+5 : width-1;
        ip6 = (i<width-5) ? i+6 : width-1;

        /* FIR filter with 0.5 sample interval phase shift */
        idx = (int)(228*(src[i]+src[ip1])
                    +70*(src[im1]+src[ip2])
                    -37*(src[im2]+src[ip3])
                    -21*(src[im3]+src[ip4])
                    +11*(src[im4]+src[ip5])
                    + 5*(src[im5]+src[ip6])+256)>>9;

#if USE_CLIPTABLE
        dst[i>>1] = context->clp[idx];
#else
        dst[i>>1] = idx < 0 ? 0 : (idx > 255 ? 255 : idx);        
#endif
      }
      src+= width;
      dst+= width>>1;
    }
  }
  else
  {
    /* MPEG-2 */
    for (j=0; j<height; j++)
    {
      for (i=0; i<width; i+=2)
      {
        im5 = (i<5) ? 0 : i-5;
        im3 = (i<3) ? 0 : i-3;
        im1 = (i<1) ? 0 : i-1;
        ip1 = (i<width-1) ? i+1 : width-1;
        ip3 = (i<width-3) ? i+3 : width-1;
        ip5 = (i<width-5) ? i+5 : width-1;

        idx = (int)(22*(src[im5]+src[ip5])-52*(src[im3]+src[ip3])
                    +159*(src[im1]+src[ip1])+256*src[i]+256)>>9;
        
        /* FIR filter coefficients (*512): 22 0 -52 0 159 256 159 0 -52 0 22 */
#if USE_CLIPTABLE
        dst[i>>1] = context->clp[idx];
#else   
        dst[i>>1] = idx < 0 ? 0 : (idx > 255 ? 255 : idx);
#endif
      }
      src+= width;
      dst+= width>>1;
    }
  }
}

/* vertical filter and 2:1 subsampling */
static void 
conv422to420(simpeg_encode_context * context,unsigned char *src, unsigned char *dst)
{
  int w, i, j, jm6, jm5, jm4, jm3, jm2, jm1;
  int jp1, jp2, jp3, jp4, jp5, jp6;

  int idx;
  int width, height;

  width = context->width;
  height = context->height;

  w = width>>1;

  if (context->prog_frame)
  {
    /* intra frame */
    for (i=0; i<w; i++)
    {
      for (j=0; j<height; j+=2)
      {
        jm5 = (j<5) ? 0 : j-5;
        jm4 = (j<4) ? 0 : j-4;
        jm3 = (j<3) ? 0 : j-3;
        jm2 = (j<2) ? 0 : j-2;
        jm1 = (j<1) ? 0 : j-1;
        jp1 = (j<height-1) ? j+1 : height-1;
        jp2 = (j<height-2) ? j+2 : height-1;
        jp3 = (j<height-3) ? j+3 : height-1;
        jp4 = (j<height-4) ? j+4 : height-1;
        jp5 = (j<height-5) ? j+5 : height-1;
        jp6 = (j<height-5) ? j+6 : height-1;
        
        idx = (int)(228*(src[w*j]+src[w*jp1])
                    +70*(src[w*jm1]+src[w*jp2])
                    -37*(src[w*jm2]+src[w*jp3])
                    -21*(src[w*jm3]+src[w*jp4])
                    +11*(src[w*jm4]+src[w*jp5])
                    + 5*(src[w*jm5]+src[w*jp6])+256)>>9;
#if USE_CLIPTABLE
        /* FIR filter with 0.5 sample interval phase shift */
        dst[w*(j>>1)] = context->clp[idx];
#else
        dst[w*(j>>1)] = idx < 0 ? 0 : (idx > 255 ? 255 : idx);
#endif
      }
      src++;
      dst++;
    }
  }
  else
  {
    /* intra field */
    for (i=0; i<w; i++)
    {
      for (j=0; j<height; j+=4)
      {
        /* top field */
        jm5 = (j<10) ? 0 : j-10;
        jm4 = (j<8) ? 0 : j-8;
        jm3 = (j<6) ? 0 : j-6;
        jm2 = (j<4) ? 0 : j-4;
        jm1 = (j<2) ? 0 : j-2;
        jp1 = (j<height-2) ? j+2 : height-2;
        jp2 = (j<height-4) ? j+4 : height-2;
        jp3 = (j<height-6) ? j+6 : height-2;
        jp4 = (j<height-8) ? j+8 : height-2;
        jp5 = (j<height-10) ? j+10 : height-2;
        jp6 = (j<height-12) ? j+12 : height-2;

        /* FIR filter with 0.25 sample interval phase shift */
        idx = (int)(8*src[w*jm5]
                    +5*src[w*jm4]
                    -30*src[w*jm3]
                    -18*src[w*jm2]
                    +113*src[w*jm1]
                    +242*src[w*j]
                    +192*src[w*jp1]
                    +35*src[w*jp2]
                    -38*src[w*jp3]
                    -10*src[w*jp4]
                    +11*src[w*jp5]
                    +2*src[w*jp6]+256)>>9;
#if USE_CLIPTABLE
        dst[w*(j>>1)] = context->clp[idx];
#else
        dst[w*(j>>1)] = idx < 0 ? 0 : (idx > 255 ? 255 : idx);
#endif
        /* bottom field */
        jm6 = (j<9) ? 1 : j-9;
        jm5 = (j<7) ? 1 : j-7;
        jm4 = (j<5) ? 1 : j-5;
        jm3 = (j<3) ? 1 : j-3;
        jm2 = (j<1) ? 1 : j-1;
        jm1 = (j<height-1) ? j+1 : height-1;
        jp1 = (j<height-3) ? j+3 : height-1;
        jp2 = (j<height-5) ? j+5 : height-1;
        jp3 = (j<height-7) ? j+7 : height-1;
        jp4 = (j<height-9) ? j+9 : height-1;
        jp5 = (j<height-11) ? j+11 : height-1;
        jp6 = (j<height-13) ? j+13 : height-1;

        idx = (int)(8*src[w*jp6]
                    +5*src[w*jp5]
                    -30*src[w*jp4]
                    -18*src[w*jp3]
                    +113*src[w*jp2]
                    +242*src[w*jp1]
                    +192*src[w*jm1]
                    +35*src[w*jm2]
                    -38*src[w*jm3]
                    -10*src[w*jm4]
                    +11*src[w*jm5]
                    +2*src[w*jm6]+256)>>9;

        /* FIR filter with 0.25 sample interval phase shift */
#if USE_CLIPTABLE
        dst[w*((j>>1)+1)] = context->clp[idx];
#else
        dst[w*((j>>1)+1)] = idx < 0 ? 0 : (idx > 255 ? 255 : idx);
#endif
      }
      src++;
      dst++;
    }
  }
}

