#ifndef SIMAGE_LIBSNDFILE_H
#define SIMAGE_LIBSNDFILE_H

#include <config.h>

#ifndef SIMAGE_LIBSNDFILE_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_LIBSNDFILE_SUPPORT */

#include <simage.h>

int libsndfile_stream_open(const char * filename, s_stream * stream, 
                                 s_params * params);
void * libsndfile_stream_get(s_stream * stream, void * buffer, 
                                   int *size, s_params * params);
int libsndfile_stream_seek(s_stream * stream, int offset, int whence,
                                 s_params * params);
int libsndfile_stream_tell(s_stream * stream, s_params * params);
void libsndfile_stream_close(s_stream * stream);

#endif /* SIMAGE_LIBSNDFILE_H */
