#ifndef SIMAGE_OGGVORBIS_H
#define SIMAGE_OGGVORBIS_H

#include <config.h>

#ifndef SIMAGE_OGGVORBIS_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_OGGVORBIS_SUPPORT */

#include <simage.h>

int oggvorbis_reader_stream_open(const char * filename, s_stream * stream, 
                                 s_params * params);
void * oggvorbis_reader_stream_get(s_stream * stream, void * buffer, 
                                   int *size, s_params * params);
int oggvorbis_reader_stream_seek(s_stream * stream, int offset, int whence,
                                 s_params * params);
int oggvorbis_reader_stream_tell(s_stream * stream, s_params * params);
void oggvorbis_reader_stream_close(s_stream * stream);

#endif /* SIMAGE_OGGVORBIS_H */
