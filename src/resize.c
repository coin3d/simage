/*
 *		Filtered Image Rescaling
 *
 *		  by Dale Schumacher 
 */

/*
 * Slightly modified by pederb to be able to use it
 * in simage.
 *
 */


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

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
put_pixel(Image * image, int x, int y, double * data)
{
  int i, bpp;
  unsigned char * p;
  double val;

  assert(x >= 0);
  assert(x < image->xsize);
  assert(y >= 0);
  assert(y < image->ysize);
  
  bpp = image->bpp;
  p = image->data + image->span * y + x * bpp;
  for (i = 0; i < bpp; i++) {
    val = data[i];
    if (val < 0.0) val = 0.0;
    else if (val > 255.0) val = 255.0;
    *p++ = (unsigned char) val; 
  }
}


/*
 *	filter function definitions
 */

#define	filter_support		(1.0)

static double
filter(double t)
{
  /* f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1 */
  if(t < 0.0) t = -t;
  if(t < 1.0) return((2.0 * t - 3.0) * t * t + 1.0);
  return(0.0);
}

#define	box_support		(0.5)

static double
box_filter(double t)
{
  if((t > -0.5) && (t <= 0.5)) return(1.0);
  return(0.0);
}

#define	triangle_support	(1.0)

static double
triangle_filter(double t)
{
  if(t < 0.0) t = -t;
  if(t < 1.0) return(1.0 - t);
  return(0.0);
}

#define	bell_support		(1.5)

static double
bell_filter(double t)		/* box (*) box (*) box */
{
  if(t < 0) t = -t;
  if(t < .5) return(.75 - (t * t));
  if(t < 1.5) {
    t = (t - 1.5);
    return(.5 * (t * t));
  }
  return(0.0);
}

#define	B_spline_support	(2.0)

static double
B_spline_filter(double t)  /* box (*) box (*) box (*) box */
{
  double tt;
  
  if(t < 0) t = -t;
  if(t < 1) {
    tt = t * t;
    return((.5 * tt * t) - tt + (2.0 / 3.0));
  } else if(t < 2) {
    t = 2 - t;
    return((1.0 / 6.0) * (t * t * t));
  }
  return(0.0);
}

static double
sinc(double x)
{
  x *= M_PI;
  if(x != 0) return(sin(x) / x);
  return(1.0);
}

#define	Lanczos3_support	(3.0)

static double
Lanczos3_filter(double t)
{
  if(t < 0) t = -t;
  if(t < 3.0) return(sinc(t) * sinc(t/3.0));
  return(0.0);
}

#define	Mitchell_support	(2.0)

#define	B	(1.0 / 3.0)
#define	C	(1.0 / 3.0)

static double
Mitchell_filter(double t)
{
  double tt;

  tt = t * t;
  if(t < 0.0) t = -t;
  if(t < 1.0) {
    t = (((12.0 - 9.0 * B - 6.0 * C) * (t * tt))
         + ((-18.0 + 12.0 * B + 6.0 * C) * tt)
         + (6.0 - 2.0 * B));
    return (t / 6.0);
  } 
  else if (t < 2.0) {
    t = (((-1.0 * B - 6.0 * C) * (t * tt))
         + ((6.0 * B + 30.0 * C) * tt)
         + ((-12.0 * B - 48.0 * C) * t)
         + (8.0 * B + 24.0 * C));
    return (t / 6.0);
  }
  return(0.0);
}

/*
 *	image rescaling routine
 */

typedef struct {
  int pixel;
  double weight;
} CONTRIB;

typedef struct {
  int	n;		/* number of contributors */
  CONTRIB * p;		/* pointer to list of contributions */
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
  if (data == NULL) img->data = malloc(img->span*img->ysize);
  return img;
}

