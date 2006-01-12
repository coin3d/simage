#ifndef _SIMAGE_GIF_H_
#define _SIMAGE_GIF_H_

#include <config.h>

#ifndef HAVE_GIFLIB
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_GIFLIB */

unsigned char *simage_gif_load(const char * filename,
                               int *width,
                               int *height,
                               int *numComponents);

int simage_gif_save(const char *filename,
                    const unsigned char * bytes,
                    int width,
                    int height,
                    int numcomponents);

int simage_gif_identify(const char *filename,
                        const unsigned char *header,
                        int headerlen);

int simage_gif_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_GIF_H_ */
