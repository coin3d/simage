#ifndef SIMAGE_PRIVATE_H
#define SIMAGE_PRIVATE_H

#include "simage.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  s_params * s_movie_params(s_movie * movie);

  void * s_stream_context_get(s_stream *stream);
  void s_stream_context_set(s_stream *stream, void *context);
  
#ifdef __cplusplus
}
#endif

#endif /* SIMAGE_PRIVATE_H */
