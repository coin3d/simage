#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <string.h>

struct _saver_data
{
  int (*save_func)(const char * name, const unsigned char * bytes,
                   int width, int height, int numcomponents);
  int (*error_func)(char * textbuffer, int bufferlen);
  char * extensions;
  char * fullname;
  char * description;
  struct _saver_data * next;
  int is_internal;
};

typedef struct _saver_data saver_data;

/* built in image loaders */ 
#ifdef HAVE_JPEGLIB
#include <simage_jpeg.h>
static saver_data jpeg_saver;
#endif /* HAVE_JPEGLIB */
#ifdef HAVE_PNGLIB
#include <simage_png.h>
static saver_data png_saver;
#endif /* HAVE_PNGLIB */
#ifdef HAVE_TIFFLIB
#include <simage_tiff.h>
static saver_data tiff_saver;
#endif /* HAVE_TIFFLIB */
#ifdef SIMAGE_RGB_SUPPORT
#include <simage_rgb.h>
static saver_data rgb_saver;
#endif /* SIMAGE_PIC_SUPPORT */
#include <assert.h>

static saver_data * first_saver = NULL;
static saver_data * last_saver = NULL;

static char * 
safe_strdup(const char * str)
{
  char * newstr = NULL;
  if (str != NULL) {
    newstr = malloc(strlen(str) + 1);
    strcpy(newstr, str);
  }
  return newstr;
}

static void
safe_strfree(char * str)
{
  if (str != NULL) free(str);
}

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
          int (*error_func)(char *, int),
          const char * extensions,
          const char * fullname,
          const char * description,
          int is_internal,
          int addbefore)
{
  assert(saver);
  saver->save_func = save_func;
  saver->extensions = safe_strdup(extensions);
  saver->fullname = safe_strdup(fullname);
  saver->description = safe_strdup(description);
  saver->error_func = error_func;
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
    char * ext = saver->extensions;
    char * str = strchr(ext, ',');
    while (str) {
      int cmp;
      /* modify string while comparing. string is a copy so it should be safe */
      *str = 0;
      cmp = strcasecmp(ext, filenameextension);
      *str = ',';
      if (cmp == 0) return saver;
      ext = str + 1;
      str = strchr(ext, ',');
    }
    if (!strcasecmp(ext, filenameextension)) return saver;
    saver = saver->next;
  }
  return NULL;
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
              simage_jpeg_error,
              "jpg,jpeg",
              "The Independent JPEG Group file format", 
              NULL,
              1, 0);
#endif /* HAVE_JPEGLIB */
#ifdef HAVE_PNGLIB
    add_saver(&png_saver, 
              simage_png_save,
              simage_png_error,
              "png",
              "The PNG file format",
              NULL,
              1, 0);
#endif /* HAVE_PNGLIB */
#ifdef HAVE_TIFFLIB
    add_saver(&tiff_saver, 
              simage_tiff_save,
              simage_tiff_error,
              "tiff,tif",
              "The Tag Image File Format",
              NULL,
              1, 0);
#endif /* HAVE_TIFFLIB */
#ifdef SIMAGE_RGB_SUPPORT
    add_saver(&rgb_saver, 
              simage_rgb_save,
              simage_rgb_error,
              "rgb,rgba,bw,inta,int",
              "The SGI RGB file format.",
              NULL,
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
    int ret = saver->save_func(filename, bytes, width,
                                     height, numcomponents);
    if (ret == 0) {
      (void) saver->error_func(simage_error_msg, SIMAGE_ERROR_BUFSIZE);
    }
    return ret;
  }
  else {
    strcpy(simage_error_msg, "Unsupported image format.");
    return 0;
  }
}

void * 
simage_add_saver(int (*save_func)(const char * name,
                                  const unsigned char * bytes,
                                  int width, int height, int nc),
                 int (*error_func)(char * textbuffer, int bufferlen),
                 const char * extensions,
                 const char * fullname,
                 const char * description,
                 int addbefore)
{
  add_internal_savers();
  return add_saver((saver_data *)malloc(sizeof(saver_data)), 
                   save_func, 
                   error_func,
                   extensions,
                   fullname,
                   description,
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
    safe_strfree(saver->extensions);
    safe_strfree(saver->fullname);
    safe_strfree(saver->description);
    free(saver);
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

int 
simage_get_num_savers(void)
{
  int cnt;
  saver_data * saver = first_saver;
  cnt = 0;
  while (saver) {
    cnt++;
    saver = saver->next;
  }
  return cnt;
}

void * 
simage_get_saver_handle(int idx)
{
  saver_data * saver = first_saver;
  while (saver && idx) {
    saver = saver->next;
    idx--;
  }
  return (void*) saver;
}

const char * 
simage_get_saver_extensions(void * handle)
{
  saver_data * saver = (saver_data *) handle;
  return saver->extensions;
}

const char * 
simage_get_saver_fullname(void * handle)
{
  saver_data * saver = (saver_data *) handle;
  return saver->fullname;
}

const char * 
simage_get_saver_description(void * handle)
{
  saver_data * saver = (saver_data *) handle;
  return saver->description;
}

