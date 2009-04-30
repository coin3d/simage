#ifndef _SIMAGE_CGIMAGE_H_
#define _SIMAGE_CGIMAGE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif

  int simage_cgimage_error(char * textbuffer, int buffersize);
  
  int simage_cgimage_identify(const char * filename,
                              const unsigned char * header,
                              int headerlen);
  
  unsigned char * simage_cgimage_load(const char *filename,
                                      int *width,
                                      int *height,
                                      int *numcomponents);
  
/*   char * simage_cgimage_get_savers(void);  */
  
/*   int simage_cgimage_save(const char *filename, */
/*                           const unsigned char * bytes, */
/*                           int width, */
/*                           int height, */
/*                           int numcomponents, */
/*                           const char * ext); */
  
  
/*   void * simage_cgimage_open(const char * filename, */
/*                              int * width, */
/*                              int * height, */
/*                              int * numcomponents); */
/*   void simage_cgimage_close(void * opendata); */
/*   int simage_cgimage_read_line(void * opendata, int y, unsigned char * buf); */
  
#ifdef __cplusplus
}
#endif

#endif /* _SIMAGE_CGIMAGE_H_ */
