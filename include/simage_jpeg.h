#ifndef _SIMAGE_JPEG_H_
#define _SIMAGE_JPEG_H_

#include <config.h>

#ifndef HAVE_JPEGLIB
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_JPEGLIB */


unsigned char * simage_jpeg_load(const char *filename,
				 int *width,
				 int *height,
				 int *numcomponents);
int simage_jpeg_save(const char *filename,
                     const unsigned char * bytes,
                     int width,
                     int height,
                     int numcomponents);

int simage_jpeg_identify(const char * filename,
                         const unsigned char * header,
                         int headerlen);

int simage_jpeg_error(char * textbuffer, int buffersize);

#endif /* _SIMAGE_JPEG_H_ */
