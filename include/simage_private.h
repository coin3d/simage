#ifndef SIMAGE_PRIVATE_H
#define SIMAGE_PRIVATE_H

/*
 * Copyright (c) Kongsberg Oil & Gas Technologies
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "simage.h"

#ifdef __cplusplus
extern "C" {
#endif

  struct simage_open_funcs {
    void * (*open_func)(const char * filename,
                        int * w, int * h, int * nc);
    int (*read_line_func)(void * opendata, int y, unsigned char * buf);
    int (*next_line_func)(void * opendata, unsigned char * buf);
    void (*close_func)(void * opendata);
  };

  struct simage_image_s {
    int width;
    int height;
    int components;
    int didalloc;
    int order;
    unsigned char * data;
    /* added for simage 1.6 */
    void * opendata;
    int oktoreadall;
    char * openfilename;
    struct simage_open_funcs openfuncs;
  };

  s_params * s_movie_params(s_movie * movie);

  void * s_stream_context_get(s_stream *stream);
  void s_stream_context_set(s_stream *stream, void *context);

#ifdef __cplusplus
}
#endif

#endif /* SIMAGE_PRIVATE_H */
