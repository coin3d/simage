#ifndef _SIMAGE_RGB_H_
#define _SIMAGE_RGB_H_

#include <config.h>

#ifndef SIMAGE_RGB_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_RGB_SUPPORT */

unsigned char * simage_rgb_load(const char * filename,
				int * width,
				int * height,
				int * numcomponents);
int simage_rgb_save(const char * filename,
                    const unsigned char * bytes,
                    int width,
                    int height,
                    int numcomponents);
int simage_rgb_identify(const char * filename,
                        const unsigned char * header,
                        int headerlen);

int simage_rgb_error(char * buffer, int bufferlen);

/* new for simage 1.6 */
void * simage_rgb_open(const char * filename,
                       int * width,
                       int * height,
                       int * numcomponents);
void simage_rgb_close(void * opendata);
int simage_rgb_read_line(void * opendata, int y, unsigned char * buf);


#endif /* _SIMAGE_RGB_H_ */
