/* *************************************************************************
 * guilesimage.c
 *
 * Written by Lars J. Aas <larsa@coin3d.org>.
 * This file is in the Public Domain.
 *
 * $Revision$
 */

/* *************************************************************************
 * TODO:
 * - do proper type checking, range checking, and throw proper errors for
 *   each hook
 * - ensure the componet order in the pixel is correct.
 * - implement simage-pixel functions
 */

#include <guilesimage.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <guile/gh.h>
#include <libguile.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#else
#error need dlopen() support
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* ********************************************************************** */
/* prototypes from simage.h */

static void (*simage_version)(int *, int *, int *);
static int (*simage_check_supported)(char *);
static const char * (*simage_get_last_error)(void);
static int (*simage_check_save_supported)(char *);
static unsigned char * (*simage_read_image)( const char *, int *, int *, int *);
static int (*simage_save_image)( const char *, const unsigned char *, int, int, int, const char * );
static unsigned char * (*simage_resize)( unsigned char *, int, int, int, int, int );
static void (*simage_free_image)(unsigned char * imagedata);

/* ********************************************************************** */
/* guile hooks */

/*/ (simage-version-major)
 *
 */

static SCM
scimage_version_major(
  void )
{
  int major = 0, minor, micro;
  simage_version( &major, &minor, &micro );
  return gh_int2scm( major );
} /* scimage_version_major() */

/*/ (simage-version-minor)
 *
 */

static SCM
scimage_version_minor(
  void )
{
  int major, minor = 0, micro;
  simage_version( &major, &minor, &micro );
  return gh_int2scm( minor );
} /* scimage_version_minor() */

/*/ (simage-version-micro)
 *
 */

static SCM
scimage_version_micro(
  void )
{
  int major, minor, micro = 0;
  simage_version( &major, &minor, &micro );
  return gh_int2scm( micro );
} /* scimage_version_micro() */

/*/ (simage-get-last-error)
 *
 */

static SCM
scimage_get_last_error(
  void )
{
  const char * last_error = simage_get_last_error();
  if ( last_error && last_error[0] != 0 )
    return gh_str02scm( simage_get_last_error() );
  return SCM_UNSPECIFIED; /* or '()? */
} /* scimage_get_last_error() */

/* ********************************************************************** */
/* the "simage-image" smob */

static long simage_image_smob_type = 0;

struct simage_image {
  unsigned char * data;
  int width;
  int height;
  int components;
};

static SCM
scimage_image_mark(
  SCM scm_simage_image )
{
  return SCM_BOOL_F;
} /* scimage_image_mark() */

static scm_sizet
scimage_image_free(
  SCM scm_simage_image )
{
  struct simage_image * simage_image_data;
  scm_sizet size;
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  size = simage_image_data->width *
      simage_image_data->height *
      simage_image_data->components +
      sizeof( struct simage_image );
  simage_free_image( simage_image_data->data );
  free( simage_image_data );
  return size;
} /* scimage_image_free() */

static int
scimage_image_print(
  SCM scm_simage_image,
  SCM port,
  scm_print_state * print_state )
{
  char dimensions[16];
  struct simage_image * simage_image_data;
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  sprintf( dimensions, "%dx%dx%d", simage_image_data->width,
    simage_image_data->height, simage_image_data->components );
  scm_puts( "#<simage-image ", port );
  scm_puts( dimensions, port );
  scm_puts( ">", port );
  return 1;
} /* scimage_image_print() */

/*/ (make-simage-image width height [components = 4])
 *
 */

static SCM
scimage_make_image(
  SCM scm_width,
  SCM scm_height,
  SCM scm_components )
{
  struct simage_image * simage_image_data;
  int width, height, components = 4;
  simage_image_data = (struct simage_image *)
      scm_must_malloc( sizeof( struct simage_image ), "simage-image" );
  width = gh_scm2int( scm_width );
  height = gh_scm2int( scm_height );
  if ( scm_components != SCM_UNDEFINED )
    components = gh_scm2int( scm_components );
  /* FIXME: assert components are 3 or 4 (rather use bits?) */
  /* FIXME: need to flag this to avoid using simage_free_image() on buffer */
  simage_image_data->data = (unsigned char *)
      scm_must_malloc( width * height * components, "simage-pixel" );
  memset( simage_image_data->data, 0, width * height * components );
  simage_image_data->width = width;
  simage_image_data->height = height;
  simage_image_data->components = components;
  SCM_RETURN_NEWSMOB( simage_image_smob_type, simage_image_data );
} /* scimage_make_image() */

/*/ (simage-image-width image)
 *
 */

