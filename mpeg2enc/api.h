#ifndef MPEG2ENC_API_H
#define MPEG2ENC_API_H

#include <simage.h>

int mpeg2enc_movie_create(const char * filename, s_movie * movie, s_params * params);
int mpeg2enc_movie_put(s_movie * movie, s_image * image, s_params * params);
void mpeg2enc_movie_close(s_movie * movie);

typedef void (*SimpegWrite_error_cb)(void * userdata, const char * text);
typedef void (*SimpegWrite_warning_cb)(void * userdata, const char * text);
typedef int  (*SimpegWrite_progress_cb)(void * userdata, float sub, int current_frame, int num_frames);

#endif /* MPEG2ENC_API_H */
