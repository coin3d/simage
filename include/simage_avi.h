#ifndef SIMAGE_AVI_H
#define SIMAGE_AVI_H

#include <config.h>

#ifndef SIMAGE_AVIENC_SUPPORT
#error "This file should not be used under the current configuration!"
#endif /* !SIMAGE_AVIENC_SUPPORT */

#include <simage.h>

int avienc_movie_create(const char * filename, s_movie * movie, s_params * params);
int avienc_movie_put(s_movie * movie, s_image * image, s_params * params);
void avienc_movie_close(s_movie * movie);

#endif /* SIMAGE_AVI_H */

