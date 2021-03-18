/*
 * Copyright (c) Kongsberg Oil & Gas Technologies
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 *                Filtered Image Rescaling
 *
 *                  by Dale Schumacher
 */

/*
 * Slightly modified by pederb to be able to use it
 * in simage.
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Need to include this so the compiler knows that the simage_resize()
   method should be defined with __declspec(dllexport) under
   MSWindows. */
#include <simage.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* !M_PI */

typedef struct {
  int xsize;            /* horizontal size of the image in Pixels */
  int ysize;            /* vertical size of the image in Pixels */
  int bpp;              /* bytes per pixel */
  unsigned char * data; /* pointer to first scanline of image */
  int span;             /* byte offset between two scanlines */
} Image;

static void
get_row(unsigned char * row, Image * image, int y)
{
  assert(y >= 0);
  assert(y < image->ysize);

  memcpy(row,
         image->data + (y * image->span),
         (image->bpp * image->xsize));
}

static void
get_column(unsigned char * column, Image * image, int x)
{
  int i, j, d, bpp, ysize;
  unsigned char * p;

  assert(x >= 0);
  assert(x < image->xsize);

  d = image->span;
  bpp = image->bpp;
  ysize = image->ysize;
  for(i = 0, p = image->data + x * bpp; i < ysize; p += d, i++) {
    for (j = 0; j < bpp; j++) {
      *column++ = p[j];
    }
  }
}

static void
put_pixel(Image * image, int x, int y, float * data)
{
  int i, bpp;
  unsigned char * p;
  float val;

  assert(x >= 0);
  assert(x < image->xsize);
  assert(y >= 0);
  assert(y < image->ysize);

  bpp = image->bpp;
  p = image->data + image->span * y + x * bpp;
  for (i = 0; i < bpp; i++) {
    val = data[i];
    if (val < 0.0f) val = 0.0f;
    else if (val > 255.0f) val = 255.0f;
    *p++ = (unsigned char) val;
  }
}


/*
 *        filter function definitions
 */

#define        filter_support                (1.0f)

static float
filter(float t)
{
  /* f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1 */
  if(t < 0.0f) t = -t;
  if(t < 1.0f) return((2.0f * t - 3.0f) * t * t + 1.0f);
  return(0.0f);
}

#define        box_support                (0.5f)

static float
box_filter(float t)
{
  if((t > -0.5f) && (t <= 0.5f)) return(1.0f);
  return(0.0f);
}

#define        triangle_support        (1.0f)

static float
triangle_filter(float t)
{
  if(t < 0.0f) t = -t;
  if(t < 1.0f) return(1.0f - t);
  return(0.0f);
}

#define        bell_support                (1.5f)

static float
bell_filter(float t)                /* box (*) box (*) box */
{
  if(t < 0.0f) t = -t;
  if(t < 0.5f) return(0.75f - (t * t));
  if(t < 1.5f) {
    t = (t - 1.5f);
    return(0.5f * (t * t));
  }
  return(0.0f);
}

#define        B_spline_support        (2.0f)

static float
B_spline_filter(float t)  /* box (*) box (*) box (*) box */
{
  float tt;

  if(t < 0.0f) t = -t;
  if(t < 1.0f) {
    tt = t * t;
    return((.5f * tt * t) - tt + (2.0f / 3.0f));
  } else if(t < 2.0f) {
    t = 2.0f - t;
    return((1.0f / 6.0f) * (t * t * t));
  }
  return(0.0f);
}

static float
sinc(float x)
{
  x *= (float)M_PI;
  if(x != 0.0f) return (float)(sin(x) / x);
  return(1.0f);
}

#define        Lanczos3_support        (3.0f)

static float
Lanczos3_filter(float t)
{
  if(t < 0.0f) t = -t;
  if(t < 3.0f) return(sinc(t) * sinc(t/3.0f));
  return(0.0f);
}

#define        Mitchell_support        (2.0)

#define        B        (1.0f / 3.0f)
#define        C        (1.0f / 3.0f)

static float
Mitchell_filter(float t)
{
  float tt;

  tt = t * t;
  if(t < 0.0f) t = -t;
  if(t < 1.0f) {
    t = (((12.0f - 9.0f * B - 6.0f * C) * (t * tt))
         + ((-18.0f + 12.0f * B + 6.0f * C) * tt)
         + (6.0f - 2.0f * B));
    return (t / 6.0f);
  }
  else if (t < 2.0f) {
    t = (((-1.0f * B - 6.0f * C) * (t * tt))
         + ((6.0f * B + 30.0f * C) * tt)
         + ((-12.0f * B - 48.0f * C) * t)
         + (8.0f * B + 24.0f * C));
    return (t / 6.0f);
  }
  return(0.0f);
}

/*
 *        image rescaling routine
 */

typedef struct {
  int pixel;
  float weight;
} CONTRIB;

typedef struct {
  int        n;                /* number of contributors */
  CONTRIB * p;                /* pointer to list of contributions */
} CLIST;

