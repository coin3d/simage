#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <string.h>
#include <ctype.h>

struct _saver_data
{
  int (*save_func)(const char * name, const unsigned char * bytes,
		   int width, int height, int numcomponents);
  int (*save_func_ext)(const char * name, const unsigned char * bytes,
		       int width, int height, int numcomponents, const char * ext);
  int (*error_func)(char * textbuffer, int bufferlen);
  char * extensions;
  char * fullname;
  char * description;
  struct _saver_data * next;
  int is_internal;
};

typedef struct _saver_data saver_data;

/* built in image loaders */
#ifdef SIMAGE_GDIPLUS_SUPPORT
#include <simage_gdiplus.h>
#endif /* SIMAGE_GDIPLUS_SUPPORT */
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
#endif /* SIMAGE_RGB_SUPPORT */
#ifdef HAVE_GIFLIB
#include <simage_gif.h>
static saver_data gif_saver;
#endif /* HAVE_GIFLIB */
#ifdef SIMAGE_EPS_SUPPORT
#include <simage_eps.h>
static saver_data eps_saver;
#endif /* SIMAGE_EPS_SUPPORT */
#ifdef SIMAGE_QIMAGE_SUPPORT
#include <simage_qimage.h>
#endif /* SIMAGE_QIMAGE_SUPPORT */
#ifdef SIMAGE_QUICKTIME_SUPPORT
#include <simage_quicktime.h>
#endif /* SIMAGE_QUICKTIME_SUPPORT */
#ifdef SIMAGE_CGIMAGE_SUPPORT
#include <simage_cgimage.h>
#endif /* SIMAGE_CGIMAGE */

#include <assert.h>

static saver_data * first_saver = NULL;
static saver_data * last_saver = NULL;

static char *
safe_strdup(const char * str)
{
  char * newstr = NULL;
  if (str != NULL) {
    newstr = (char *) malloc(strlen(str) + 1);
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
 * use this instead of strcasecmp, which doesn't exist on Win32
 * FIXME: replace with a configure test? pederb, 2001-02-07
 */
static int
simage_strcasecmp(const char * str1, const char * str2)
{
  if (str1 == NULL && str2 != NULL) return 1;
  if (str1 != NULL && str2 == NULL) return -1;
  while (*str1 && *str2) {
    int tst = tolower(*str1) - tolower(*str2);
    if (tst) return tst;
    str1++;
    str2++;
  }
  return *str1 - *str2;
}

/*
 * internal functions which adds a saver to the list of savers
 * returns a void pointer to the saver. addbefore specifies
 * if the saver should be added at the beginning or at the
 * end of the linked list. useful if a user program finds
 * a bug in this library (simply use addbefore)
 *
 * Note: The actual significance of is_internal is that memory is
 * allocated for the "non internal" loaders (currently QImage and
 * QuickTime), while the other (internal) ones are static structs. The
 * is_internal flag is used to determine whether the memory for the
 * loader should be freed at cleanup.
 */

static void
add_saver_data(saver_data * saver,
	       int (*error_func)(char *, int),
	       const char * extensions,
	       const char * fullname,
	       const char * description,
	       int is_internal,
	       int addbefore)
{
  saver->extensions = is_internal ? (char*) extensions : safe_strdup(extensions);
  saver->fullname = is_internal ? (char*) fullname : safe_strdup(fullname);
  saver->description = is_internal ? (char*) description : safe_strdup(description);
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
}


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
  saver->save_func_ext = NULL;
  add_saver_data(saver, error_func, extensions, fullname,
		 description, is_internal, addbefore);
  return saver;
}

static void *
add_saver_ext(saver_data * saver,
	      int (*save_func)(const char *,
			       const unsigned char *,
			       int, int, int, const char *),
	      int (*error_func)(char *, int),
	      const char * extensions,
	      const char * fullname,
	      const char * description,
	      int is_internal,
	      int addbefore)
{
  assert(saver);
  saver->save_func = NULL;
  saver->save_func_ext = save_func;
  add_saver_data(saver, error_func, extensions, fullname,
		 description, is_internal, addbefore);
  return saver;
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
    char * str;
    char * ext = saver->extensions;
    str = strchr(ext, ',');

    while (str) {
      int cmp;
      /* modify string while comparing. string is a copy so it should be safe */
      *str = 0;
      cmp = simage_strcasecmp(ext, filenameextension);
      *str = ',';
      if (cmp == 0) return saver;
      ext = str + 1;
      str = strchr(ext, ',');
    }
    if (!simage_strcasecmp(ext, filenameextension)) return saver;
    saver = saver->next;
  }
  return NULL;
}

/* don't const the ext-strings, they are modified in find_saver */
static char jpegext[] = "jpg,jpeg";
static const char jpegfull[] = "The Independent JPEG Group file format";
static char pngext[] = "png";
static const char pngfull[] = "The PNG file format";
static char tiffext[] = "tiff,tif";
static const char tifffull[] = "The Tag Image File Format";
static char rgbext[] = "rgb,rgba,bw,inta,int";
static const char rgbfull[] ="The SGI RGB file format";
static char gifext[] = "gif";
static const char giffull[] = "The Graphics Interchange Format";
static char epsext[] = "eps,ps";
static const char epsfull[] ="Encapsulated postscript";

static void
str_tolower(char * str)
{
  while (*str) {
    *str = tolower(*str);
    str++;
  }
}

