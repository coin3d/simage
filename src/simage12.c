#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <simage_private.h>
#include <string.h>

s_image * 
s_image_create(int w, int h, int components,
               unsigned char * prealloc /* | NULL */ )
{
  s_image * image = (s_image*) malloc(sizeof(s_image));
  image->width = w;
  image->height = h;
  image->components = components;
  image->order = SIMAGE_ORDER_RGB; 
  image->didalloc = 0;
  image->data = prealloc;
  if (image->data == NULL) {
    image->didalloc = 1;
    image->data = (unsigned char *) malloc(w*h*components);
  }
  
  /* needed for simage 1.6 */
  image->opendata = NULL; 
  image->oktoreadall = 1;
  image->openfilename = NULL;
  memset(&image->openfuncs, 0, sizeof(struct simage_open_funcs));

  /* return image struct */
  return (s_image*) image;
}

void 
s_image_destroy(s_image * image)
{
  if (image) {
    if (image->didalloc) free((void*)image->data);

    if (image->opendata) {
      image->openfuncs.close_func(image->opendata);
    }
    if (image->openfilename) {
      free((void*) image->openfilename);
    }
    free((void*)image);
  }
}

int 
s_image_width(s_image * image)
{
  if (image) return image->width;
  return 0;
}

int 
s_image_height(s_image * image)
{
  if (image) return image->height;
  return 0;
}

int 
s_image_components(s_image * image)
{
  if (image) return image->components;
  return 0;
}


int 
s_image_set_component_order(s_image * image, int order)
{
  int oldorder = image->order;
  image->order = order;
  return oldorder;
}

int 
s_image_get_component_order(s_image * image)
{
  return image->order;
}

unsigned char * 
s_image_data(s_image * image)
{
  if (image) {
    if (image->opendata && image->data == NULL) {
      int i;
      image->data = (unsigned char *) malloc(image->width*image->height*image->components);
      image->didalloc = 1;
      for (i = 0; i < image->height; i++) {
        (void) s_image_read_line(image, i,
                                 image->data+image->width*image->components);
      }
    }
    return image->data;
  }
  return NULL;
}

void 
s_image_set(s_image * image, int w, int h, int components,
            unsigned char * data, int copydata)
{
  if (image->width == w && image->height == h && image->components == components) {
    if (copydata) {
      if (!image->didalloc) {
        /* we shouldn't overwrite preallocated data */
        image->data = (unsigned char*) malloc(w*h*components);
        image->didalloc = 1;
      }
      memcpy(image->data, data, w*h*components);
    }
    else {
      if (image->didalloc) free((void*) image->data);
      image->data = data;
      image->didalloc = 0;
    }
  }
  else {
    if (image->didalloc) free((void*) image->data);
    image->width = w;
    image->height = h;
    image->components = components;
    if (copydata) {
      image->data = (unsigned char *) malloc(w*h*components);
      image->didalloc = 1;
      memcpy(image->data, data, w*h*components);
    }
    else {
      image->data = data;
      image->didalloc = 0;
    }
  }
  image->order = SIMAGE_ORDER_RGB;
}

s_image * 
s_image_load(const char * filename, s_image * prealloc /* | NULL */)
{
  unsigned char * data;
  int w,h,nc;
  
  data = simage_read_image(filename, &w, &h, &nc);
  if (data == NULL) return NULL;
  if (prealloc == NULL || 
      prealloc->width != w || 
      prealloc->height != h ||
      prealloc->components != nc) {
    prealloc = s_image_create(w, h, nc, data);
    prealloc->didalloc = 1; /* we did alloc this data */
  }
  else {
    /* copy into preallocated buffer */
    memcpy(prealloc->data, data, w*h*nc);
    
    /* we don't need this copy any more */
    simage_free_image(data);
  }
  prealloc->order = SIMAGE_ORDER_RGB;
  prealloc->openfilename = (char*) malloc(strlen(filename) + 1);
  strcpy(prealloc->openfilename, filename);
  return prealloc;
}

int 
s_image_save(const char * filename, s_image * image,
             s_params * params /* | NULL */)
{
  char * ext = NULL;
  if (params != NULL) {
    s_params_get(params, S_STRING_PARAM_TYPE, 
                 "file type", &ext,
                 NULL);
  }
  if (ext == NULL) {
    ext = (char*) strrchr(filename, '.');
    if (ext == NULL) return 0;
    
    /* skip period */
    ext++;
  }
  
  return simage_save_image(filename, 
                           s_image_data(image),
                           image->width,
                           image->height,
                           image->components,
                           ext);
}
