#ifndef _SIMAGE_PIC_H_
#define _SIMAGE_PIC_H_

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

#ifndef SIMAGE_PIC_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_PIC_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif

  unsigned char *simage_pic_load(const char *filename,
				 int *width,
				 int *height,
				 int *numComponents);
  int simage_pic_identify(const char *filename,
			  const unsigned char *header,
			  int headerlen);

  int simage_pic_error(char *buffer, int bufferlen);

#ifdef __cplusplus
}
#endif

#endif /* _SIMAGE_PIC_H_ */
