#ifndef SIMAGE_AVI_H
#define SIMAGE_AVI_H

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

#ifndef SIMAGE_AVIENC_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_AVIENC_SUPPORT */

#include <simage.h>

#ifdef __cplusplus
extern "C" {
#endif

  int avienc_movie_create(const char * filename, s_movie * movie, s_params * params);
  int avienc_movie_put(s_movie * movie, s_image * image, s_params * params);
  void avienc_movie_close(s_movie * movie);

#ifdef __cplusplus
}
#endif

#endif /* SIMAGE_AVI_H */

