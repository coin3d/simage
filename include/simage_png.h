#ifndef _SIMAGE_PNG_H_
#define _SIMAGE_PNG_H_

#include <config.h>

#ifndef SIMAGE_PNG_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_PNG_SUPPORT */

unsigned char *simage_png_load(const char *filename,
				int *width,
				int *height,
				int *numComponents);
int simage_png_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_png_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_PNG_H_ */
