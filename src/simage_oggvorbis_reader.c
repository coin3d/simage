#include <config.h>
#ifdef SIMAGE_OGGVORBIS_SUPPORT

#include <simage_private.h>
#include <simage_oggvorbis.h>

#include <stdio.h>

typedef struct {
  FILE *file;
  OggVorbis_File vorbisfile;
  int current_section;
} oggvorbis_reader_context;

int 
oggvorbis_reader_stream_open(const char * filename, s_stream * stream)
{
  oggvorbis_reader_context *context;
  int channels;

  if (!oggvorbis_reader_open(&context, filename)) 
    return 0;
  
  s_params_set(s_movie_params(movie), "oggvorbis reader handle", 
               S_POINTER_PARAM_TYPE, (void *) context, 0);
  channels = oggvorbis_reader_get_num_channels(context);
  s_params_set(s_movie_params(movie), "channels", 
               S_INTEGER_PARAM_TYPE, channels, 0);
  return 1;
}

int 
oggvorbis_reader_stream_get(s_stream * stream, void * buffer, int * size, s_params * params)
{
  void * handle;

  if (s_params_get(s_stream_params(stream), "oggvorbis reader handle", 
                   S_POINTER_PARAM_TYPE, &handle, 0)) {
    return oggvorbis_reader_read((oggvorbis_reader_context *)handle, buffer, 
                                 size);
    /* fixme 20020904 thammer: check parameters for conversion requests
     */
  }
  return 0;
}

void 
oggvorbis_reader_stream_close(s_movie * movie)
{
  void * handle;
  if (s_params_get(s_stream_params(movie), "oggvorbis reader handle", 
                   S_POINTER_PARAM_TYPE, &handle, 0)) {
    return oggvorbis_reader_close((oggvorbis_reader_context *)handle);
  }
}

static void oggvorbis_reader_init_context(oggvorbis_reader_context *context)
{
  context->file = NULL;
  context->current_section = 0;
}

static void oggvorbis_reader_cleanup_context(oggvorbis_reader_context *context)
{
  oggvorbis_reader_close();
}

size_t oggvorbis_reader_read_cb(void *ptr, size_t size, size_t nmemb, void *datasource)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  return fread(ptr, size, nmemb, context->file);
}

int oggvorbis_reader_seek_cb(void *datasource, ogg_int64_t offset, int whence)
{
  return -1; /* seek not supported */
}

int oggvorbis_reader_close_cb(void *datasource)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  if (context->file != NULL)
    fclose(context->file);
  context->file = NULL;
  return 0;
}

int oggvorbis_reader_open(oggvorbis_reader_context **contextp, 
                          const char *filename)
{
  *contextp = (oggvorbis_reader_context *) 
    malloc(sizeof(oggvorbis_reader_context));
  oggvorbis_reader_init_context(contextp);
  oggvorbis_reader_context *context = *contextp;  
  
  context->file = fopen(filename, "rb");
  if (context->file == NULL) {
    oggvorbis_reader_cleanup_context(context);
    free(context);
    return 0;
  }

  ov_callbacks callbacks;
  callbacks.read_func = oggvorbis_reader_read_cb;
  callbacks.seek_func = oggvorbis_reader_seek_cb;
  callbacks.close_func = oggvorbis_reader_close_cb;
  callbacks.tell_func = NULL;

  if(ov_open_callbacks((void *)context, &context->vorbisfile, NULL, 0, callbacks) < 0) {
    fclose(context->file);
    context->file = NULL;
    oggvorbis_reader_cleanup_context(context);
    free(context);
    return 0;
  }

  return 1;
}

int oggvorbis_reader_read(oggvorbis_reader_context *context, 
                          char *buffer, int size)
{
  int readsize = 0;
  int numread = 0;
  while (readsize<size) {
    numread=ov_read(&context->vorbisfile, buffer+readsize, size-readsize, 0, 2, 1, &context->current_section);
    if (numread<=0)
      return numread;
    else
      readsize += numread;
  }
  return readsize;
}

int oggvorbis_reader_get_num_channels(oggvorbis_reader_context *context)
{
  if (context->file == NULL)
    return 0;
  vorbis_info *vi=ov_info(&context->vorbisfile,-1);
  return vi->channels;
}


void oggvorbis_reader_close(oggvorbis_reader_context *context)
{
  if (context->file != NULL)
    ov_clear(&context->vorbisfile);

  oggvorbis_reader_cleanup_context(context);
  free(context);
}





#endif /* SIMAGE_OGGVORBIS_SUPPORT */
