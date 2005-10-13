#ifndef _SIMAGE_JASPER_H_
#define _SIMAGE_JASPER_H_

#include <config.h>

#ifndef HAVE_JASPER
#error "This file should not be used under the current configuration!"
#endif /* !HAVE_JASPERLIB */

unsigned char * simage_jasper_load(const char * filename,
                                   int * width,
                                   int * height,
                                   int * numComponents);
int simage_jasper_save(const char * filename,
                       const unsigned char * bytes,
                       int width,
                       int height,
                       int numcomponents);
int simage_jasper_identify(const char * filename,
                           const unsigned char * header,
                           int headerlen);

int simage_jasper_error(char * buffer, int bufferlen);

/* new for simage 1.6 */
void * simage_jasper_open(const char * filename,
                          int * width,
                          int * height,
                          int * numcomponents);
void simage_jasper_close(void * opendata);
int simage_jasper_read_line(void * opendata, int y, unsigned char * buf);

#endif /* _SIMAGE_JASPER_H_ */