static Image *
new_image(int xsize, int ysize, int bpp, unsigned char * data)
{
  Image * img = (Image*) malloc(sizeof(Image));
  img->xsize = xsize;
  img->ysize = ysize;
  img->bpp = bpp;
  img->span = xsize * bpp;
  img->data = data;
  if (data == NULL) img->data = (unsigned char*) malloc((size_t)img->span*img->ysize);
  return img;
}

static void
zoom(Image * dst,               /* destination image structure */
     Image * src,               /* source image structure */
     float (*filterf)(float), /* filter function */
     float fwidth)             /* filter width (support) */
{
  CLIST * contrib;
  Image * tmp;                  /* intermediate image */
  float xscale, yscale;        /* zoom scale factors */
  int i, j, k, b;                        /* loop variables */
  int n;                        /* pixel number */
  int left, right;        /* filter calculation variables */
  float center;        /* filter calculation variables */
  float width, fscale, weight;        /* filter calculation variables */
  unsigned char * raster;        /* a row or column of pixels */
  float pixel[4];              /* one pixel */
  int bpp;
  unsigned char * dstptr;
  int dstxsize, dstysize;

  bpp = src->bpp;
  dstxsize = dst->xsize;
  dstysize = dst->ysize;

  /* create intermediate image to hold horizontal zoom */
  tmp = new_image(dstxsize, src->ysize, dst->bpp, NULL);
  xscale = (float) dstxsize / (float) src->xsize;
  yscale = (float) dstysize / (float) src->ysize;

  /* pre-calculate filter contributions for a row */
  contrib = (CLIST *)calloc(dstxsize, sizeof(CLIST));
  if(xscale < 1.0f) {
    width = fwidth / xscale;
    fscale = 1.0f / xscale;
    for(i = 0; i < dstxsize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (width * 2 + 1),
                                       sizeof(CONTRIB));
      center = (float) i / xscale;
      left = (int) ceil(center - width);
      right = (int) floor(center + width);
      for(j = left; j <= right; j++) {
        weight = center - (float) j;
        weight = (*filterf)(weight / fscale) / fscale;
        if(j < 0) {
          n = -j;
        }
        else if(j >= src->xsize) {
          n = (src->xsize - j) + src->xsize - 1;
        }
        else {
          n = j;
        }
        k = contrib[i].n++;
        contrib[i].p[k].pixel = n*bpp;
        contrib[i].p[k].weight = weight;
      }
    }
  }
  else {
    for(i = 0; i < dstxsize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (fwidth * 2 + 1),
                                       sizeof(CONTRIB));
      center = (float) i / xscale;
      left = (int) ceil(center - fwidth);
      right = (int) floor(center + fwidth);
      for(j = left; j <= right; j++) {
        weight = center - (float) j;
        weight = (*filterf)(weight);
        if(j < 0) {
          n = -j;
        }
        else if(j >= src->xsize) {
          n = (src->xsize - j) + src->xsize - 1;
        }
        else {
          n = j;
        }
        k = contrib[i].n++;
        contrib[i].p[k].pixel = n*bpp;
        contrib[i].p[k].weight = weight;
      }
    }
  }

  /* apply filter to zoom horizontally from src to tmp */
  raster = (unsigned char *)calloc(src->xsize, src->bpp);

  dstptr = tmp->data;

  for(k = 0; k < tmp->ysize; k++) {
    get_row(raster, src, k);
    for(i = 0; i < tmp->xsize; i++) {
      for (b = 0; b < bpp; b++) pixel[b] = 0.0f;
      for(j = 0; j < contrib[i].n; j++) {
        for (b = 0; b < bpp; b++) {
          pixel[b] += raster[contrib[i].p[j].pixel+b]
            * contrib[i].p[j].weight;
        }
      }
#if 1 /* obsoleted 2001-11-18 pederb. Too slow */
      put_pixel(tmp, i, k, pixel);
#else /* new code */
      for (b = 0; b < bpp; b++) {
        float val = pixel[b];
        if (val < 0.0f) val = 0.0f;
        else if (val > 255.0f) val = 255.0f;
        *dstptr++ = (unsigned char ) val;
      }
#endif /* new, faster code */
    }
  }
  free(raster);

  /* free the memory allocated for horizontal filter weights */
  for(i = 0; i < tmp->xsize; i++) {
    free(contrib[i].p);
  }
  free(contrib);

  /* pre-calculate filter contributions for a column */
  contrib = (CLIST *)calloc(dstysize, sizeof(CLIST));
  if(yscale < 1.0f) {
    width = fwidth / yscale;
    fscale = 1.0f / yscale;
    for(i = 0; i < dstysize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (width * 2 + 1),
                                       sizeof(CONTRIB));
      center = (float) i / yscale;
      left = (int) ceil(center - width);
      right = (int) floor(center + width);
      for(j = left; j <= right; j++) {
        weight = center - (float) j;
        weight = (*filterf)(weight / fscale) / fscale;
        if(j < 0) {
          n = -j;
        }
        else if(j >= tmp->ysize) {
          n = (tmp->ysize - j) + tmp->ysize - 1;
        }
        else {
          n = j;
        }
        k = contrib[i].n++;
        contrib[i].p[k].pixel = n*bpp;
        contrib[i].p[k].weight = weight;
      }
    }
  }
  else {
    for(i = 0; i < dstysize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (fwidth * 2 + 1),
                                       sizeof(CONTRIB));
      center = (float) i / yscale;
      left = (int) ceil(center - fwidth);
      right = (int) floor(center + fwidth);
      for(j = left; j <= right; j++) {
        weight = center - (float) j;
        weight = (*filterf)(weight);
        if(j < 0) {
          n = -j;
        }
        else if(j >= tmp->ysize) {
          n = (tmp->ysize - j) + tmp->ysize - 1;
        }
        else {
          n = j;
        }
        k = contrib[i].n++;
        contrib[i].p[k].pixel = n*bpp;
        contrib[i].p[k].weight = weight;
      }
    }
  }

  /* apply filter to zoom vertically from tmp to dst */
  raster = (unsigned char *) calloc(tmp->ysize, tmp->bpp);
  for(k = 0; k < dstxsize; k++) {
    get_column(raster, tmp, k);
    dstptr = dst->data + k * bpp;
    for(i = 0; i < dstysize; i++) {
      for (b = 0; b < bpp; b++) pixel[b] = 0.0f;
      for(j = 0; j < contrib[i].n; ++j) {
        for (b = 0; b < bpp; b++) {
          pixel[b] += raster[contrib[i].p[j].pixel+b]
            * contrib[i].p[j].weight;
        }
      }
#if 1 /* obsoleted 2001-11-18 pederb. Too slow */
      put_pixel(dst, k, i, pixel);
#else /* new code */
      for (b = 0; b < bpp; b++) {
        float val = pixel[b];
        if (val < 0.0f) val = 0.0f;
        else if (val > 255.0f) val = 255.0f;
        dstptr[b] = (unsigned char) val;
      }
#endif /* new, faster code */
      dstptr += bpp * dstxsize;
    }
  }

  free(raster);

  /* free the memory allocated for vertical filter weights */
  for(i = 0; i < dstysize; ++i) {
    free(contrib[i].p);
  }
  free(contrib);
  free(tmp->data);
  free(tmp);
}