static SCM
scimage_image_width(
  SCM scm_simage_image )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-image-width" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  return gh_int2scm( simage_image_data->width );
} /* scimage_image_width() */

/*/ (simage-image-height image)
 *
 */

static SCM
scimage_image_height(
  SCM scm_simage_image )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-image-height" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  return gh_int2scm( simage_image_data->height );
} /* scimage_image_height() */

/*/ (simage-image-components)
 *
 */

static SCM
scimage_image_components(
  SCM scm_simage_image )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-image-components" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  return gh_int2scm( simage_image_data->components );
} /* scimage_image_components() */

/* ********************************************************************** */
/* the "simage-pixel" smob */

static long simage_pixel_smob_type = 0;

static SCM
scimage_pixel_mark(
  SCM simage_pixel_smob )
{
  return SCM_BOOL_F;
} /* scimage_pixel_mark() */

static scm_sizet
scimage_pixel_free(
  SCM simage_pixel_smob )
{
  return 0;
} /* scimage_pixel_free() */

static int
scimage_pixel_print(
  SCM simage_pixel_smob,
  SCM port,
  scm_print_state * print_state )
{
  char value[12];
  unsigned long pixel;
  pixel = (unsigned long) SCM_SMOB_DATA( simage_pixel_smob );
  sprintf( value, "0x%02x%02x%02x%02x", (pixel >> 24) & 0xff,
    (pixel >> 16) & 0xff, (pixel >> 8) & 0xff, pixel & 0xff );
  scm_puts( "#<simage-pixel ", port );
  scm_puts( value, port );
  scm_puts( ">", port );
  return 1;
} /* scimage_pixel_print() */

/*/ (make-simage-pixel red green blue [alpha = 255])
 *
 */

static SCM
scimage_make_pixel(
  SCM scm_red,
  SCM scm_green,
  SCM scm_blue,
  SCM scm_alpha )
{
  unsigned long pixel = 0;
  unsigned int red, green, blue, alpha = 255;
  red = gh_scm2int( scm_red );
  green = gh_scm2int( scm_green );
  blue = gh_scm2int( scm_blue );
  if ( scm_alpha != SCM_UNDEFINED )
    alpha = gh_scm2int( scm_alpha );
  pixel = (red << 24) | (green << 16) | (blue << 8) | alpha;
  SCM_RETURN_NEWSMOB( simage_pixel_smob_type, pixel );
} /* scimage_make_pixel() */

/*/ (simage-pixel-get-red simage-pixel)
 *
 */

static SCM
scimage_pixel_get_red(
  SCM simage_pixel_smob )
{
  unsigned long pixel;
  pixel = (unsigned long) SCM_SMOB_DATA( simage_pixel_smob );
  return gh_int2scm( (pixel >> 24) & 0xff );
} /* scimage_pixel_get_red() */

/*/ (simage-pixel-get-green simage-pixel)
 *
 */

static SCM
scimage_pixel_get_green(
  SCM simage_pixel_smob )
{
  unsigned long pixel;
  pixel = (unsigned long) SCM_SMOB_DATA( simage_pixel_smob );
  return gh_int2scm( (pixel >> 16) & 0xff );
} /* scimage_pixel_get_green() */

/*/ (simage-pixel-get-blue simage-pixel)
 *
 */

static SCM
scimage_pixel_get_blue(
  SCM simage_pixel_smob )
{
  unsigned long pixel;
  pixel = (unsigned long) SCM_SMOB_DATA( simage_pixel_smob );
  return gh_int2scm( (pixel >> 8) & 0xff );
} /* scimage_pixel_get_blue() */

/*/ (simage-pixel-get-alpha simage-pixel)
 *
 */

static SCM
scimage_pixel_get_alpha(
  SCM simage_pixel_smob )
{
  unsigned long pixel;
  pixel = (unsigned long) SCM_SMOB_DATA( simage_pixel_smob );
  return gh_int2scm( pixel & 0xff );
} /* scimage_pixel_get_alpha() */

/* ********************************************************************** */

/*/ (simage-load-supported? filename)
 *
 */

static SCM
scimage_load_supported_p(
  SCM scm_filename )
{
  char * filename;
  int result;
  filename = gh_scm2newstr( scm_filename, NULL );
  result = simage_check_supported( filename );
  free( filename );
  return result ? SCM_BOOL_T : SCM_BOOL_F;
} /* scimage_load_supported_p() */

/*/ (simage-load filename)
 *
 */

