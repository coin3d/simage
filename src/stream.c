#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <simage_private.h>
#include <string.h>

#ifdef SIMAGE_OGGVORBIS_READ_SUPPORT
#include "simage_oggvorbis_read.h"
#endif /* SIMAGE_OGGVORBIS_READ_SUPPORT */

struct simage_stream_s {
  char * filename;
  
  s_stream_open_func * open;
  s_stream_create_func * create;
  s_stream_get_func * get;
  s_stream_put_func * put;
  s_stream_close_func * close;
  
  s_params * params;
};

struct simage_stream_importer {
  s_stream_open_func * open;
  s_stream_get_func * get;
  s_stream_close_func * close;  

  struct simage_stream_importer * next;
};

struct simage_stream_exporter {
  s_stream_create_func * create;
  s_stream_put_func * put;
  s_stream_close_func * close;  
  
  struct simage_stream_exporter * next;
};

// FIXME: convert access to these variables into the singleton
// pattern. 20010917 mortene.
static struct simage_stream_importer * importers;
static struct simage_stream_exporter * exporters;

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
#ifdef SIMAGE_OGGVORBIS_READ_SUPPORT
    s_stream_exporter_add(oggvorbis_reader_stream_create,
                          oggvorbis_reader_stream_put,
                          oggvorbis_reader_stream_close);
#endif
   first = 0;
  }
}

s_stream * 
s_stream_open(const char * filename)
{
  struct simage_stream_importer * imp;
  s_stream * stream = (s_stream*) malloc(sizeof(s_stream));
  stream->params = NULL;
  stream->filename = NULL;

  add_internal_importers();

  imp = importers;
  while (imp) {
    if (imp->open(filename, stream)) break;
    imp = imp->next;
  }
  if (imp == NULL) {
    free((void*)stream);
    return NULL;
  }

  stream->filename = (char*) malloc(strlen(filename)+1);
  strcpy(stream->filename, filename);
  stream->open = imp->open;
  stream->get = imp->get;
  stream->close = imp->close;

  return stream;
}

s_stream * 
s_stream_create(const char * filename, s_params * params /* | NULL */)
{
  struct simage_stream_exporter * exp;
  s_stream * stream = (s_stream*) malloc(sizeof(s_stream));
  stream->params = NULL;
  stream->filename = NULL;
  
  add_internal_exporters();

  exp = exporters;
  while (exp) {
    if (exp->create(filename, stream, params)) break;
    exp = exp->next;
  }
  if (exp == NULL) {
    free((void*) stream);
    return NULL;
  }

  stream->filename = (char*) malloc(strlen(filename)+1);
  stream->create = exp->create;
  stream->put = exp->put;
  stream->close = exp->close;
  strcpy(stream->filename, filename);
  return stream;
}

void * 
s_stream_get_buffer(s_stream * stream, void * prealloc,
                           int *size, s_params * params)
{
  return stream->get(stream, prealloc, size, params);
}

int 
s_stream_put_buffer(s_stream * stream, void * buffer, 
                        int size, s_params * params)
{
  return stream->put(stream, buffer, size, params);
}

void 
s_stream_close(s_stream * stream)
{
  stream->close(stream);
}

void 
s_stream_destroy(s_stream * stream)
{
  if (stream->params) s_params_destroy(stream->params);
  if (stream->filename) free((void*) stream->filename);
  free((void*) stream);
}

s_params * 
s_stream_params(s_stream * stream)
{
  if (stream->params == NULL) {
    stream->params = s_params_create();
  }
  return stream->params;
}

void 
s_stream_importer_add(s_stream_open_func * open,
                     s_stream_get_func * get,
                     s_stream_close_func * close)
{
  struct simage_stream_importer * last, * imp = importers;
  last = NULL;
  while (imp) {
    last = imp;
    imp = imp->next;
  }
  imp = (struct simage_stream_importer*) malloc(sizeof(struct simage_stream_importer));
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
s_stream_exporter_add(s_stream_create_func * create,
                     s_stream_put_func * put,
                     s_stream_close_func * close)
{
  struct simage_stream_exporter * last, * exp = exporters;
  last = NULL;
  while (exp) {
    last = exp;
    exp = exp->next;
  }
  exp = (struct simage_stream_exporter*) malloc(sizeof(struct simage_stream_exporter));
  exp->create = create;
  exp->put = put;
  exp->close = close;
  exp->next = NULL;

  if (last == NULL) {
    exporters = exp;
  }
  else last->next = exp;
}
