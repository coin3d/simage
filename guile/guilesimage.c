/* *************************************************************************
 * guilesimage.c
 * Written by Lars J. Aas <larsa@coin3d.org>.
 * This file is in the Public Domain.
 */

#include <guilesimage.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <guile/gh.h>
#include <libguile.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
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
  SCM simage_image_smob )
{
  return SCM_BOOL_F;
} /* scimage_image_mark() */

static scm_sizet
scimage_image_free(
  SCM simage_image_smob )
{
  struct simage_image * simage_image_data;
  scm_sizet size;
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  size = simage_image_data->width * simage_image_data->height * 4
    + sizeof( struct simage_image );
  simage_free_image( simage_image_data->data );
  free( simage_image_data );
  return size;
} /* scimage_image_free() */

static int
scimage_image_print(
  SCM simage_image_smob,
  SCM port,
  scm_print_state * print_state )
{
  char dimensions[16];
  struct simage_image * simage_image_data;
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  sprintf( dimensions, "%dx%dx%d", simage_image_data->width,
    simage_image_data->height, simage_image_data->components );
  scm_puts( "#<simage-image ", port );
  scm_puts( dimensions, port );
  scm_puts( ">", port );
  return 1;
} /* scimage_image_print() */

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
  if ( last_error )
    return gh_str02scm( simage_get_last_error() );
  return SCM_UNSPECIFIED;
} /* scimage_get_last_error() */

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
  simage_image_data =
    (struct simage_image *) malloc( sizeof( struct simage_image ) );
  simage_image_data->data = data;
  simage_image_data->width = width;
  simage_image_data->height = height;
  simage_image_data->components = components;
  SCM_RETURN_NEWSMOB( simage_image_smob_type, simage_image_data );
} /* scimage_load() */

/*/ (simage-resize image width height)
 *
 */

static SCM
scimage_resize(
  SCM simage_image_smob,
  SCM scm_width,
  SCM scm_height )
{
  struct simage_image * simage_image_data;
  int new_width, new_height, new_components;
  unsigned char * new_data;

  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, simage_image_smob ),
              simage_image_smob, SCM_ARG1, "simage-image-width" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  new_width = gh_scm2int( scm_width );
  new_height = gh_scm2int( scm_height );
  new_components = simage_image_data->components;
  
  new_data = simage_resize( simage_image_data->data,
    simage_image_data->width, simage_image_data->height,
    simage_image_data->components, new_width, new_height );

  if ( ! new_data ) return SCM_UNSPECIFIED;
  simage_image_data =
    (struct simage_image *) malloc( sizeof( struct simage_image ) );
  simage_image_data->data = new_data;
  simage_image_data->width = new_width;
  simage_image_data->height = new_height;
  simage_image_data->components = new_components;
  SCM_RETURN_NEWSMOB( simage_image_smob_type, simage_image_data );
} /* scimage_resize() */

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
  SCM simage_image_smob,
  SCM scm_filename,
  SCM scm_extension )
{
  char * filename, * extension;
  struct simage_image * simage_image_data;
  int result;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, simage_image_smob ),
              simage_image_smob, SCM_ARG1, "simage-image-width" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  filename = gh_scm2newstr( scm_filename, NULL );
  extension = gh_scm2newstr( scm_extension, NULL );
  result = simage_save_image( filename, simage_image_data->data,
    simage_image_data->width, simage_image_data->height,
    simage_image_data->components, extension );
  free( filename );
  free( extension );
  return result ? SCM_BOOL_T : SCM_BOOL_F;
} /* scimage_save() */

/* ********************************************************************** */
/* guile hooks operating on the simage-image struct */

/*/ (simage-image-width image)
 *
 */

static SCM
scimage_image_width(
  SCM simage_image_smob )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, simage_image_smob ),
              simage_image_smob, SCM_ARG1, "simage-image-width" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  return gh_int2scm( simage_image_data->width );
} /* scimage_image_width() */

/*/ (simage-image-height image)
 *
 */

static SCM
scimage_image_height(
  SCM simage_image_smob )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, simage_image_smob ),
              simage_image_smob, SCM_ARG1, "simage-image-height" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  return gh_int2scm( simage_image_data->height );
} /* scimage_image_height() */

/*/ (simage-image-components)
 *
 */

static SCM
scimage_image_components(
  SCM simage_image_smob )
{
  struct simage_image * simage_image_data;
  SCM_ASSERT( SCM_SMOB_PREDICATE( simage_image_smob_type, simage_image_smob ),
              simage_image_smob, SCM_ARG1, "simage-image-components" );
  simage_image_data =
    (struct simage_image *) SCM_SMOB_DATA( simage_image_smob );
  return gh_int2scm( simage_image_data->components );
} /* scimage_image_components() */

/* ********************************************************************** */
/* init-function for binding the functions to the Guile environment */

/*/
 *
 */

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

  /* register guile functions */
  scm_make_gsubr( "simage-version-major", 0, 0, 0, scimage_version_major );
  scm_make_gsubr( "simage-version-minor", 0, 0, 0, scimage_version_minor );
  scm_make_gsubr( "simage-version-micro", 0, 0, 0, scimage_version_micro );
  scm_make_gsubr( "simage-get-last-error", 0, 0, 0, scimage_get_last_error );

  scm_make_gsubr( "simage-load-supported?", 1, 0, 0, scimage_load_supported_p );
  scm_make_gsubr( "simage-load", 1, 0, 0, scimage_load );
  scm_make_gsubr( "simage-resize", 3, 0, 0, scimage_resize );
  scm_make_gsubr( "simage-save-supported?", 1, 0, 0, scimage_save_supported_p );
  scm_make_gsubr( "simage-save", 3, 0, 0, scimage_save );

  scm_make_gsubr( "simage-image-width", 1, 0, 0, scimage_image_width );
  scm_make_gsubr( "simage-image-height", 1, 0, 0, scimage_image_height );
  scm_make_gsubr( "simage-image-components", 1, 0, 0,scimage_image_components );
} /* guilesimage_init() */

/* EOF ****************************************************************** */
