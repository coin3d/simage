/*!
  GIF loader, using libungif.
  Based, in part, on source code found in libungif, gif2rgb.c
*/

#include <config.h>
#ifdef HAVE_GIFLIB

#include <simage_gif.h>
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

static void
decode_row(GifFileType * giffile, 
           unsigned char * buffer, 
           unsigned char * rowdata,
           int x, int y, int len, 
           int transparent)
{
  GifColorType * cmentry;
  ColorMapObject * colormap;
  int colormapsize;
  unsigned char col;
  unsigned char * ptr;
  y = giffile->SHeight - (y+1);
  ptr = buffer + (giffile->SWidth * y + x) * 4;

  colormap = (giffile->Image.ColorMap
              ? giffile->Image.ColorMap
              : giffile->SColorMap);
  colormapsize = colormap ? colormap->ColorCount : 255;
  
  while (len--) {
    col = *rowdata++;
    if (col >= colormapsize) col = 0; /* just in case */
    cmentry = colormap ? &colormap->Colors[col] : NULL;
    if (cmentry) {
      *ptr++ = cmentry->Red;
      *ptr++ = cmentry->Green;
      *ptr++ = cmentry->Blue;
    }
    else {
      *ptr++ = col;
      *ptr++ = col;
      *ptr++ = col;
    }
    *ptr++ = (col == transparent ? 0x00 : 0xff);
  }
}

unsigned char *
simage_gif_load(const char *filename,
                int *width_ret,
                int *height_ret,
                int *numComponents_ret)
{
  int i, j, n, row, col, width, height, extcode;
  unsigned char * rowdata;
  unsigned char * buffer, * ptr;
  unsigned char bg;
  int transparent;
  GifRecordType recordtype;
  GifByteType * extension;
  GifFileType * giffile;
  GifColorType * bgcol;
  
  /* The way an interlaced image should be read - offsets and jumps */
  int interlacedoffset[] = { 0, 4, 2, 1 }; 
  int interlacedjumps[] = { 8, 8, 4, 2 };    
  
  giffile = DGifOpenFileName(filename);
  if (!giffile) {
    giferror = ERR_OPEN;
    return NULL;
  }
  
  transparent = -1; /* no transparent color by default */
  
  n = giffile->SHeight * giffile->SWidth;
  buffer = (unsigned char*) malloc(n * 4);
  if (!buffer) {
    giferror = ERR_MEM;
    return NULL;
  }
  rowdata = (unsigned char*) malloc(giffile->SWidth);
  if (!rowdata) {
    giferror = ERR_MEM;
    free(buffer);
    return NULL;
  }
  
  bg = giffile->SBackGroundColor;
  if (giffile->SColorMap && bg < giffile->SColorMap->ColorCount) {
    bgcol = &giffile->SColorMap->Colors[bg];
  }
  else bgcol = NULL;  
  ptr = buffer;
  for (i = 0; i < n; i++) {
    if (bgcol) {
      *ptr++ = bgcol->Red;
      *ptr++ = bgcol->Green;
      *ptr++ = bgcol->Blue;
      *ptr++ = 0xff;
    }
    else {
      *ptr++ = 0x00;
      *ptr++ = 0x00;
      *ptr++ = 0x00;
      *ptr++ = 0xff;
    }
  }
 
  /* Scan the content of the GIF file and load the image(s) in: */
  do {
    if (DGifGetRecordType(giffile, &recordtype) == GIF_ERROR) {
      giferror = ERR_READ;
      free(buffer);
      free(rowdata);
      return NULL;
    }
    switch (recordtype) {
    case IMAGE_DESC_RECORD_TYPE:
      if (DGifGetImageDesc(giffile) == GIF_ERROR) {
        giferror = ERR_READ;
        free(buffer);
        free(rowdata);
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
        free(rowdata);
        return NULL;
      }
      if (giffile->Image.Interlace) {
        /* Need to perform 4 passes on the images: */
        for (i = 0; i < 4; i++) {
          for (j = row + interlacedoffset[i]; j < row + height;
               j += interlacedjumps[i]) {
            if (DGifGetLine(giffile, rowdata, width) == GIF_ERROR) {
              giferror = ERR_READ;
              free(buffer);
              free(rowdata);
              return NULL;
            }
            else decode_row(giffile, buffer, rowdata, col, j, width, transparent);
          }
        }
      }
      else {
        for (i = 0; i < height; i++, row++) {
          if (DGifGetLine(giffile, rowdata, width) == GIF_ERROR) {
            giferror = ERR_READ;
            free(buffer);
            free(rowdata);
            return NULL;
          }
          else decode_row(giffile, buffer, rowdata, col, row, width, transparent);
        }
      }
      break;
    case EXTENSION_RECORD_TYPE:
      /* Skip any extension blocks in file: */
      if (DGifGetExtension(giffile, &extcode, &extension) == GIF_ERROR) {
        giferror = ERR_READ;
        free(buffer);
        free(rowdata);
        return NULL;
      }
      /* transparent test from the gimp gif-plugin. Open Source rulez! */
      else if (extcode == 0xf9) { 
        if (extension[0] >= 4 && extension[1] & 0x1) transparent = extension[4];
        else transparent = -1;
      }
      while (extension != NULL) {
        if (DGifGetExtensionNext(giffile, &extension) == GIF_ERROR) {
          giferror = ERR_READ;
          free(buffer);
          free(rowdata);
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

  free(rowdata);
  *width_ret = giffile->SWidth;
  *height_ret = giffile->SHeight;
  *numComponents_ret = 4;
  DGifCloseFile(giffile);
  return buffer;
}

#endif /* HAVE_GIFLIB */