static void
add_internal_savers(void)
{
  static int first = 1;
  if (first) {
#if defined(SIMAGE_GDIPLUS_SUPPORT) || defined(SIMAGE_QIMAGE_SUPPORT) || \
    defined(SIMAGE_QUICKTIME_SUPPORT) || defined(SIMAGE_CGIMAGE_SUPPORT)
    char * qtext = NULL;
#endif
    first = 0;

#ifdef HAVE_JPEGLIB
    add_saver(&jpeg_saver,
	      simage_jpeg_save,
	      simage_jpeg_error,
	      jpegext,
	      jpegfull,
	      NULL,
	      1, 0);
#endif /* HAVE_JPEGLIB */
#ifdef HAVE_PNGLIB
    add_saver(&png_saver,
	      simage_png_save,
	      simage_png_error,
	      pngext,
	      pngfull,
	      NULL,
	      1, 0);
#endif /* HAVE_PNGLIB */
#ifdef HAVE_TIFFLIB
    add_saver(&tiff_saver,
	      simage_tiff_save,
	      simage_tiff_error,
	      tiffext,
	      tifffull,
	      NULL,
	      1, 0);
#endif /* HAVE_TIFFLIB */
#ifdef SIMAGE_RGB_SUPPORT
    add_saver(&rgb_saver,
	      simage_rgb_save,
	      simage_rgb_error,
	      rgbext,
	      rgbfull,
	      NULL,
	      1, 0);
#endif /* SIMAGE_RGB_SUPPORT */
#ifdef HAVE_GIFLIB
    add_saver(&gif_saver,
	      simage_gif_save,
	      simage_gif_error,
	      gifext,
	      giffull,
	      NULL,
	      1, 0);
#endif /* HAVE_GIFLIB */

#ifdef SIMAGE_EPS_SUPPORT
    add_saver(&eps_saver,
	      simage_eps_save,
	      simage_eps_error,
	      epsext,
	      epsfull,
	      NULL,
	      1, 0);
#endif /* SIMAGE_EPS_SUPPORT */

#ifdef SIMAGE_GDIPLUS_SUPPORT
    qtext = simage_gdiplus_get_savers();
    if (qtext) {
      saver_data * saver;
      char * str;
      char * ext = qtext;
      do {
	str = strchr(ext, ',');
	if (str) *str = 0;
	str_tolower(ext);
	saver = (saver_data*) malloc(sizeof(saver_data));
	add_saver_ext(saver,
		      simage_gdiplus_save,
		      simage_gdiplus_error,
		      ext,
		      "GDI+ saver",
		      NULL,
		      0, 0);

	if (str) ext = str + 1;
      } while (str);
      free(qtext);
    }
#endif /* SIMAGE_GDIPLUS_SUPPORT */
#ifdef SIMAGE_QIMAGE_SUPPORT
    qtext = simage_qimage_get_savers();
    if (qtext) {
      saver_data * saver;
      char * str;
      char * ext = qtext;
      do {
	str = strchr(ext, ',');
	if (str) *str = 0;
	str_tolower(ext);
	saver = (saver_data*) malloc(sizeof(saver_data));
	add_saver_ext(saver,
		      simage_qimage_save,
		      simage_qimage_error,
		      ext,
		      "QImage saver",
		      NULL,
		      0, 0);

	if (str) ext = str + 1;
      } while (str);
      free(qtext);
    }
#endif /* SIMAGE_QIMAGE_SUPPORT */
#ifdef SIMAGE_QUICKTIME_SUPPORT
    qtext = simage_quicktime_get_savers();
    if (qtext) {
      saver_data * saver;
      char * str;
      char * ext = qtext;
      do {
	str = strchr(ext, ',');
	if (str) *str = 0;
	str_tolower(ext);
	saver = (saver_data*) malloc(sizeof(saver_data));
	add_saver_ext(saver,
		      simage_quicktime_save,
		      simage_quicktime_error,
		      ext,
		      "QuickTime saver",
		      NULL,
		      0, 1);
	if (str) ext = str + 1;
      } while (str);

      free(qtext);
    }
#endif /* SIMAGE_QUICKTIME_SUPPORT */
#ifdef SIMAGE_CGIMAGE_SUPPORT
    qtext = simage_cgimage_get_savers();
    if (qtext) {
      saver_data * saver;
      char * str;
      char * ext = qtext;
      do {
	str = strchr(ext, ',');
	if (str) *str = 0;
	str_tolower(ext);
	saver = (saver_data*) malloc(sizeof(saver_data));
	add_saver_ext(saver,
		      simage_cgimage_save,
		      simage_cgimage_error,
		      ext,
		      "CGImage saver",
		      NULL,
		      0, 1);
	if (str) ext = str + 1;
      } while (str);

      free(qtext);
    }
#endif /* SIMAGE_CGIMAGE_SUPPORT */
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
    int ret = 0;
    if (saver->save_func_ext) {
      ret = saver->save_func_ext(filename, bytes, width,
				 height, numcomponents,
				 filenameextension);
    }
    else if (saver->save_func) {
      ret = saver->save_func(filename, bytes, width,
			     height, numcomponents);
    }
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
    if (!saver->is_internal) {
      safe_strfree(saver->extensions);
      safe_strfree(saver->fullname);
      safe_strfree(saver->description);
      free(saver);
    }
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
  int cnt = 0;
  saver_data * saver;

  /* FIXME: it's ugly design that we have to call this method on all
     public API functions for initialization. 20020215 mortene. */
  add_internal_savers();

  saver = first_saver; /* must be set after add_internal_savers(), obviously */
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
