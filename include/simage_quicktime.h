#ifndef _SIMAGE_QUICKTIME_H_
#define _SIMAGE_QUICKTIME_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

  int simage_quicktime_error(char * textbuffer, int buffersize);
  
  int simage_quicktime_identify(const char * filename,
                                const unsigned char * header,
                                int headerlen);

  unsigned char * simage_quicktime_load(const char *filename,
                                        int *width,
                                        int *height,
                                        int *numcomponents);
  
  char * simage_quicktime_get_savers(void); 

  int simage_quicktime_save(const char *filename,
                            const unsigned char * bytes,
                            int width,
                            int height,
                            int numcomponents,
                            const char * ext);
  

#endif /* _SIMAGE_QUICKTIME_H_ */

