#ifndef _SIMAGE_PNG_H_
#define _SIMAGE_PNG_H_

#ifndef HAVE_LIBPNG
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_LIBPNG */

unsigned char *simage_png_load(const char *filename,
				int *width,
				int *height,
				int *numComponents);
int simage_png_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_png_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_PNG_H_ */
