#ifndef SIMAGE_OGGVORBIS_H
#define SIMAGE_OGGVORBIS_H

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef SIMAGE_OGGVORBIS_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_OGGVORBIS_SUPPORT */

#include <simage.h>

#ifdef __cplusplus
extern "C" {
#endif

  int oggvorbis_reader_stream_open(const char * filename, s_stream * stream,
                                   s_params * params);
  void * oggvorbis_reader_stream_get(s_stream * stream, void * buffer,
                                     int *size, s_params * params);
  int oggvorbis_reader_stream_seek(s_stream * stream, int offset, int whence,
                                   s_params * params);
  int oggvorbis_reader_stream_tell(s_stream * stream, s_params * params);
  void oggvorbis_reader_stream_close(s_stream * stream);

#ifdef __cplusplus
}
#endif

#endif /* SIMAGE_OGGVORBIS_H */
