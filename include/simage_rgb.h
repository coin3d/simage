#ifndef _SIMAGE_RGB_H_
#define _SIMAGE_RGB_H_

unsigned char *simage_rgb_load(const char *filename,
				int *width,
				int *height,
				int *numComponents);
int simage_rgb_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_rgb_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_RGB_H_ */
