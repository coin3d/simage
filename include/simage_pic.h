#ifndef _SIMAGE_PIC_H_
#define _SIMAGE_PIC_H_

#include <config.h>

#ifndef SIMAGE_PIC_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_PIC_SUPPORT */

unsigned char *simage_pic_load(const char *filename,
				int *width,
				int *height,
				int *numComponents);
int simage_pic_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_pic_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_PIC_H_ */
