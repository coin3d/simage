#ifndef _SIMAGE_QIMAGE_H_
#define _SIMAGE_QIMAGE_H_

#include <config.h>

#ifndef SIMAGE_QIMAGE_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_QIMAGE_SUPPORT */


#ifdef __cplusplus
extern "C" {
#endif

  unsigned char * simage_qimage_load(const char *filename,
                                     int *width,
                                     int *height,
                                     int *numcomponents);
  
  char * simage_qimage_get_savers(void); /* extra function needed to create saver info */
  int simage_qimage_save(const char *filename,
                         const unsigned char * bytes,
                         int width,
                         int height,
                         int numcomponents,
                         const char * ext);
  
  int simage_qimage_identify(const char * filename,
                             const unsigned char * header,
                             int headerlen);
  
  int simage_qimage_error(char * textbuffer, int buffersize);
  

#ifdef __cplusplus
}
#endif

#endif /* _SIMAGE_QIMAGE_H_ */
