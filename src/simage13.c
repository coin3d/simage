#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <simage_private.h>
#include <string.h>

unsigned char * simage_resize3d(unsigned char *src,
                                int width, int height,
                                int nc, int layers,
                                int newwidth, int newheight,
                                int newlayers)
{
  float sx, sy, sz, dx, dy, dz;
  int src_bpr, dest_bpr, src_bpl, dest_bpl, xstop, ystop, zstop;
  int x, y, z, offset, i;
  unsigned char *dest = 
    (unsigned char*) malloc(newwidth*newheight*nc*newlayers);
  
  dx = ((float)width)/((float)newwidth);
  dy = ((float)height)/((float)newheight);
  dz = ((float)layers)/((float)newlayers);
  src_bpr = width * nc;
  dest_bpr = newwidth * nc;
  src_bpl = src_bpr * height;
  dest_bpl = dest_bpr * newheight;
  
  zstop = newlayers * dest_bpl;
  ystop = dest_bpl;
  xstop = dest_bpr;
  sz = 0.0f;
  for (z = 0; z < zstop; z += dest_bpl) {
    sy = 0.0f;
    for (y = 0; y < ystop; y += dest_bpr) {
      sx = 0.0f;
      for (x = 0; x < xstop; x += nc) {
        offset = ((int)sz)*src_bpl + ((int)sy)*src_bpr + ((int)sx)*nc;
        for (i = 0; i < nc; i++) dest[x+y+z+i] = src[offset+i];
        sx += dx;
      }
      sy += dy;
    }
    sz += dz;
  }
  return dest;
}
