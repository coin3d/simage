#ifndef _SIMAGE_JPEG_H_
#define _SIMAGE_JPEG_H_

unsigned char *simage_jpeg_load(const char *filename,
				 int *width,
				 int *height,
				 int *numComponents);
int simage_jpeg_identify(const char *filename,
			  const unsigned char *header,
			  const int headerlen);

int simage_jpeg_error(char *textbuffer, int buffersize);

#endif /* _SIMAGE_JPEG_H_ */
