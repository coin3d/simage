#ifndef _SIMAGE_TGA_H_
#define _SIMAGE_TGA_H_

unsigned char *simage_tga_load(const char * filename,
				int *width,
				int *height,
				int *numComponents);
int simage_tga_identify(const char *filename,
			 const unsigned char *header,
			 int headerlen);

int simage_tga_error(char *buffer, int bufferlen);

#endif /* _SIMAGE_TGA_H_ */
