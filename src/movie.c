/*
  Note 20020923 thammer:
  movie.c should probably be rewritten to use stream.c. 
  If you are considering modifying movie.c, please discuss it
  with coin@sim.no first.

 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <simage_private.h>
#include <string.h>

#ifdef SIMAGE_MPEG2ENC_SUPPORT
#include "../mpeg2enc/api.h"
#endif /* SIMAGE_MPEG2ENC_SUPPORT */

#ifdef SIMAGE_AVIENC_SUPPORT
#include "simage_avi.h"
#endif /* SIMAGE_AVIENC_SUPPORT */

struct simage_movie_s {
  char * filename;
  
  s_movie_open_func * open;
  s_movie_create_func * create;
  s_movie_get_func * get;
  s_movie_put_func * put;
  s_movie_close_func * close;
  
  s_params * params;
};

struct simage_movie_importer {
  s_movie_open_func * open;
  s_movie_get_func * get;
  s_movie_close_func * close;  

  struct simage_movie_importer * next;
};

struct simage_movie_exporter {
  s_movie_create_func * create;
  s_movie_put_func * put;
  s_movie_close_func * close;  
  
  struct simage_movie_exporter * next;
};

// FIXME: convert access to these variables into the singleton
// pattern. 20010917 mortene.
static struct simage_movie_importer * importers;
static struct simage_movie_exporter * exporters;

static void 
add_internal_importers(void)
{
  static int first = 1;
  if (first) {
    /* none yet */
    first = 0;
  }                
}

static void
add_internal_exporters(void)
{
  static int first = 1;
  if (first) {    
#ifdef SIMAGE_MPEG2ENC_SUPPORT
    s_movie_exporter_add(mpeg2enc_movie_create,
                         mpeg2enc_movie_put,
                         mpeg2enc_movie_close);
#endif
#ifdef SIMAGE_AVIENC_SUPPORT
    s_movie_exporter_add(avienc_movie_create,
                         avienc_movie_put,
                         avienc_movie_close);
#endif
   first = 0;
  }
}

s_movie * 
s_movie_open(const char * filename)
{
  struct simage_movie_importer * imp;
  s_movie * movie = (s_movie*) malloc(sizeof(s_movie));
  movie->params = NULL;
  movie->filename = NULL;

  add_internal_importers();

  imp = importers;
  while (imp) {
    if (imp->open(filename, movie)) break;
    imp = imp->next;
  }
  if (imp == NULL) {
    free((void*)movie);
    return NULL;
  }

  movie->filename = (char*) malloc(strlen(filename)+1);
  strcpy(movie->filename, filename);
  movie->open = imp->open;
  movie->get = imp->get;
  movie->close = imp->close;

  return movie;
}

s_movie * 
s_movie_create(const char * filename, s_params * params /* | NULL */)
{
  struct simage_movie_exporter * exp;
  s_movie * movie = (s_movie*) malloc(sizeof(s_movie));
  movie->params = NULL;
  movie->filename = NULL;
  
  add_internal_exporters();

  exp = exporters;
  while (exp) {
    if (exp->create(filename, movie, params)) break;
    exp = exp->next;
  }
  if (exp == NULL) {
    free((void*) movie);
    return NULL;
  }

  movie->filename = (char*) malloc(strlen(filename)+1);
  movie->create = exp->create;
  movie->put = exp->put;
  movie->close = exp->close;
  strcpy(movie->filename, filename);
  return movie;
}

s_image *
s_movie_get_image(s_movie * movie, s_image * prealloc, s_params * params)
{
  return movie->get(movie, prealloc, params);
}

int 
s_movie_put_image(s_movie * movie, s_image * image,
                  s_params * params)
{
  return movie->put(movie, image, params);
}

void 
s_movie_close(s_movie * movie)
{
  movie->close(movie);
}

void 
s_movie_destroy(s_movie * movie)
{
  if (movie->params) s_params_destroy(movie->params);
  if (movie->filename) free((void*) movie->filename);
  free((void*) movie);
}

s_params * 
s_movie_params(s_movie * movie)
{
  if (movie->params == NULL) {
    movie->params = s_params_create();
  }
  return movie->params;
}

void 
s_movie_importer_add(s_movie_open_func * open,
                     s_movie_get_func * get,
                     s_movie_close_func * close)
{
  struct simage_movie_importer * last, * imp = importers;
  last = NULL;
  while (imp) {
    last = imp;
    imp = imp->next;
  }
  imp = (struct simage_movie_importer*) malloc(sizeof(struct simage_movie_importer));
  imp->open = open;
  imp->get = get;
  imp->close = close;
  imp->next = NULL;

  if (last == NULL) {
    importers = imp;
  }
  else last->next = imp;
}

void 
s_movie_exporter_add(s_movie_create_func * create,
                     s_movie_put_func * put,
                     s_movie_close_func * close)
{
  struct simage_movie_exporter * last, * exp = exporters;
  last = NULL;
  while (exp) {
    last = exp;
    exp = exp->next;
  }
  exp = (struct simage_movie_exporter*) malloc(sizeof(struct simage_movie_exporter));
  exp->create = create;
  exp->put = put;
  exp->close = close;
  exp->next = NULL;

  if (last == NULL) {
    exporters = exp;
  }
  else last->next = exp;
}
