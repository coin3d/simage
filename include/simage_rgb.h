#ifndef _SIMAGE_RGB_H_
#define _SIMAGE_RGB_H_

#include <config.h>

#ifndef SIMAGE_RGB_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_RGB_SUPPORT */

unsigned char *simage_rgb_load(const char *filename,
				int *width,
				int *height,
				int *numComponents);
int simage_rgb_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_rgb_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_RGB_H_ */