static SCM
scimage_load(
  SCM scm_filename )
{
  char * filename;
  unsigned char * data;
  int width, height, components;
  struct simage_image * simage_image_data;
  filename = gh_scm2newstr( scm_filename, NULL );
  data = simage_read_image( filename, &width, &height, &components );
  if ( ! data ) return SCM_UNSPECIFIED;
  /* tell gc */
  scm_done_malloc(width * height * components);
  
  simage_image_data = (struct simage_image *)
      scm_must_malloc( sizeof( struct simage_image ), "simage-image" );
  simage_image_data->data = data;
  simage_image_data->width = width;
  simage_image_data->height = height;
  simage_image_data->components = components;
  SCM_RETURN_NEWSMOB( simage_image_smob_type, simage_image_data );
} /* scimage_load() */

/*/ (simage-save-supported? extension)
 *
 */

static SCM
scimage_save_supported_p(
  SCM scm_extension )
{
  char * extension;
  int result;
  extension = gh_scm2newstr( scm_extension, NULL );
  result = simage_check_save_supported( extension );
  free( extension );
  return result ? SCM_BOOL_T : SCM_BOOL_F;
} /* scimage_save_supported_p() */

/*/ (simage-save image filename extension)
 *
 */

static SCM
scimage_save(
  SCM scm_simage_image,
  SCM scm_filename,
  SCM scm_extension )
{
  char * filename, * extension;
  struct simage_image * simage_image_data;
  int result;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-save" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  filename = gh_scm2newstr( scm_filename, NULL );
  extension = gh_scm2newstr( scm_extension, NULL );
  result = simage_save_image( filename, simage_image_data->data,
    simage_image_data->width, simage_image_data->height,
    simage_image_data->components, extension );
  free( filename );
  free( extension );
  return result ? SCM_BOOL_T : SCM_BOOL_F;
} /* scimage_save() */

/*/ (simage-resize image width height)
 *
 */

static SCM
scimage_resize(
  SCM scm_simage_image,
  SCM scm_width,
  SCM scm_height )
{
  struct simage_image * simage_image_data;
  int new_width, new_height, new_components;
  unsigned char * new_data;

  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-resize" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  new_width = gh_scm2int( scm_width );
  new_height = gh_scm2int( scm_height );
  new_components = simage_image_data->components;
  
  new_data = simage_resize( simage_image_data->data,
    simage_image_data->width, simage_image_data->height,
    simage_image_data->components, new_width, new_height );

  if ( ! new_data ) return SCM_UNSPECIFIED;
  /* tell gc */
  scm_done_malloc(new_width * new_height * new_components);

  simage_image_data = (struct simage_image *)
      scm_must_malloc( sizeof( struct simage_image ), "simage-image" );
  simage_image_data->data = new_data;
  simage_image_data->width = new_width;
  simage_image_data->height = new_height;
  simage_image_data->components = new_components;
  SCM_RETURN_NEWSMOB( simage_image_smob_type, simage_image_data );
} /* scimage_resize() */

/* ********************************************************************** */
/* image<->pixel functions */

/*/ (simage-image-get-pixel simage-image x y)
 *
 */

static SCM
scimage_image_get_pixel(
  SCM scm_simage_image,
  SCM scm_x,
  SCM scm_y )
{
  struct simage_image * simage_image_data;
  int x, y;
  unsigned char * pos;
  unsigned long pixel;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-image-get-pixel" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  x = gh_scm2int( scm_x );
  y = gh_scm2int( scm_y );
  if ( x < 0 || x >= simage_image_data->width )
    return SCM_BOOL_F;
  if ( y < 0 || y >= simage_image_data->height )
    return SCM_BOOL_F;
  pos = simage_image_data->data +
    (simage_image_data->width * y + x) * simage_image_data->components;
  pixel = pos[0] << 24 | pos[1] << 16 | pos[2] << 8 | 0xff;
  if ( simage_image_data->components == 4 )
    pixel &= ~((unsigned long)pos[3]);
  SCM_RETURN_NEWSMOB( simage_pixel_smob_type, pixel );
} /* scimage_image_get_pixel() */

/*/ (simage-image-set-pixel! simage-image x y pixel)
 *
 */

