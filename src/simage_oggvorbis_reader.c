#include <config.h>
#ifdef SIMAGE_OGGVORBIS_SUPPORT

#include <simage.h>
#include <simage_private.h>
#include <simage_oggvorbis.h>

#include <stdio.h>
#include <stdlib.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

typedef struct {
  FILE *file;
  OggVorbis_File vorbisfile;
  int current_section;
} oggvorbis_reader_context;


static void 
oggvorbis_reader_init_context(oggvorbis_reader_context *context)
{
  context->file = NULL;
  context->current_section = 0;
}

static void 
oggvorbis_reader_cleanup_context(oggvorbis_reader_context *context)
{
}

static size_t 
oggvorbis_reader_read_cb(void *ptr, size_t size, size_t nmemb, 
                                void *datasource)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  return fread(ptr, size, nmemb, context->file);
}

static int 
oggvorbis_reader_seek_cb(void *datasource, ogg_int64_t offset, int whence)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  return fseek(context->file, (int)offset, whence);
}

static long
oggvorbis_reader_tell_cb(void *datasource)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  return ftell(context->file);
}

static int 
oggvorbis_reader_close_cb(void *datasource)
{
  oggvorbis_reader_context *context = (oggvorbis_reader_context *)datasource;
  if (context->file != NULL)
    fclose(context->file);
  context->file = NULL;
  return 0;
}

static int 
oggvorbis_reader_open(oggvorbis_reader_context **contextp, 
                      const char *filename)
{
  ov_callbacks callbacks;
  oggvorbis_reader_context *context;  

  *contextp = (oggvorbis_reader_context *) 
    malloc(sizeof(oggvorbis_reader_context));
  oggvorbis_reader_init_context(*contextp);
  context = *contextp;  
  
  context->file = fopen(filename, "rb");
  if (context->file == NULL) {
    oggvorbis_reader_cleanup_context(context);
    free(context);
    return 0;
  }

  callbacks.read_func = oggvorbis_reader_read_cb;
  callbacks.seek_func = oggvorbis_reader_seek_cb;
  callbacks.close_func = oggvorbis_reader_close_cb;
  callbacks.tell_func = oggvorbis_reader_tell_cb;

  if(ov_open_callbacks((void *)context, &context->vorbisfile, NULL, 0, 
                       callbacks) < 0) {
    fclose(context->file);
    context->file = NULL;
    oggvorbis_reader_cleanup_context(context);
    free(context);
    return 0;
  }

  return 1;
}

static int 
oggvorbis_reader_read(oggvorbis_reader_context *context, 
                      char *buffer, int size)
{
  int readsize;
  int numread;
  int bigEndian;

  union {
    int testWord;
    char testByte[4];
  } endianTest;

  endianTest.testWord = 1;
  if (endianTest.testByte[0] == 1) {
    bigEndian = 0; // little endian
  } else {
    bigEndian = 1; // big endian
  }


  readsize = 0;
  numread = 0;

  while (readsize<size) {
    /* FIXME: Check how libvorbisfile handles odd sizes.  The
       libvorbisfile documentation doesn't say anything about this, so
       everything might be OK.  
       2002-01-10 thammer.  
    */
    numread=ov_read(&context->vorbisfile, 
                    buffer+readsize, 
                    size-readsize, bigEndian, 2, 1, 
                    &context->current_section);
    /* FIXME: Verify that EOF's (and numread=0) are
       handled correctly. 2003-01-09 thammer
     */
    if (numread<=0)
      return numread;
    else
      readsize += numread;
  }

  return readsize;
}

static void 
oggvorbis_reader_get_stream_info(oggvorbis_reader_context *context, 
                                 int *channels, int *samplerate)
{
  if (context->file) {
    vorbis_info * vi;
    vi = ov_info(&context->vorbisfile,-1);
    *channels = vi->channels;
    *samplerate = vi->rate;
  }
}


static void 
oggvorbis_reader_close(oggvorbis_reader_context *context)
{
  if (context->file != NULL)
    ov_clear(&context->vorbisfile);

  oggvorbis_reader_cleanup_context(context);
  free(context);
}


int 
oggvorbis_reader_stream_open(const char * filename, s_stream * stream,
                             s_params * params)
{
  oggvorbis_reader_context *context;
  int channels, samplerate;
  
  if (!oggvorbis_reader_open(&context, filename)) 
    return 0;

  s_stream_context_set(stream, (void *)context);

  oggvorbis_reader_get_stream_info(context, &channels, &samplerate);
  s_params_set(s_stream_params(stream),
               "channels",   S_INTEGER_PARAM_TYPE, channels,
               "samplerate", S_INTEGER_PARAM_TYPE, samplerate,
               NULL);
  return 1;
}

void * 
oggvorbis_reader_stream_get(s_stream * stream, void * buffer, int * size, s_params * params)
{
  int ret;
  oggvorbis_reader_context *context;

  context = (oggvorbis_reader_context *)s_stream_context_get(stream);

  if (context != NULL) {
    ret = oggvorbis_reader_read(context, (char*) buffer, *size);
    if (ret>0) {
      *size = ret;
      return buffer;
    }
    /* fixme 20020904 thammer: check params for conversion requests
     */
  }
  *size=0;
  return NULL;
}

void 
oggvorbis_reader_stream_close(s_stream * stream)
{
  oggvorbis_reader_context *context;

  context = (oggvorbis_reader_context *)s_stream_context_get(stream);
  if (context != NULL) {
    oggvorbis_reader_close(context);
  }
}

int
oggvorbis_reader_stream_seek(s_stream * stream, int offset, int whence,
                             s_params * params)
{
  oggvorbis_reader_context *context;

  context = (oggvorbis_reader_context *)s_stream_context_get(stream);

  if (context != NULL) {
    if (whence == SIMAGE_SEEK_SET)
      return ov_pcm_seek(&context->vorbisfile, offset);
    else
      return -1;
    /* FIXME: Support the whence parameter. 2003-03-10 thammer */
  }
  else
    return -1;
}

int
oggvorbis_reader_stream_tell(s_stream * stream, s_params * params)
{
  oggvorbis_reader_context *context;

  context = (oggvorbis_reader_context *)s_stream_context_get(stream);

  if (context != NULL) {
    return ov_pcm_tell(&context->vorbisfile);
  }
  else
    return -1;
}

#endif /* SIMAGE_OGGVORBIS_SUPPORT */