static void
zoom(Image * dst,               /* destination image structure */
     Image * src,               /* source image structure */
     double (*filterf)(double), /* filter function */
     double fwidth)             /* filter width (support) */
{
  CLIST * contrib;
  Image * tmp;                  /* intermediate image */
  double xscale, yscale;	/* zoom scale factors */
  int i, j, k, b;			/* loop variables */
  int n;			/* pixel number */
  double center, left, right;	/* filter calculation variables */
  double width, fscale, weight;	/* filter calculation variables */
  unsigned char * raster;	/* a row or column of pixels */
  double pixel[4];              /* one pixel */
  int bpp;
  
  bpp = src->bpp;
  /* create intermediate image to hold horizontal zoom */
  tmp = new_image(dst->xsize, src->ysize, dst->bpp, NULL);
  xscale = (double) dst->xsize / (double) src->xsize;
  yscale = (double) dst->ysize / (double) src->ysize;

  /* pre-calculate filter contributions for a row */
  contrib = (CLIST *)calloc(dst->xsize, sizeof(CLIST));
  if(xscale < 1.0) {
    width = fwidth / xscale;
    fscale = 1.0 / xscale;
    for(i = 0; i < dst->xsize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (width * 2 + 1),
                                       sizeof(CONTRIB));
      center = (double) i / xscale;
      left = ceil(center - width);
      right = floor(center + width);
      for(j = left; j <= right; j++) {
        weight = center - (double) j;
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
    for(i = 0; i < dst->xsize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (fwidth * 2 + 1),
                                       sizeof(CONTRIB));
      center = (double) i / xscale;
      left = ceil(center - fwidth);
      right = floor(center + fwidth);
      for(j = left; j <= right; j++) {
        weight = center - (double) j;
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

  for(k = 0; k < tmp->ysize; k++) {
    get_row(raster, src, k);
    for(i = 0; i < tmp->xsize; i++) {
      for (b = 0; b < bpp; b++) pixel[b] = 0.0;
      for(j = 0; j < contrib[i].n; j++) {
        for (b = 0; b < bpp; b++) {
          pixel[b] += raster[contrib[i].p[j].pixel+b]
            * contrib[i].p[j].weight;
        }
      }
      put_pixel(tmp, i, k, pixel);
    }
  }
  free(raster);
  
  /* free the memory allocated for horizontal filter weights */
  for(i = 0; i < tmp->xsize; i++) {
    free(contrib[i].p);
  }
  free(contrib);
  
  /* pre-calculate filter contributions for a column */
  contrib = (CLIST *)calloc(dst->ysize, sizeof(CLIST));
  if(yscale < 1.0) {
    width = fwidth / yscale;
    fscale = 1.0 / yscale;
    for(i = 0; i < dst->ysize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (width * 2 + 1),
                                       sizeof(CONTRIB));
      center = (double) i / yscale;
      left = ceil(center - width);
      right = floor(center + width);
      for(j = left; j <= right; j++) {
        weight = center - (double) j;
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
    for(i = 0; i < dst->ysize; i++) {
      contrib[i].n = 0;
      contrib[i].p = (CONTRIB *)calloc((int) (fwidth * 2 + 1),
                                       sizeof(CONTRIB));
      center = (double) i / yscale;
      left = ceil(center - fwidth);
      right = floor(center + fwidth);
      for(j = left; j <= right; j++) {
        weight = center - (double) j;
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
  for(k = 0; k < dst->xsize; k++) {
    get_column(raster, tmp, k);
    for(i = 0; i < dst->ysize; i++) {
      for (b = 0; b < bpp; b++) pixel[b] = 0.0;
      for(j = 0; j < contrib[i].n; ++j) {
        for (b = 0; b < bpp; b++) { 
          pixel[b] += raster[contrib[i].p[j].pixel+b]
            * contrib[i].p[j].weight;
        }
      }
      put_pixel(dst, k, i, pixel);
    }
  }

  free(raster);
  
  /* free the memory allocated for vertical filter weights */
  for(i = 0; i < dst->ysize; ++i) {
    free(contrib[i].p);
  }
  free(contrib);
  free(tmp->data);
  free(tmp);
}

unsigned char *
simage_resize(unsigned char * src, int width,
	      int height, int num_comp,
	      int newwidth, int newheight)
{
  unsigned char * dstdata;
  Image * srcimg, * dstimg;

  srcimg = new_image(width, height, num_comp, src);
  dstimg = new_image(newwidth, newheight, num_comp, NULL);

  zoom(dstimg, srcimg, Mitchell_filter, Mitchell_support);

  dstdata = dstimg->data;
  free(srcimg);
  free(dstimg);
  
  return dstdata;
}
