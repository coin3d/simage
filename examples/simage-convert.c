/*
 * simage-convert.c 
 *  a simple program that shows how to use simage, by pederb@sim.no
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
  fprintf(stderr, "Usage:\n %s <infile> <outfile> [-newsize <x> <y>] [-scale <xmul> <ymul>] [-alphathreshold <val>]\n",argv0);
}

int main(int argc, char ** argv)
{
  int i;
  int w, h, nc;
  float xmul, ymul;
  int neww, newh;
  unsigned char * buf;
  char * infile, * outfile;
  const char * ext;
  int ret;
  int alphathreshold = -1;

  if (argc < 3) {
    usage(argv[0]);
    return -1;
  }

  xmul = -1.0f;
  ymul = -1.0f;
  neww = -1;
  newh = -1;
  infile = NULL;
  outfile = NULL;

  i = 1;
  while (i < argc) {
    if (argv[i][0] != '-') {
      if (infile == NULL) infile = argv[i];
      else if (outfile == NULL) outfile = argv[i];
      else {
        usage(argv[0]);
        return -1;
      }
      i++;
    }
    else {
      if (strcmp(argv[i], "-newsize") == 0 && i < argc-2) {
        i++;
        neww = atoi(argv[i++]);
        newh = atoi(argv[i++]);
        if (neww <= 0 || newh <= 0) {
          usage(argv[0]);
          return -1;
        }
      }
      else if (strcmp(argv[i], "-scale") == 0 && i < argc-2) {
        i++;
        xmul = atof(argv[i++]);
        ymul = atof(argv[i++]);
        if (xmul <= 0.0f || ymul <= 0.0f) {
          usage(argv[0]);
          return -1;
        }
      }
      else if (strcmp(argv[i], "-alphathreshold") == 0 && i < argc-1) {
        i++;
        alphathreshold = atoi(argv[i++]);
      }
      else {
        usage(argv[0]);
        return -1;
      }
    }
  }

  if (outfile == NULL) {
    usage(argv[0]);
    return -1;
  }
  else {
    ext = strrchr(outfile, '.');
    if (ext == NULL) {
      fprintf(stderr,"outfile has no valid extension\n");
      return -1;
    }
    else {
      ext++; /* skip period */
      if (!simage_check_save_supported(ext)) {
        fprintf(stderr,"Extension '%s' not supported by simage\n",
                ext);
        return -1;
      }
    }
  }

  fprintf(stderr,"read image: %s...", infile);
  buf = simage_read_image(infile, &w, &h, &nc);
  if (buf) {
    fprintf(stderr,"done\n");
  }
  else {
    fprintf(stderr,"Unable to read file\n", argv[1]);
    return -1;
  }

  fprintf(stderr,"image size: (%d, %d), components: %d\n",
          w, h, nc);

  if (xmul > 0.0f) {
    neww = (int) (w * xmul); 
    newh = (int) (h * ymul);
  }
  if (neww > 0 && newh > 0 && (neww != w || newh != h)) {
    unsigned char * newbuf;

    fprintf(stderr,"resizing image: (%d, %d) to (%d %d)...",
            w, h, neww, newh);
    newbuf = simage_resize(buf, w, h, nc,
                           neww, newh);
    fprintf(stderr,"done\n");
    simage_free_image(buf);
    buf = newbuf;
    w = neww;
    h = newh;
  }
  if ((nc == 2 || nc == 4) && alphathreshold >= 0 && alphathreshold <= 255) {
    int cnt = 0;
    unsigned char * p = buf;
    const int n = w * h;
    for (i = 0; i < n; i++) {
      if (nc == 2) p++;
      else p += 3; 
      if (*p < alphathreshold) {
        if (*p != 0) {
          cnt++;
          *p = 0;
        }
      }
      else {
        if (*p != 255) {
          *p = 255;
          cnt++;
        }
      }
      p++;
    }
    fprintf(stderr,"alpha threshold changed %d pixels\n", cnt);
  }

  fprintf(stderr,"save image '%s'...", outfile);
  ret = simage_save_image(argv[2], buf, w, h, nc, ext);
  simage_free_image(buf);
  if (ret != 1) {
    fprintf(stderr,"error: %s\n", simage_get_last_error());
    return -1;
  }
  else {
    fprintf(stderr,"done\n");
  }
  return 0;
}
