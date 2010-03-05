#ifndef SIMAGE_XWD_H
#define SIMAGE_XWD_H

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

#ifndef SIMAGE_XWD_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_XWD_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif

  unsigned char * simage_xwd_load(const char * filename,
                                  int * width,
                                  int * height,
                                  int * numcomponents);
  int simage_xwd_save(const char * filename,
                      const unsigned char * bytes,
                      int width,
                      int height,
                      int numcomponents);
  int simage_xwd_identify(const char * filename,
                          const unsigned char * header,
                          int headerlen);

  int simage_xwd_error(char * buffer, int bufferlen);

#ifdef __cplusplus
}
#endif

#endif /* ! SIMAGE_XWD_H */
