/*!
  GIF loader, using libungif.
  Based, in part, on source code found in libungif, gif2rgb.c

  TODO:
  - figure out when background should be set to transparent...
*/
#include "simage_gif.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gif_lib.h>

#define ERR_NO_ERROR     0
#define ERR_OPEN         1
#define ERR_READ         2
#define ERR_MEM          3

#define MY_GIF_DEBUG 1

static int giferror = ERR_NO_ERROR;

int
simage_gif_error(char * buffer, int buflen)
{
  switch (giferror) {
  case ERR_OPEN:
    strncpy(buffer, "GIF loader: Error opening file", buflen);
    break;
  case ERR_READ:
    strncpy(buffer, "GIF loader: Error reading file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "GIF loader: Out of memory error", buflen);
    break;
  }
  return giferror;
}

int 
simage_gif_identify(const char *filename,
                    const unsigned char *header,
                    int headerlen)
{
  static unsigned char gifcmp[] = {'G', 'I', 'F'};
  if (headerlen < 3) return 0;
  if (memcmp((const void*)header, 
	     (const void*)gifcmp, 3) == 0) return 1;
  return 0;
}

unsigned char *
simage_gif_load(const char *filename,
                int *width_ret,
                int *height_ret,
                int *numComponents_ret)
{
  int i, j, n, row, col, width, height, extcode, count, colormapsize;
  unsigned char * image;
  unsigned char * buffer, * ptr;
  unsigned char bg;
  GifRecordType recordtype;
  GifByteType * extension;
  GifFileType * giffile;
  GifColorType * cmentry;
  ColorMapObject * colormap;
  
  /* The way an interlaced image should be read - offsets and jumps */
  int interlacedoffset[] = { 0, 4, 2, 1 }; 
  int interlacedjumps[] = { 8, 8, 4, 2 };    
  
  giffile = DGifOpenFileName(filename);
  if (!giffile) {
    giferror = ERR_OPEN;
    return NULL;
  }
  
  n = giffile->SHeight * giffile->SWidth;
  buffer = malloc(n * 3);
  if (!buffer) {
    giferror = ERR_MEM;
    return NULL;
  }
  
  /* use last part of buffer to store paletted image */
  image = buffer + 2 * n; 

  /* set all pixels to the background color */
  bg = giffile->SBackGroundColor;
  for (i = 0; i < n; i++) image[i] = bg;
  
  /* Scan the content of the GIF file and load the image(s) in: */
  do {
    if (DGifGetRecordType(giffile, &recordtype) == GIF_ERROR) {
      giferror = ERR_READ;
      free(buffer);
      return NULL;
    }
    switch (recordtype) {
    case IMAGE_DESC_RECORD_TYPE:
      if (DGifGetImageDesc(giffile) == GIF_ERROR) {
        giferror = ERR_READ;
        free(buffer);
        return NULL;
      }
      row = giffile->Image.Top; /* subimage position in composite image */
      col = giffile->Image.Left;
      width = giffile->Image.Width;
      height = giffile->Image.Height;
      if (giffile->Image.Left + giffile->Image.Width > giffile->SWidth ||
          giffile->Image.Top + giffile->Image.Height > giffile->SHeight) {
        /* image is not confined to screen dimension */
        giferror = ERR_READ;
        free(buffer);
        return NULL;
      }
      if (giffile->Image.Interlace) {
        /* Need to perform 4 passes on the images: */
        for (count = i = 0; i < 4; i++) {
          for (j = row + interlacedoffset[i]; j < row + height;
               j += interlacedjumps[i]) {
            if (DGifGetLine(giffile, image + (j*giffile->SWidth) + col,
                            width) == GIF_ERROR) {
              giferror = ERR_READ;
              free(buffer);
              return NULL;
            }
          }
        }
      }
      else {
        for (i = 0; i < height; i++) {
          if (DGifGetLine(giffile, image + (row++ * giffile->SWidth) + col,
                          width) == GIF_ERROR) {
            giferror = ERR_READ;
            free(buffer);
            return NULL;
          }
        }
      }
      break;
    case EXTENSION_RECORD_TYPE:
      /* Skip any extension blocks in file: */
      if (DGifGetExtension(giffile, &extcode, &extension) == GIF_ERROR) {
        giferror = ERR_READ;
        free(buffer);
        return NULL;
      }
      while (extension != NULL) {
        if (DGifGetExtensionNext(giffile, &extension) == GIF_ERROR) {
          giferror = ERR_READ;
          free(buffer);
          return NULL;
        }
      }
      break;
    case TERMINATE_RECORD_TYPE:
      break;
    default:		    /* Should be trapped by DGifGetRecordType. */
      break;
    }
  }
  while (recordtype != TERMINATE_RECORD_TYPE);

  /* whizz through image, and convert to RGB */
  colormap = (giffile->Image.ColorMap
              ? giffile->Image.ColorMap
              : giffile->SColorMap);
  colormapsize = colormap->ColorCount;

  ptr = buffer;
  for (i = 0; i < n; i++) {
    if (*image < colormapsize) /* just in case */
      cmentry = &colormap->Colors[*image++];
    else
      cmentry = &colormap->Colors[bg];
    *ptr++ = cmentry->Red;
    *ptr++ = cmentry->Green;
    *ptr++ = cmentry->Blue;
  }
  DGifCloseFile(giffile);
  *width_ret = giffile->SWidth;
  *height_ret = giffile->SHeight;
  *numComponents_ret = 3;
  return buffer;
}

