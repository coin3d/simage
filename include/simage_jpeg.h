#ifndef SIMAGE_JPEG_H
#define SIMAGE_JPEG_H

/* This is an internal header file to the simage library. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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

#endif /* SIMAGE_JPEG_H */
