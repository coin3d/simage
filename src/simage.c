/*
 * simage.cpp
 */

#include <config.h>

#include "simage.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct _loader_data
{
  struct simage_plugin funcs;
  struct _loader_data *next;
  int is_internal;
};

typedef struct _loader_data loader_data;

/* built in image loaders */ 
#ifdef SIMAGE_JPEG_SUPPORT
#include "simage_jpeg.h"
static loader_data jpeg_loader;
#endif /* SIMAGE_JPEG_SUPPORT */
#ifdef SIMAGE_PNG_SUPPORT
#include "simage_png.h"
static loader_data png_loader;
#endif /* SIMAGE_PNG_SUPPORT */
#ifdef SIMAGE_TGA_SUPPORT
#include "simage_tga.h"
static loader_data targa_loader;
#endif /* SIMAGE_TGA_SUPPORT */
#ifdef SIMAGE_TIFF_SUPPORT
#include "simage_tiff.h"
static loader_data tiff_loader;
#endif /* SIMAGE_TIFF_SUPPORT */
#ifdef SIMAGE_PIC_SUPPORT
#include "simage_pic.h"
static loader_data pic_loader;
#endif /* SIMAGE_PIC_SUPPORT */
#ifdef SIMAGE_RGB_SUPPORT
#include "simage_rgb.h"
static loader_data rgb_loader;
#endif /* SIMAGE_PIC_SUPPORT */

#include <assert.h>

static loader_data *first_loader = NULL;
static loader_data *last_loader = NULL;

/*
 * internal function which adds a loader to the list of loaders
 * returns a void pointer to the loader. addbefore specifies
 * if the loader should be added at the beginning or at the
 * end of the linked list. useful if a user program finds
 * a bug in this library (simply use addbefore) 
 */
static void *
add_loader(loader_data *loader,
	   unsigned char *(*load_func)(const char *,
				       int *,
				       int *,
				       int *),
	   int (*identify_func)(const char *,
				const unsigned char *,
				int headerlen),
	   int (*error_func)(char *, int),
	   int is_internal,
	   int addbefore)
{
  assert(loader);
  loader->funcs.load_func = load_func;
  loader->funcs.identify_func = identify_func;
  loader->funcs.error_func = error_func;
  loader->is_internal = is_internal;
  loader->next = NULL;

  if (first_loader == NULL) first_loader = last_loader = loader;
  else {
    if (addbefore) {
      loader->next = first_loader;
      first_loader = loader;
    }
    else {
      last_loader->next = loader;
      last_loader = loader;
    }
  }
  return (void*) loader;
}

/*
 * internal function which finds the correct loader. Returns
 * NULL if none was found
 */
static loader_data *
find_loader(const char *filename)
{
  loader_data *loader;
  int readlen;
  unsigned char buf[256] = {0};
  FILE *fp = fopen(filename, "rb");
  if (!fp) return NULL;

  readlen = fread(buf, 1, 256, fp);
  fclose(fp);
  if (readlen <= 0) return NULL;

  loader = first_loader;
  while (loader) {
    if (loader->funcs.identify_func(filename, buf, readlen)) break;
    loader = loader->next;
  }
  return loader;
}


static void
add_internal_loaders()
{
  static int first = 1;
  if (first) {
    first = 0;
#ifdef SIMAGE_JPEG_SUPPORT
    add_loader(&jpeg_loader, 
	       simage_jpeg_load,
	       simage_jpeg_identify,
	       simage_jpeg_error,
	       1, 0);
#endif /* SIMAGE_JPEG_SUPPORT */
#ifdef SIMAGE_PNG_SUPPORT
    add_loader(&png_loader, 
	       simage_png_load,
	       simage_png_identify,
	       simage_png_error,
	       1, 0);
#endif /* SIMAGE_PNG_SUPPORT */
#ifdef SIMAGE_TGA_SUPPORT
    add_loader(&targa_loader, 
	       simage_tga_load,
	       simage_tga_identify,
	       simage_tga_error,
	       1, 0);
#endif /* SIMAGE_TGA_SUPPORT */
#ifdef SIMAGE_TIFF_SUPPORT
    add_loader(&tiff_loader, 
	       simage_tiff_load,
	       simage_tiff_identify,
	       simage_tiff_error,
	       1, 0);
#endif /* SIMAGE_TIFF_SUPPORT */
#ifdef SIMAGE_RGB_SUPPORT
    add_loader(&rgb_loader, 
	       simage_rgb_load,
	       simage_rgb_identify,
	       simage_rgb_error,
	       1, 0);
#endif /* SIMAGE_RGB_SUPPORT */
#ifdef SIMAGE_PIC_SUPPORT
    add_loader(&pic_loader, 
	       simage_pic_load,
	       simage_pic_identify,
	       simage_pic_error,
	       1, 0);
#endif /* SIMAGE_PIC_SUPPORT */
  }
}

unsigned char *
simage_read_image(const char *filename,
		   int *width, int *height,
		   int *numComponents)
{
  FILE *fp;
  char buf[256] = {0};
  loader_data *loader;

  add_internal_loaders();
  
  fp = fopen(filename, "rb");
  if (!fp) return NULL;

  if (fread(buf, 1, 256, fp) <= 0) {
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  
  loader = find_loader(filename);
  if (loader) return loader->funcs.load_func(filename, width, height, numComponents);
  else return NULL;
}

int 
simage_check_supported(const char *filename)
{
  add_internal_loaders();
  return find_loader(filename) != NULL;
}

void *
simage_add_plugin_loader(const struct simage_plugin *plugin, int addbefore)
  
{
  add_internal_loaders();
  return add_loader(malloc(sizeof(loader_data)), 
		    plugin->load_func, 
		    plugin->identify_func,
		    plugin->error_func,
		    0, addbefore);
}

void 
simage_remove_plugin_loader(void *handle)
{
  loader_data *prev = NULL;
  loader_data *loader = first_loader;
  
  while (loader && loader != (loader_data*)handle) {
    prev = loader;
    loader = loader->next;
  }
  assert(loader);
  if (loader) { /* found it! */
    if (last_loader == loader) { /* new last_loader? */
      last_loader = prev;
    }
    if (prev) prev->next = loader->next;
    else first_loader = loader->next;
    if (loader) free(loader);
  }
}

/*
 * a pretty lame resize-function
 */
unsigned char *
simage_resize(unsigned char *src, int width,
	      int height, int num_comp,
	      int newwidth, int newheight)
{
  float sx, sy, dx, dy;
  int src_bpr, dest_bpr, xstop, ystop, x, y, offset, i;
  unsigned char *dest = 
    (unsigned char*) malloc(newwidth*newheight*num_comp);
  
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


/*
 * a helpful function
 */
static int 
cnt_bits(int val, int * highbit)
{
  int cnt = 0;
  *highbit = 0;
  while (val) {
    if (val & 1) cnt++;
    val>>=1;
    *highbit++;
  }
  return cnt;  
}

int 
simage_next_power_of_two(int val)
{
  int highbit;
  if (cnt_bits(val, &highbit) > 1) {
    return 1<<highbit;
  }
  return val;  
}
