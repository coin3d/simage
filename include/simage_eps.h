#ifndef _SIMAGE_EPS_H_
#define _SIMAGE_EPS_H_

#include <config.h>

#ifndef SIMAGE_EPS_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_EPS_SUPPORT */

int simage_eps_save(const char * filename,
                    const unsigned char * bytes,
                    int width,
                    int height,
                    int numcomponents);
int simage_eps_error(char * buffer, int bufferlen);

#endif /* _SIMAGE_EPS_H_ */
