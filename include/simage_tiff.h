#ifndef _SIMAGE_TIFF_H_
#define _SIMAGE_TIFF_H_

#include <config.h>

#ifndef SIMAGE_TIFF_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_TIFF_SUPPORT */

unsigned char *simage_tiff_load(const char *filename,
				 int *width,
				 int *height,
				 int *numComponents);
int simage_tiff_identify(const char *filename,
			  const unsigned char *header,
			  int headerlen);

int simage_tiff_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_TIFF_H_ */
