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
  int offset;
  char offsetstr[40];
  char * buffer;

  if ( (argc != 3) && (argc != 4) ) {
    fprintf(stderr, "Usage: audio2raw <input file> <output file> "
                    "[sample offset]\n");  
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

  offset = -1;
  if (argc == 4)
    offset = atoi(argv[3]);

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

  sprintf(offsetstr, ", starting at sample %d\n", offset);

  printf("Writing a raw file with %d channels, 16 bits signed integers %s", 
         channels, (offset != -1) ? offsetstr : "\n");

  if (offset != -1) {
    int pos = s_stream_seek(stream, offset, SIMAGE_SEEK_SET, NULL);
    if (pos == -1)
      printf("Error: Seeking failed\n");
    else
      printf("Successfully seeked to position %d\n", pos);
  }

  while (s_stream_get_buffer(stream, buffer, &readsize, NULL) && 
         (readsize>0) ) {
    if (readsize >0) {
      fwrite(buffer, readsize, 1, outfile);
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
