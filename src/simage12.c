#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <string.h>

struct simage_image_s {
  int width;
  int height;
  int numcomponents;
  int didalloc;
  unsigned char * data;
};

s_image * 
s_image_create(int w, int h, int components,
               unsigned char * prealloc /* | NULL */ )
{
  s_image * image = (s_image*) malloc(sizeof(s_image));
  image->width = w;
  image->height = h;
  image->numcomponents = components;
  image->didalloc = 0;
  image->data = prealloc;
  if (image->data == NULL) {
    image->didalloc = 1;
    image->data = (unsigned char *) malloc(w*h*components);
  }
  return (s_image*) image;
}

void 
s_image_destroy(s_image * image)
{
  if (image) {
    if (image->didalloc) free((void*)image->data);
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
  if (image) return image->numcomponents;
  return 0;
}

unsigned char * 
s_image_data(s_image * image)
{
  if (image) return image->data;
  return NULL;
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
      prealloc->numcomponents != nc) {
    prealloc = s_image_create(w, h, nc, data);
    prealloc->didalloc = 1; /* we did alloc this data */
  }
  else {
    /* copy into preallocated buffer */
    memcpy(prealloc->data, data, w*h*nc);
    
    /* we don't need this copy any more */
    simage_free_image(data);
  }
  return prealloc;
}

int 
s_image_save(const char * filename, s_image * image,
             s_params * params /* | NULL */)
{
  char * ext = (char*) strrchr(filename, '.');
  if (ext == NULL) return 0;
  
  return simage_save_image(filename, 
                           image->data, 
                           image->width,
                           image->height,
                           image->numcomponents,
                           ext);
}
