#ifndef SIMAGE_XWD_H
#define SIMAGE_XWD_H

#include <config.h>

#ifndef SIMAGE_XWD_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_XWD_SUPPORT */

unsigned char * simage_xwd_load(const char * filename,
				int * width,
				int * height,
				int * numcomponents);
int simage_xwd_save(const char * filename,
                    const unsigned char * bytes,
                    int width,
                    int height,
                    int numcomponents);
int simage_xwd_identify(const char * filename,
                        const unsigned char * header,
                        int headerlen);

int simage_xwd_error(char * buffer, int bufferlen);

#endif /* ! SIMAGE_XWD_H */
