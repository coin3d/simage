#ifndef _SIMAGE_TIFF_H_
#define _SIMAGE_TIFF_H_

#include <config.h>

#ifndef HAVE_TIFFLIB
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_TIFFLIB */

unsigned char * simage_tiff_load(const char * filename,
				 int * width,
				 int * height,
				 int * numComponents);
int simage_tiff_save(const char * filename,
                     const unsigned char * bytes,
                     int width,
                     int height,
                     int numcomponents);
int simage_tiff_identify(const char * filename,
			  const unsigned char * header,
			  int headerlen);

int simage_tiff_error(char * buffer, int bufferlen);

/* new for simage 1.6 */
void * simage_tiff_open(const char * filename,
                        int * width,
                        int * height,
                        int * numcomponents);
void simage_tiff_close(void * opendata);
int simage_tiff_read_line(void * opendata, int y, unsigned char * buf);

#endif /* _SIMAGE_TIFF_H_ */
