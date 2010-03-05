#ifndef _SIMAGE_GDIPLUS_H_
#define _SIMAGE_GDIPLUS_H_

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

#ifndef SIMAGE_GDIPLUS_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_GDIPLUS_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif

  int simage_gdiplus_error(char * textbuffer, int buffersize);

  int simage_gdiplus_identify(const char * filename,
                              const unsigned char * header,
                              int headerlen);

  unsigned char * simage_gdiplus_load(const char * filename,
                                      int * width,
                                      int * height,
                                      int * numcomponents);

  char * simage_gdiplus_get_savers(void); /* extra function needed to create saver info */
  int simage_gdiplus_save(const char * filename,
                          const unsigned char * bytes,
                          int width,
                          int height,
                          int numcomponents,
                          const char * ext);

/* new for simage 1.6 */
  void * simage_gdiplus_open(const char * filename,
                             int * width,
                             int * height,
                             int * numcomponents);
  void simage_gdiplus_close(void * opendata);
  int simage_gdiplus_read_line(void * opendata, int y, unsigned char * buf);

/*
 * FIXME: unstable experimental region support API. Use it without
 * knowing why and a fiercely looking fluffy cow will cross your way
 * one day. The fluffy cow is gonna eat up all your code and will
 * happily moo the doomsday symphony while blocking your getaway
 * road. YA HAVE BEEN WARNED! 20060520 tamer.
 */
  int simage_gdiplus_read_region(void * opendata,
                                 int x, int y, int w, int h,
                                 unsigned char * buf);

#ifdef __cplusplus
}
#endif

#endif /* _SIMAGE_GDIPLUS_H_ */
