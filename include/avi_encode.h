#ifndef AVI_ENCODE_H_
#define AVI_ENCODE_H_

#ifndef SIMAGE_AVIENC_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_AVIENC_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif

void * avi_begin_encode(const char *filename, int width, int height, int fps, const char *preferences_filename);
int avi_encode_bitmap(void *handle, unsigned char *buf, int rgb2bgr);
int avi_end_encode(void *handle);

#ifdef __cplusplus
}
#endif


#endif /* AVI_ENCODE_H_ */

