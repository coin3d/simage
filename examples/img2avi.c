#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* MSVC: link with setargv.obj to process wildcard arguments */

#include <simage.h>

int main(int argc, char *argv[])
{
  int i;
  char *filename;
  int fps;
  int first;
  int width;
  int height;
  s_movie * movie;
  s_params * params;
  s_image *image;
  char formatbuf[20];

  first = 1;

  if (argc < 4) {
    printf("USAGE: IMG2AVI <filename.avi> <fps> <imagefile1> <imagefile2> ...\n"
           "All images must have the same width and height, and both must be divisible by 4\n");
    return 0;
  }

  filename = argv[1];
  fps = atoi(argv[2]);

  sprintf(formatbuf, "%%%dd/%%%dd  \"%%s\"", (int)ceil(log10(argc-3)), (int)ceil(log10(argc-3)));

  for (i=3; i<argc; i++) {

    /* read image */
    printf(formatbuf, i-2, argc-3, argv[i]);
    printf(" ");
    image = s_image_load(argv[i], NULL);
    if (image==NULL) {
      fprintf(stderr, "\nError: Couldn't read image from file '%s'.\n", argv[i]);
      exit(1);
    }
    printf("(%dx%dx%d)\n", s_image_width(image), s_image_height(image), s_image_components(image));

    if (first) {
      /* Create movie file */
      first = 0;
      width = s_image_width(image);
      height = s_image_height(image);
      if ( (width % 4 != 0) || (height % 4 != 0) ) {
        fprintf(stderr, "\nError: Image ('%s') width (%d) and height (%d) must be divisible by 4\n", filename, width, height);
        exit(1);
      }
      params = s_params_create();
      s_params_set(params, 
                   "mime-type", S_STRING_PARAM_TYPE, "video/avi",
                   "width", S_INTEGER_PARAM_TYPE, width,
                   "height", S_INTEGER_PARAM_TYPE, height,
                   "fps", S_INTEGER_PARAM_TYPE, fps,
                   "parameter file", S_STRING_PARAM_TYPE, "",
                   /* NULL means no more params */
                   NULL);
      movie = s_movie_create(filename, params);
      if (movie == NULL) {
        fprintf(stderr, "\nError: Could not create movie file '%s'\n", filename);
        exit(1);
      }
    }

    if ( (s_image_width(image) != width) || (s_image_height(image) != height) ) {
      fprintf(stderr, "\nError: All images must have the same width and height.\n", width, height);
      exit(1);
    }

    /* Encode image */ 
    s_movie_put_image(movie, image, NULL);

    s_image_destroy(image);
  }

  /* Clean up */
  if (movie != NULL) {
    s_movie_close(movie);
    s_movie_destroy(movie);
  }

  return 0;
};