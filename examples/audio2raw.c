#include <simage.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  FILE *outfile;
  s_stream * stream;
  s_params * params;
  int count;
  int channels;
  int size;
  int readsize;
  char * buffer;

  if (argc != 3) {
    fprintf(stderr, "Usage: audio2raw <input file> <output file>\n");  
    return 1;
  };

  outfile = fopen(argv[2], "wb");
  if (outfile == NULL) {
    fprintf(stderr, "Error: Couldn't open file '%s' for writing\n", argv[2]);
    return 1;
  }

  stream = s_stream_open(argv[1], NULL);
  if (stream == NULL) {
    fprintf(stderr, "Error: Couldn't open file '%s' for reading\n", argv[1]);
    return 1;
  }

  params = s_stream_params(stream);

  channels = 0;
  if (params != NULL) {
    s_params_get(params,
                 "channels", S_INTEGER_PARAM_TYPE, &channels, NULL);
  }

  size = 10240;
  readsize = size;
  buffer = (char *)malloc(size);
  count = 0;

  printf("Writing a raw file with %d channels, 16 bits signed integers\n", channels);

  while (s_stream_get_buffer(stream, buffer, &readsize, NULL) && (readsize>0) ) {
    if (readsize >0) {
      fwrite(buffer, size, 1, outfile);
      if (count++%10==0)
        printf(".");
    }
    readsize = size;
  }

  printf(" done\n");

  s_stream_close(stream);
  s_stream_destroy(stream);
  fclose(outfile);

  free(buffer);

  return 0;
}
