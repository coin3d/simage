#ifndef _SIMAGE_TGA_H_
#define _SIMAGE_TGA_H_

#include <config.h>

#ifndef SIMAGE_TGA_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_TGA_SUPPORT */

unsigned char *simage_tga_load(const char * filename,
				int *width,
				int *height,
				int *numComponents);
int simage_tga_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_tga_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_TGA_H_ */
