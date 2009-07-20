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
  fprintf(stderr, "Usage:\n %s <infile> <outfile> [-newsize <x> <y>] [-scale <xmul> <ymul>] [-alphathreshold <val>] [-addalpha <filename>] [-setundef]\n",argv0);
}

int main(int argc, char ** argv)
{
  int i;
  int w, h, nc;
  float xmul, ymul;
  int neww, newh;
  unsigned char * buf;
  char * infile, * outfile;
  char * addalpha = NULL;
  const char * ext;
  int ret;
  int alphathreshold = -1;
  int gray = 0;
  int setundef = 0;
  int mirrory = 0;
  int save = 1;

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
      else if (strcmp(argv[i], "-addalpha") == 0 && i < argc-1) {
        i++;
        addalpha = argv[i++];
      }
      else if (strcmp(argv[i], "-gray") == 0) {
        gray = 1;
        i++;
      }
      else if (strcmp(argv[i], "-mirrory") == 0) {
        mirrory = 1;
        i++;
      }
      else if (strcmp(argv[i], "-setundef") == 0) {
        setundef = 1;
        i++;
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
        fprintf(stderr,"Extension '%s' not supported for writing by simage\n",
                ext);
        save = 0;
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

  if (addalpha) {
    int x, y, c;
    int ac;
    int w2, h2, nc2;
    unsigned char * buf2 = simage_read_image(addalpha, &w2, &h2, &nc2);
    fprintf(stderr,"alpha size: %d %d %d\n", w2, h2, nc2);
    if (buf2) {
      if (w == w2 && h == h2 && (nc2 == 1 || nc2 == 2 || nc2 == 4)) {
        unsigned char * dstbuf = buf;
        if (nc == 1 || nc == 3) {
          fprintf(stderr,"adding alpha channel\n");
          dstbuf = (unsigned char*) malloc(w*h*(nc+1));
          for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
              for (c = 0; c < nc; c++) {
                dstbuf[y*w*(nc+1) + x*(nc+1) + c] = buf[y*w*nc + x*nc + c];
              }
            }
          }
          nc++; // alpha channel added
        }
        ac = 0;
        if (nc2 == 2) ac = 1;
        if (nc2 == 4) ac = 3;

        fprintf(stderr,"merging in alpha channel\n");
        for (y = 0; y < h; y++) {
          for (x = 0; x < w; x++) {
            dstbuf[y*w*nc + x*nc + (nc-1)] = buf2[y*w*nc2 + x * nc2 + ac];
          }
        }
        if (dstbuf != buf) {
          free(buf);
          buf = dstbuf;
        }
      }
      else {
        fprintf(stderr,"Unable to add alpha channel\n");
      }
    }
    else {
      fprintf(stderr,"Unable to read alpha file: %s\n", addalpha);
    }
  }

/*   if (setundef && nc == 3) { */

/*   } */

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

  if (gray && nc >= 3) {
    unsigned char * src = buf;
    unsigned char * dst = buf;
    
    for (i = 0; i < w*h; i++) {
      *dst++ = (unsigned char) ((src[0]*77 + src[1]*150 + src[2] * 28) >> 8);
      src += 3;
      if (nc == 4) *dst++ = *src++;
    }
    nc = nc == 3 ? 1 : 2;
  }

  if (setundef && ((nc == 2) || (nc == 4))) {
    int cnt = 0;
    unsigned char * p = buf;
    const int n = w * h;
    for (i = 0; i < n; i++) {
      if (nc == 2) {
        if (p[1] == 0) {
          p[0] = 0;
          cnt++;
        }
      }
      else {
        if (p[3] == 0) {
          p[0] = p[1] = p[2] = 0;
          cnt++;
        }
      }
      p += nc;
    }
    fprintf(stderr,"undef'ed %d pixels\n", cnt);
  }
  if (mirrory) {
    unsigned char * tmp = (unsigned char*) malloc(w*nc);
    int y;
    for (y = 0; y < h/2; y++) {
      unsigned char * src1 = buf + w*nc*y;
      unsigned char * src2 = buf + w*nc*(h-1-y);
      memcpy(tmp, src1, w*nc);
      memcpy(src1, src2, w*nc);
      memcpy(src2, tmp, w*nc);
    }
    free(tmp);
  }

  if (save) {
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
  }
  return 0;
}
