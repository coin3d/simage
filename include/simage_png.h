#ifndef _SIMAGE_PNG_H_
#define _SIMAGE_PNG_H_

#include <config.h>

#ifndef HAVE_PNGLIB
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_PNGLIB */

unsigned char * simage_png_load(const char * filename,
                                int * width,
                                int * height,
                                int * numComponents);
int simage_png_save(const char * filename,
                    const unsigned char * bytes, 
                    int width,
                    int height,
                    int numcomponents);
int simage_png_identify(const char * filename,
                        const unsigned char * header,
                        int headerlen);

int simage_png_error(char * buffer, int bufferlen);

#endif /* _SIMAGE_PNG_H_ */
