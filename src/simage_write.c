#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <string.h>

#define MAX_EXTENSION_LENGTH 16

struct _saver_data
{
  struct simage_save_plugin funcs;
  struct _saver_data * next;
  char extension[MAX_EXTENSION_LENGTH+1];
  int is_internal;
};

typedef struct _saver_data saver_data;

/* built in image loaders */ 
#ifdef HAVE_JPEGLIB
#include <simage_jpeg.h>
static saver_data jpeg_saver;
static saver_data jpeg_saver2;
#endif /* HAVE_JPEGLIB */
#ifdef HAVE_PNGLIB
#include <simage_png.h>
static saver_data png_saver;
#endif /* HAVE_PNGLIB */
#ifdef HAVE_TIFFLIB
#include <simage_tiff.h>
static saver_data tiff_saver;
static saver_data tiff_saver2;
#endif /* HAVE_TIFFLIB */
#ifdef SIMAGE_RGB_SUPPORT
#include <simage_rgb.h>
static saver_data rgb_saver;
#endif /* SIMAGE_PIC_SUPPORT */
#include <assert.h>

static saver_data * first_saver = NULL;
static saver_data * last_saver = NULL;

/*
 * internal function which adds a saver to the list of savers
 * returns a void pointer to the saver. addbefore specifies
 * if the saver should be added at the beginning or at the
 * end of the linked list. useful if a user program finds
 * a bug in this library (simply use addbefore) 
 */
static void *
add_saver(saver_data * saver,
          int (*save_func)(const char *,
                           const unsigned char *,
                           int, int, int),
          const char * extension,
          int (*error_func)(char *, int),
          int is_internal,
          int addbefore)
{
  assert(saver);
  saver->funcs.save_func = save_func;
  strncpy(saver->extension, extension, MAX_EXTENSION_LENGTH);
  saver->extension[MAX_EXTENSION_LENGTH] = 0;
  saver->funcs.error_func = error_func;
  saver->is_internal = is_internal;
  saver->next = NULL;

  if (first_saver == NULL) first_saver = last_saver = saver;
  else {
    if (addbefore) {
      saver->next = first_saver;
      first_saver = saver;
    }
    else {
      last_saver->next = saver;
      last_saver = saver;
    }
  }
  return (void*) saver;
}

/*
 * internal function which finds the correct saver. Returns
 * NULL if none was found
 */
static saver_data *
find_saver(const char * filenameextension)
{
  saver_data * saver;
  saver = first_saver;
  while (saver) {
    if (!strcasecmp(saver->extension, filenameextension)) break;
    saver = saver->next;
  }
  return saver;
}

static void
add_internal_savers(void)
{
  static int first = 1;
  if (first) {
    first = 0;
#ifdef HAVE_JPEGLIB
    add_saver(&jpeg_saver, 
              simage_jpeg_save,
              "jpg",
              simage_jpeg_error,
              1, 0);
    add_saver(&jpeg_saver2, 
              simage_jpeg_save,
              "jpeg",
              simage_jpeg_error,
              1, 0);
#endif /* HAVE_JPEGLIB */
#ifdef HAVE_PNGLIB
    add_saver(&png_saver, 
              simage_png_save,
              "png",
              simage_png_error,
              1, 0);
#endif /* HAVE_PNGLIB */
#ifdef HAVE_TIFFLIB
    add_saver(&tiff_saver, 
              simage_tiff_save,
              "tiff",
              simage_tiff_error,
              1, 0);
    add_saver(&tiff_saver2, 
              simage_tiff_save,
              "tif",
              simage_tiff_error,
              1, 0);
#endif /* HAVE_TIFFLIB */
#ifdef SIMAGE_RGB_SUPPORT
    add_saver(&rgb_saver, 
              simage_rgb_save,
              "rgb",
              simage_rgb_error,
              1, 0);
#endif /* SIMAGE_RGB_SUPPORT */
  }
}

#define SIMAGE_ERROR_BUFSIZE 512 /* hack warning. Must match define in simage.c */
/* defined in simage.c */
extern char simage_error_msg[];

int
simage_save_image(const char * filename,
                  const unsigned char * bytes,
                  int width, int height, int numcomponents,
                  const char * filenameextension)
{
  saver_data * saver;

  simage_error_msg[0] = 0; /* clear error msg */
  add_internal_savers();  

  saver = find_saver(filenameextension);

  if (saver) {
    int ret = saver->funcs.save_func(filename, bytes, width,
                                     height, numcomponents);
    if (ret == 0) {
      (void) saver->funcs.error_func(simage_error_msg, SIMAGE_ERROR_BUFSIZE);
    }
    return ret;
  }
  else {
    strcpy(simage_error_msg, "Unsupported image format.");
    return 0;
  }
}

void *
simage_add_saver(const struct simage_save_plugin * plugin, int addbefore)
{
  add_internal_savers();
  return add_saver((saver_data *)malloc(sizeof(saver_data)), 
                   plugin->save_func, 
                   plugin->filenameextension,
                   plugin->error_func,
                   0, addbefore);
}

void 
simage_remove_saver(void * handle)
{
  saver_data *prev = NULL;
  saver_data *saver = first_saver;
  
  while (saver && saver != (saver_data*)handle) {
    prev = saver;
    saver = saver->next;
  }
  assert(saver);
  if (saver) { /* found it! */
    if (last_saver == saver) { /* new last_saver? */
      last_saver = prev;
    }
    if (prev) prev->next = saver->next;
    else first_saver = saver->next;
    if (saver) free(saver);
  }
}

int 
simage_check_save_supported(const char * filenameextension)
{
  saver_data * saver; 
  add_internal_savers();
  saver = find_saver(filenameextension);
  return saver != NULL ? 1 : 0;
}