/*
 * a pretty lame resize-function
 */
static unsigned char *
simage_resize_fast(unsigned char *src, int width,
                   int height, int num_comp,
                   int newwidth, int newheight)
{
  float sx, sy, dx, dy;
  int src_bpr, dest_bpr, xstop, ystop, x, y, offset, i;
  unsigned char *dest =
    (unsigned char*) malloc((size_t)newwidth*newheight*num_comp);

  dx = ((float)width)/((float)newwidth);
  dy = ((float)height)/((float)newheight);
  src_bpr = width * num_comp;
  dest_bpr = newwidth * num_comp;

  sy = 0.0f;
  ystop = newheight * dest_bpr;
  xstop = newwidth * num_comp;
  for (y = 0; y < ystop; y += dest_bpr) {
    sx = 0.0f;
    for (x = 0; x < xstop; x += num_comp) {
      offset = ((int)sy)*src_bpr + ((int)sx)*num_comp;
      for (i = 0; i < num_comp; i++) dest[x+y+i] = src[offset+i];
      sx += dx;
    }
    sy += dy;
  }
  return dest;
}


// FIXME: pederb suspects that MSVC++ v6 mis-compiles some code in the
// resize functionality -- which again causes a crash whenever (?) a
// resize is attempted with a simage-library compiled in release mode.
// This sounds very nasty with regard to application programmers using
// Coin / simage, and should be investigated ASAP. 20010904 mortene.

// UPDATE: I haven't observed any crashes in the resize functionality
// for several years. That goes for debug builds as well as for
// release builds. Perhaps the bug has been fixed in the latest
// service pack, SP5 (which I'm using). 20031210 thammer.

// FIXME: methinks the type of the first argument should have been
// ``const unsigned char *''. Can't change it now, though, as it'd
// probably break ABI compatibility (?). Wait for simage2. 20010809 mortene.

unsigned char *
simage_resize(unsigned char * src, int width,
              int height, int num_comp,
              int newwidth, int newheight)
{
  unsigned char * dstdata;
  Image * srcimg, * dstimg;

#if 0 /* for comparing speed of resize functions */
  return simage_resize_fast(src, width,
                            height, num_comp,
                            newwidth, newheight);
#endif /* testing only */
  srcimg = new_image(width, height, num_comp, src);
  dstimg = new_image(newwidth, newheight, num_comp, NULL);

  /* Using the bell filter as default */
  zoom(dstimg, srcimg, bell_filter, bell_support);

  dstdata = dstimg->data;
  free(srcimg);
  free(dstimg);
  return dstdata;
}