static SCM
scimage_image_set_pixel(
  SCM scm_simage_image,
  SCM scm_x,
  SCM scm_y,
  SCM scm_simage_pixel )
{
  struct simage_image * simage_image_data;
  int x, y;
  unsigned long pixel;
  unsigned char * pos;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, scm_simage_image ),
              scm_simage_image, SCM_ARG1, "simage-image-set-pixel!" );
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_pixel_smob_type, scm_simage_pixel ),
              scm_simage_pixel, SCM_ARG4, "simage-image-set-pixel!" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( scm_simage_image );
  x = gh_scm2int( scm_x );
  y = gh_scm2int( scm_y );
  pixel = (unsigned long) SCM_SMOB_DATA( scm_simage_pixel );
  if ( x < 0 || x >= simage_image_data->width )
    return SCM_BOOL_F;
  if ( y < 0 || y >= simage_image_data->height )
    return SCM_BOOL_F;
  pos = simage_image_data->data +
    (simage_image_data->width * y + x) * simage_image_data->components;
  pos[0] = (pixel >> 24) & 0xff;
  pos[1] = (pixel >> 16) & 0xff;
  pos[2] = (pixel >> 8) & 0xff;
  if ( simage_image_data->components == 4 )
    pos[3] = pixel & 0xff;
  return SCM_BOOL_T;
} /* scimage_image_set_pixel() */
  
/* ********************************************************************** */
/* init-function for binding the functions to the Guile environment */

void
guilesimage_init(
  void )
{
  void * simage_so;

  /* dlopen simage */
  simage_so = dlopen( "libsimage.so", RTLD_NOW /* | RTLD_GLOBAL? */ );
  simage_version = dlsym( simage_so, "simage_version" );
  simage_check_supported = dlsym( simage_so, "simage_check_supported" );
  simage_get_last_error = dlsym( simage_so, "simage_get_last_error" );
  simage_check_save_supported =
    dlsym( simage_so, "simage_check_save_supported" );
  simage_read_image = dlsym( simage_so, "simage_read_image" );
  simage_free_image = dlsym( simage_so, "simage_free_image" );
  simage_save_image = dlsym( simage_so, "simage_save_image" );
  simage_resize = dlsym( simage_so, "simage_resize" );

  /* register simage smobs */
  simage_image_smob_type =
    scm_make_smob_type( "simage-image", sizeof( struct simage_image ) );
  scm_set_smob_mark( simage_image_smob_type, scimage_image_mark );
  scm_set_smob_free( simage_image_smob_type, scimage_image_free );
  scm_set_smob_print( simage_image_smob_type, scimage_image_print );

  simage_pixel_smob_type =
    scm_make_smob_type( "simage-pixel", 0 ); /* immediate data */
  scm_set_smob_mark( simage_pixel_smob_type, scimage_pixel_mark );
  scm_set_smob_free( simage_pixel_smob_type, scimage_pixel_free );
  scm_set_smob_print( simage_pixel_smob_type, scimage_pixel_print );

  /* register guile hooks */
  scm_c_define_gsubr( "simage-version-major", 0, 0, 0, scimage_version_major );
  scm_c_define_gsubr( "simage-version-minor", 0, 0, 0, scimage_version_minor );
  scm_c_define_gsubr( "simage-version-micro", 0, 0, 0, scimage_version_micro );
  scm_c_define_gsubr( "simage-get-last-error", 0, 0, 0, scimage_get_last_error );

  scm_c_define_gsubr( "make-simage-image", 2, 1, 0, scimage_make_image );
  scm_c_define_gsubr( "simage-image-width", 1, 0, 0, scimage_image_width );
  scm_c_define_gsubr( "simage-image-height", 1, 0, 0, scimage_image_height );
  scm_c_define_gsubr( "simage-image-components", 1, 0, 0,scimage_image_components );

  scm_c_define_gsubr( "simage-load-supported?", 1, 0, 0, scimage_load_supported_p );
  scm_c_define_gsubr( "simage-load", 1, 0, 0, scimage_load );
  scm_c_define_gsubr( "simage-save-supported?", 1, 0, 0, scimage_save_supported_p );
  scm_c_define_gsubr( "simage-save", 3, 0, 0, scimage_save );
  scm_c_define_gsubr( "simage-resize", 3, 0, 0, scimage_resize );

  scm_c_define_gsubr( "make-simage-pixel", 3, 1, 0, scimage_make_pixel );
  scm_c_define_gsubr( "simage-pixel-get-red", 1, 0, 0, scimage_pixel_get_red );
  scm_c_define_gsubr( "simage-pixel-get-green", 1, 0, 0, scimage_pixel_get_green );
  scm_c_define_gsubr( "simage-pixel-get-blue", 1, 0, 0, scimage_pixel_get_blue );
  scm_c_define_gsubr( "simage-pixel-get-alpha", 1, 0, 0, scimage_pixel_get_alpha );

  scm_c_define_gsubr( "simage-image-get-pixel", 3, 0, 0, scimage_image_get_pixel );
  scm_c_define_gsubr( "simage-image-set-pixel!", 4, 0, 0, scimage_image_set_pixel );
} /* guilesimage_init() */

/* EOF ****************************************************************** */
