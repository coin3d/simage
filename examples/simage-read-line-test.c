/*
 * simage-read-line-test.c 
 *
 * A simple program for testing line-by-line image access.
 *
 * This program is public domain.
 */

#include <simage.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void 
usage(const char * argv0)
{
  fprintf(stderr, "Usage:\n %s <infile>\n");
}

int main(int argc, char ** argv)
{
  int i;
  int w, h, nc;
  char * infile;
  char * addalpha = NULL;
  unsigned char * row = NULL;
  s_image * image = NULL;

  if (argc < 2) {
    usage(argv[0]);
    return -1;
  }

  image = s_image_open(argv[1], 1);
  
  if (!image) {
    fprintf(stderr,"Unable to open file: %s\n" ,argv[1]);
    return -1;
  }

  w = s_image_width(image);
  h = s_image_height(image);
  nc = s_image_components(image);

  fprintf(stderr,"testing file: %s, %d %d %d\n", argv[1], w, h, nc);

  row = (unsigned char*) malloc(w*nc);

  fprintf(stderr,"testing step: %s, %d %d %d\n", argv[1], w, h, nc);
  for (i = 0; i < h; i += 146) {
    if (!s_image_read_line(image, i, row)) {
      fprintf(stderr,"failed to read line %d\n", i);
    }
  }

  fprintf(stderr,"testing normal: %s, %d %d %d\n", argv[1], w, h, nc);
  for (i = 0; i < h; i++) {
    if (!s_image_read_line(image, i, row)) {
      fprintf(stderr,"failed to read line %d\n", i);
    }
  }

  fprintf(stderr,"testing backwards: %s, %d %d %d\n", argv[1], w, h, nc);
  for (i = h-1; i >=0; i--) {
    if (!s_image_read_line(image, i, row)) {
      fprintf(stderr,"failed to read line %d\n", i);
    }
  }

  free(row);
  return 0;
}
