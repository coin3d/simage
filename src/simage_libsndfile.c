#include <config.h>
#ifdef SIMAGE_LIBSNDFILE_SUPPORT

#include <simage.h>
#include <simage_private.h>
#include <simage_libsndfile.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sndfile.h>

typedef struct {
  SNDFILE *file;
  SF_INFO sfinfo;
  double *tempbuffer;
  int tempbuffersize;
} libsndfile_context;

static void 
libsndfile_init_context(libsndfile_context *context)
{
  context->file = NULL;
  context->tempbuffer = NULL;
  context->tempbuffersize = 0;
}

static void 
libsndfile_cleanup_context(libsndfile_context *context)
{
  if (context->tempbuffer) 
    free(context->tempbuffer);
  context->tempbuffer = NULL;
  context->tempbuffersize = 0;
}

int 
libsndfile_stream_open(const char * filename, s_stream * stream,
                             s_params * params)
{
  libsndfile_context *context;
  FILE *dummyfile;

  dummyfile = fopen(filename, "rb");
  if (!dummyfile)
    return 0;
  else
    fclose(dummyfile);

  context = (libsndfile_context *) malloc(sizeof(libsndfile_context));
  libsndfile_init_context(context);

  context->file = sf_open (filename, SFM_READ, &context->sfinfo) ;
  if (!context->file) {
    libsndfile_cleanup_context(context);
    free(context);
    return 0;
  }
  sf_command (context->file, SFC_SET_NORM_DOUBLE, NULL, SF_TRUE) ;

  s_stream_context_set(stream, (void *)context);

  /* FIXME: SF_INFO::frames is of type sf_count_t, which is a 32bit
     integer in some libsndfile version and 64bit on others (like the 
     Mac OS X one), so casting context->sfinfo.frames to int is not 
     totally correct - but necessary, since we do not have a 64bit 
     s_params type. 
     20030108 kyrah
  */
  s_params_set(s_stream_params(stream),
               "samplerate", S_INTEGER_PARAM_TYPE, context->sfinfo.samplerate,
               "frames",     S_INTEGER_PARAM_TYPE, (int) context->sfinfo.frames,
               "channels",   S_INTEGER_PARAM_TYPE, context->sfinfo.channels,
               NULL);

  return 1;
}

void * 
libsndfile_stream_get(s_stream * stream, void * buffer, int * size, s_params * params)
{
  int itemsread;
  libsndfile_context *context;
  int items;
  int itemssize;
  int i;
  short int *intbuffer;

  context = (libsndfile_context *)s_stream_context_get(stream);

  if (context != NULL) {
    /* fixme 20020924 thammer : support other (return) formats
     * than 16 bit signed. This should be very little work!
     */

    /*
     * size must be an integer multiple of bytespersample*channels
     */

    if ( (*size) % (2 * context->sfinfo.channels) ) {
      *size = 0;
      return NULL;
    }

    items = *size / 2;
    itemssize = items*sizeof(double);

    if (context->tempbuffersize < itemssize) {
      if (context->tempbuffer)
        free(context->tempbuffer);
      context->tempbuffer = (double *)malloc(itemssize);
    }

    intbuffer = (short int*)buffer;
    itemsread = sf_read_double(context->file, context->tempbuffer, items);
    for (i=0; i<itemsread; i++) {
      intbuffer[i] = context->tempbuffer[i] * (double)32767.0;
    }

    if (itemsread > 0) {
      *size = itemsread * 2;
      return buffer;
    }

    /* fixme 20020924 thammer: check params for conversion requests
     */
  }
  *size = 0;
  return NULL;
}

void 
libsndfile_stream_close(s_stream * stream)
{
  libsndfile_context *context;
  context = (libsndfile_context *)s_stream_context_get(stream);
  if (context != NULL) {
    sf_close(context->file);
    context->file = NULL;
  }
  libsndfile_cleanup_context(context);
  free(context);
}

int 
libsndfile_stream_seek(s_stream * stream, int offset, int whence, 
                        s_params *params)
{
  libsndfile_context *context;
  context = (libsndfile_context *)s_stream_context_get(stream);
  if (context != NULL) {
    return sf_seek(context->file, offset, whence);
  }
  else
    return -1;
}

int 
libsndfile_stream_tell(s_stream * stream, s_params *params)
{
  libsndfile_context *context;
  context = (libsndfile_context *)s_stream_context_get(stream);
  if (context != NULL) {
    return sf_seek(context->file, 0, SEEK_CUR);
  }
  else
    return -1;
}

#endif /* SIMAGE_LIBSNDFILE_SUPPORT */
