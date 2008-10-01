/* 
 * GDI+ provides support for the following image formats:
 *   bmp, gif, jpeg, exif, png, tiff, icon, wmf, emf
 * Only the raster images are used through usage of the Bitmap class.
 *
 * It amazes me everytime anew, whenever I get exposed to them, how
 * much a Microsoft API manages to suck stronger than the biggest
 * black hole known to mankind... 20060415 tamer.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <simage_gdiplus.h>

#include <windows.h>

/* MSVC6 fix for gdiplus.h */
#if defined(_MSC_VER) && (_MSC_VER == 1200) && !defined(ULONG_PTR)
#define ULONG_PTR ULONG /* (32bit build) */
#endif /* MSVC6 */
#include <gdiplus.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER
#define LOCKBITS_RECT_CAST(arg) &arg
#else // !GDIPVER
/* Old VC6 legacy download from codeproject.com with slightly
   different LockBits() signature (Rect by reference). */
#define LOCKBITS_RECT_CAST(arg) arg
#endif // !GDIPVER

enum { 
  ERR_NO_ERROR,
  ERR_OPEN,
  ERR_READ,
  ERR_MEM,
  ERR_OPEN_WRITE,
  ERR_WRITE,
  ERR_NOT_IMPLEMENTED,
  ERR_INIT
};

static int gdipluserror = ERR_NO_ERROR;

/*
 * Get the pixel format that should be used for reading the image.
 *
 * The component depth and component composition of simage images differ
 * from the pixel formats used by GDI+. This function maps a Bitmap to 
 * an appropriate PixelFormat that makes it easy for us to copy the
 * bitmap data returned by Bitmap::LockBits(...) to the simage image.
 *
 * The 'grayscale' flag is an output parameter that is used to indicate 
 * that even though the returned pixel format is PixelFormat32bppARGB, 
 * the image is really a gray scale image with alpha channel that can be
 * converted to a two-component simage image.
 */
static Gdiplus::PixelFormat
getReadFormat(Gdiplus::Bitmap & bitmap, bool & grayscale)
{
  grayscale = false;
  Gdiplus::PixelFormat format = bitmap.GetPixelFormat();

  if (Gdiplus::IsIndexedPixelFormat(format)) {
    INT palettesize = bitmap.GetPaletteSize();
    Gdiplus::ColorPalette * palette = 
      reinterpret_cast<Gdiplus::ColorPalette *>(new char[palettesize]);
    bitmap.GetPalette(palette, palettesize);
    bool alphaflag = (palette->Flags & Gdiplus::PaletteFlagsHasAlpha) != 0;
    bool grayflag = (palette->Flags & Gdiplus::PaletteFlagsGrayScale) != 0;
    delete[] reinterpret_cast<char *>(palette);

    if (!grayflag && !alphaflag)
      return PixelFormat24bppRGB;
    if (!grayflag && alphaflag)
      return PixelFormat32bppARGB;
    if (grayflag && !alphaflag)
      return PixelFormat16bppGrayScale;
    
    // since GDI+ doesn't define a non-indexed color format for 
    // grayscale images with alpha channel, we must extract the 
    // image as ARGB, and convert it to simage's 0xGGAA - format 
    // afterwards.
    grayscale = true;
    return PixelFormat32bppARGB;
  }

  if (format == PixelFormat16bppGrayScale) {
    return PixelFormat16bppGrayScale;
  }

  if ((format == PixelFormat16bppRGB555) ||
      (format == PixelFormat16bppRGB565) ||
      (format == PixelFormat24bppRGB) ||
      (format == PixelFormat32bppRGB) ||
      (format == PixelFormat48bppRGB)) {
    return PixelFormat24bppRGB;
  }

  return PixelFormat32bppARGB;
}

/*
 * Crate a buffer and copy image data into the buffer.
 *
 * Only use this function for RGB or RGBA images, that is
 * (numcomponents == 3) || (numcomponents == 4). 
 *
 * The 'stride' parameter is the number of bytes between each
 * consecutive line in the input buffer.
 *
 * The image data is stored in the output buffer in simage 
 * representation: 0xRRGGBBAA and 0xRRGGBB.
 *
 * The function will return NULL if malloc fails to allocate
 * memory for the output buffer.
 */
static unsigned char *
copyImageBuffer(unsigned char * src, unsigned int width, 
                unsigned int height, 
                unsigned int numcomponents, unsigned int stride)
{
  assert(src);
  assert(stride >= (width * numcomponents));
  assert((numcomponents == 3) || (numcomponents == 4));
  if ((numcomponents != 3) && (numcomponents != 4)) { return NULL; }

  unsigned char * dst = 
    (unsigned char *)malloc(width * height * numcomponents);
  if (!dst) { return NULL; }
  
  /* The image must be flipped horizontally so we start writing from the
     end of the the destination buffer */
  dst += width * height * numcomponents;

  if (numcomponents == 3) {
    for (unsigned int y = 0; y < height; y++) {
      dst -= width * numcomponents;
      for (unsigned int x = 0; x < width; ++x) {
        /* GDI+ stores the image internally as BGR, 
           we store it as RGB */
        unsigned int offset = numcomponents * x;
        dst[offset] = src[offset + 2];
        dst[offset + 1] = src[offset + 1];
        dst[offset + 2] = src[offset];
      }
      src += stride;
    }
  } else {
    for (unsigned int y = 0; y < height; y++) {
      dst -= width * numcomponents;
      for (unsigned int x = 0; x < width; ++x) {
        /* GDI+ stores the image internally as BGRA, 
           we store it as RGBA */
        unsigned int offset = numcomponents * x;
        dst[offset] = src[offset + 2];
        dst[offset + 1] = src[offset + 1];
        dst[offset + 2] = src[offset];
        dst[offset + 3] = src[offset + 3];
      }
      src += stride;
    }
  }
  
  return dst;
}


/*
 * Crate a buffer and copy image data into the buffer.
 *
 * Only use this function for images that GDI+ represent as ARGB,
 * but simage can store as 0xGGAA.
 *
 * The 'stride' parameter is the number of bytes between each
 * consecutive line in the input buffer.
 *
 * The function will return NULL if malloc fails to allocate
 * memory for the output buffer.
 */
static unsigned char *
copy32bppGrayScaleBuffer(unsigned char * src, unsigned int width, 
                         unsigned int height, unsigned int stride)
{
  assert(src);
  assert(stride >= (width * 4));

  unsigned char * dst = (unsigned char *)malloc(width * height * 2);
  if (!dst) { return NULL; }

  /* The image must be flipped horizontally so we start writing from the
     end of the the destination buffer */
  dst += width * height * 2;

  for (unsigned int y = 0; y < height; y++) {
    dst -= width * 2;
    for (unsigned int x = 0; x < width * 2; 
         x += 2) {
      /* a GDI+ buffer stores the components internally in reverse order, 
         eg. ARGB is stored as BGRA. Extract the blue component as the 
         gray value and the alpha value.*/
      dst[x] = src[x];
      dst[x + 1] = src[x + 3];
    }
    src += stride;
  }
  
  return dst;
}

/*
 * Crate a buffer and copy image data into the buffer.
 *
 * Only use this function for images that GDI+ represent as 16-bits
 * gray scale. Simage stores these as 8-bits grayscale.
 *
 * The 'stride' parameter is the number of bytes between each
 * consecutive line in the input buffer.
 *
 * The function will return NULL if malloc fails to allocate
 * memory for the output buffer.
 */
static unsigned char *
copy16bppGrayScaleBuffer(unsigned char * src, unsigned int width,
                         unsigned int height, unsigned int stride)
{
  assert(stride >= (2 * width));

  unsigned char * dst = (unsigned char *)malloc(width * height);
  if (!dst) { return NULL; }

  /* The image must be flipped horizontally so we start writing from the
     end of the the destination buffer */
  dst += width * height;

  unsigned short * shortsrc = (unsigned short *)src;

  for (unsigned int y = 0; y < height; ++y) {
    dst -= width;
    for (unsigned int x = 0; x < width; ++x) {
      /* Compress the 2-byte value to a 1-byte value */
      dst[x] = (unsigned char)((shortsrc[x] / 65535.0) * 255.0);
    }
    shortsrc += stride;
  }
  
  return dst;
}

static int
gdiplus_init(void) 
{
  static int did_init = 0;

  if (!did_init) {
    /* initialize GDI+ */

    /* Windows 64-bit uses the LLP64 type model, where int and long
     * are 32-bit and a pointer is 64-bit. Most Unices use the LP64
     * where int is 32-bit and long and pointer are 64-bit. */
    /* FIXME: the following solution is a kludge. 20081001 tamer. */
#if defined(_WIN64)
    unsigned long long gdiplusToken = 0;
#else
    unsigned long gdiplusToken = 0;
#endif
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) == Gdiplus::Ok) {
      did_init = 1;
    }
  }

  return did_init;
}

int
simage_gdiplus_error(char * buffer, int buflen)
{
  switch (gdipluserror) {
  case ERR_INIT:
    strncpy(buffer, "GDI+ loader: Error initializing GDI+", buflen);    
    break;
  case ERR_OPEN:
    strncpy(buffer, "GDI+ loader: Error opening file", buflen);
    break;
  case ERR_READ:
    strncpy(buffer, "GDI+ loader: Error reading file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "GDI+ loader: Out of memory error", buflen);
    break;
  case ERR_OPEN_WRITE:
    strncpy(buffer, "GDI+ saver: Error opening file", buflen);
    break;
  case ERR_WRITE:
    strncpy(buffer, "GDI+ loader: Error writing file", buflen);
    break;
  case ERR_NOT_IMPLEMENTED:
    strncpy(buffer, "GDI+ loader: Feature not implemented", buflen);
    break;
  default:
    strncpy(buffer, "GDI+ loader: Interesting unknown error you got", buflen);
  }

  return gdipluserror;
}

int 
simage_gdiplus_identify(const char * ptr,
                        const unsigned char * header,
                        int headerlen)
{
  gdipluserror = ERR_NO_ERROR;
  if (!gdiplus_init()) { gdipluserror = ERR_INIT; return 0; }

  /* convert C string to wide char */
  wchar_t * filename = new wchar_t[strlen(ptr)+1];

  if (!filename) { return 0; }
  mbstowcs(filename, ptr, strlen(ptr)+1);
  
  Gdiplus::Bitmap bitmap(filename);

  if (bitmap.GetFlags() == Gdiplus::ImageFlagsNone) {
    delete filename; return 0;
  }
  
  delete filename;

  return 1;
}

unsigned char *
simage_gdiplus_load(const char * filename,
                    int * width,
                    int * height,
                    int * numcomponents)
{  
  Gdiplus::Bitmap * bitmap = 
    (Gdiplus::Bitmap*)simage_gdiplus_open(filename, width, height,
                                          numcomponents);
  if (!bitmap) { return NULL; }

  bool grayscale = false;
  Gdiplus::PixelFormat format = getReadFormat(*bitmap, grayscale);

  Gdiplus::BitmapData bitmapdata;
  Gdiplus::Rect rect(0, 0, *width, *height);

  Gdiplus::Status result = bitmap->LockBits(LOCKBITS_RECT_CAST(rect),
                                            Gdiplus::ImageLockModeRead,
                                            format, &bitmapdata);

  if (result != Gdiplus::Ok) {
    if (result == Gdiplus::OutOfMemory)
      gdipluserror = ERR_MEM;      
    else 
      gdipluserror = ERR_OPEN;
    bitmap->UnlockBits(&bitmapdata);
    delete bitmap;
    return NULL; 
  }

  unsigned char * src = (unsigned char *)bitmapdata.Scan0;

  unsigned char * dst = NULL;

  switch (format) {
  case PixelFormat16bppGrayScale:
    (*numcomponents) = 1;
    dst = copy16bppGrayScaleBuffer(src, (*width), (*height),
                                   bitmapdata.Stride);
    break;
  case PixelFormat24bppRGB:
    (*numcomponents) = 3;
    dst = copyImageBuffer(src, (*width), (*height),
                          (*numcomponents),
                          bitmapdata.Stride);
    break;
  case PixelFormat32bppARGB:
    if (grayscale) {
      (*numcomponents) = 2;
      dst = copy32bppGrayScaleBuffer(src, (*width), (*height),
                                     bitmapdata.Stride);
    } else {
      (*numcomponents) = 4;
      dst = copyImageBuffer(src, (*width), (*height),
                            (*numcomponents),
                            bitmapdata.Stride);
    }
    break;
  default:
    dst = NULL;
  }
  
  bitmap->UnlockBits(&bitmapdata);    
  delete bitmap;    

  if (!dst) {
    gdipluserror = ERR_MEM;
  }

  return dst;
}

char * 
simage_gdiplus_get_savers(void)
{
  gdipluserror = ERR_NO_ERROR;
  if (!gdiplus_init()) {
    gdipluserror = ERR_INIT;
    return NULL;
  }

  UINT num = 0, size = 0;
  Gdiplus::GetImageEncodersSize(&num, &size);

  Gdiplus::ImageCodecInfo * pImageCodecInfo = (Gdiplus::ImageCodecInfo *) malloc(size);

  Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

  char * formats = NULL;
  unsigned int formats_size = 0;
  unsigned int format_len = 0;

  for (unsigned int i = 0; i < num; i++) {
    /* convert wide char to C string */
    const wchar_t * w_format = pImageCodecInfo[i].FormatDescription;
    unsigned int w_format_len = wcslen(w_format);
    char * format = (char *)malloc(w_format_len+10);
    wcstombs(format, w_format, w_format_len);
    format[w_format_len] = '\0';
    format_len = strlen(format);

    formats_size += format_len + ((i == 0) ? 1 : 2);
    formats = (char *) realloc(formats, formats_size);

    /* make JPEG and TIFF strings 3 letter extensions */
    /* FIXME: too dirty. wash it! 20060418 tamer. */
    if (format_len == 4) {
      if (!strncmp(format, "JPEG", 4)) {
        format[2] = 'G'; format[3] = '\0';
      } else if (!strncmp(format, "TIFF", 4)) {
        format[3] = '\0';
      }
    }

    if (i == 0) {
      strncpy(formats, format, format_len + 1);
    } else {
      strncat(formats, ",", 2);
      strncat(formats, format, format_len + 1);
    }

    free(format);
  }

  free(pImageCodecInfo);

  return formats;
}

static int
GetEncoderClsid(const char * format, CLSID * pClsid)
{
  Gdiplus::ImageCodecInfo * pImageCodecInfo = NULL;
  unsigned int num = 0, size = 0;

  Gdiplus::GetImageEncodersSize(&num, &size);
  if (size == 0) { return -1; }
  
  pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
  if (!pImageCodecInfo) { return -1; }
  
  Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

  /* convert C string filename to wide char */
  wchar_t * format_wide = new wchar_t[strlen(format)+1];
  
  if (!format_wide) { return -1; }
  mbstowcs(format_wide, format, strlen(format)+1);

  for (unsigned int i = 0; i < num; i++) {
    if (wcsstr(pImageCodecInfo[i].MimeType, format_wide)) {
      *pClsid = pImageCodecInfo[i].Clsid;
      free(format_wide);
      free(pImageCodecInfo);
      return i;
    }
  }
  
  free(format_wide);
  free(pImageCodecInfo);

  return -1;
}

int 
simage_gdiplus_save(const char * filename,
                    const unsigned char * bytes,
                    int width,
                    int height,
                    int numcomponents,
                    const char * filetypeext)
{
  /* convert C string filename to wide char */
  wchar_t * filename_wide = new wchar_t[strlen(filename)+1];
  
  if (!filename_wide) { gdipluserror = ERR_WRITE; return 0; }
  mbstowcs(filename_wide, filename, strlen(filename)+1);

  Gdiplus::PixelFormat pixelFormat =
    ((numcomponents == 1) || (numcomponents == 3)) ?
    PixelFormat24bppRGB : PixelFormat32bppARGB;

  Gdiplus::Bitmap * bitmap = new Gdiplus::Bitmap(width, height, pixelFormat);
  Gdiplus::BitmapData * bitmapData = new Gdiplus::BitmapData;
  Gdiplus::Rect rect(0, 0, width, height);

  bitmap->LockBits(LOCKBITS_RECT_CAST(rect),
                   Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite,
                   pixelFormat, bitmapData);

  unsigned char * dst = ((unsigned char *)bitmapData->Scan0);
  const unsigned char * src = bytes + (width*height*numcomponents);

  int numcomp34 = ((numcomponents==1) || (numcomponents==3)) ? 3 : 4;
  unsigned int stride = bitmapData->Stride - width*numcomp34;
      
  switch (numcomponents) {
  case 1:
  case 2:
    {
      /* FIXME: code for 2 components case has not been tested. comp 1
         should rather be written out as PixelFormat8bppIndexed. comp 2? 
         20060420 tamer. */
      for (unsigned int y = 0; y < height; y++) {
        src -= width*numcomponents;
        for (unsigned int x = 0; x < width*numcomponents; x+=numcomponents) {
          *dst++ = src[x]; *dst++ = src[x]; *dst++ = src[x];
          /* greyscale-alpha buffer */
          if (numcomponents == 2) { *dst++ = src[x+1]; }
        }
        dst += stride;
      }
    }
    break;
  case 3:
  case 4:
    {
      for (unsigned int y = 0; y < height; y++) {
        src -= width*numcomponents;
        for (unsigned int x = 0; x < width*numcomponents; x+=numcomponents) {
          *dst++ = src[x+2]; *dst++ = src[x+1]; *dst++ = src[x];
          /* ARGB GDI+ buffer, internally really represented as BGRA */
          if (numcomponents == 4) { *dst++ = src[x+3]; }
        }
        dst += stride;
      }
    }
    break;
  default:
    gdipluserror = ERR_WRITE;
    delete bitmapData;
    delete bitmap;
    delete filename_wide;
    return 0;
  }

  bitmap->UnlockBits(bitmapData);

  int ret;
  CLSID imgClsid;

  /* handle special JPEG and TIFF naming cases */
  /* FIXME: too dirty. wash it! 20060418 tamer. */
  if (!strncmp(filetypeext, "jpg", 3)) {
    ret = GetEncoderClsid("jpeg", &imgClsid);
  } else if (!strncmp(filetypeext, "tif", 3)) {
    ret = GetEncoderClsid("tiff", &imgClsid);
  } else {
    ret = GetEncoderClsid(filetypeext, &imgClsid);
  }
    
  if (ret != -1) { 
    bitmap->Save(filename_wide, &imgClsid, NULL);
  }

  delete bitmapData;
  delete bitmap;
  delete filename_wide;

  return 1;
}

void * 
simage_gdiplus_open(const char * filename,
                    int * width,
                    int * height,
                    int * numcomponents)
{
  gdipluserror = ERR_NO_ERROR;

  if (!gdiplus_init()) { gdipluserror = ERR_INIT; return NULL; }

  /* convert C string filename to wide char */
  wchar_t * filename_wide = new wchar_t[strlen(filename)+1];

  if (!filename_wide) { gdipluserror = ERR_OPEN; return NULL; }
  mbstowcs(filename_wide, filename, strlen(filename)+1);
  
  Gdiplus::Bitmap * bitmap = new Gdiplus::Bitmap(filename_wide);

  if (bitmap && (bitmap->GetLastStatus() == Gdiplus::Ok)) {
    *width = bitmap->GetWidth();
    *height = bitmap->GetHeight();

    Gdiplus::PixelFormat pixelFormat = bitmap->GetPixelFormat();

    (*numcomponents) = (pixelFormat == PixelFormat8bppIndexed) ? 1 :
      (pixelFormat == PixelFormat24bppRGB) ? 3 : 4;

    /* Simply use PixelFormat24bppRGB for GIF images, so GDI+ handles
       them correctly and we don't have to write a verbose palette
       handling essay to gain proper GIF support. FIXME: a bit
       dirty. rinse it! 20060520 tamer. */
    GUID guid; bitmap->GetRawFormat(&guid);
    if (guid == Gdiplus::ImageFormatGIF) { (*numcomponents) = 4; }

    delete filename_wide;
    return bitmap;
  }

  if (bitmap) { delete bitmap; }
  delete filename_wide;

  gdipluserror = ERR_OPEN;
  return NULL;
}

void 
simage_gdiplus_close(void * opendata)
{
  Gdiplus::Bitmap * bitmap = (Gdiplus::Bitmap*)opendata;
  if (bitmap) { delete bitmap; }
}

int 
simage_gdiplus_read_line(void * opendata, int y, unsigned char * buf)
{
  Gdiplus::Bitmap * bitmap = (Gdiplus::Bitmap*)opendata;
  if (!bitmap && !buf) { gdipluserror = ERR_READ; return 0; }

  Gdiplus::PixelFormat pixelFormat = bitmap->GetPixelFormat();

  int width = bitmap->GetWidth();
  int height = bitmap->GetHeight();

  if (y < 0 || y > height) { gdipluserror = ERR_READ; return 0; }

  int numcomponents = (pixelFormat == PixelFormat8bppIndexed) ? 1 :
    (pixelFormat == PixelFormat24bppRGB) ? 3 : 4;

  /* Simply use PixelFormat24bppRGB for GIF images, so GDI+ handles
     them correctly and we don't have to write a verbose palette
     handling essay to gain proper GIF support. FIXME: a bit
     dirty. rinse it! 20060520 tamer. */
  GUID guid; bitmap->GetRawFormat(&guid);
  if (guid == Gdiplus::ImageFormatGIF) { numcomponents = 4; }
  
  /* default to RGBA conversion for other pixel formats */
  if (numcomponents == 4) { pixelFormat = PixelFormat32bppARGB; }
  
  Gdiplus::BitmapData bitmapData;
  Gdiplus::Rect rect(0, height-y-1, width, 1);
  
  bitmap->LockBits(LOCKBITS_RECT_CAST(rect), Gdiplus::ImageLockModeRead,
                   pixelFormat, &bitmapData);
  
  unsigned char * src = (unsigned char *)bitmapData.Scan0;

  switch (numcomponents) {
  case 1:
    memcpy(buf, src, width);
    break;
  case 3:
  case 4:
    {
      for (unsigned int x = 0; x < width*numcomponents; x+=numcomponents) {
        buf[x+2] = *src++; buf[x+1] = *src++; buf[x] = *src++;
        /* ARGB GDI+ buffer, internally represented as BGRA */
        if (numcomponents == 4) { buf[x+3] = *src++; }
      }
    }
    break;
  default:
    gdipluserror = ERR_READ;
    return 0;
  }

  bitmap->UnlockBits(&bitmapData);

  return 1;
}

/*
 * FIXME: unstable experimental region support API. Use it without
 * knowing why and a fiercely looking fluffy cow will cross your way
 * one day. The fluffy cow is gonna eat up all your code and will
 * happily moo the doomsday symphony while blocking your getaway
 * road. YA HAVE BEEN WARNED! 20060520 tamer.
 */
int 
simage_gdiplus_read_region(void * opendata,
                           int x, int y, int w, int h,
                           unsigned char * buf)
{
  Gdiplus::Bitmap * bitmap = (Gdiplus::Bitmap*)opendata;
  if (!bitmap && !buf) { gdipluserror = ERR_READ; return 0; }

  Gdiplus::PixelFormat pixelFormat = bitmap->GetPixelFormat();

  int width = bitmap->GetWidth();
  int height = bitmap->GetHeight();

  if (x < 0 || y < 0 || w <= 0 || h <= 0 ||
      (x+w > width) || (y+h > height)) { gdipluserror = ERR_READ; return 0; }

  int numcomponents = (pixelFormat == PixelFormat8bppIndexed) ? 1 :
    (pixelFormat == PixelFormat24bppRGB) ? 3 : 4;

  /* Simply use PixelFormat24bppRGB for GIF images, so GDI+ handles
     them correctly and we don't have to write a verbose palette
     handling essay to gain proper GIF support. FIXME: a bit
     dirty. rinse it! 20060520 tamer. */
  GUID guid; bitmap->GetRawFormat(&guid);
  if (guid == Gdiplus::ImageFormatGIF) { numcomponents = 4; }
  
  /* default to RGBA conversion for other pixel formats */
  if (numcomponents == 4) { pixelFormat = PixelFormat32bppARGB; }
  
  Gdiplus::BitmapData bitmapData;
  Gdiplus::Rect rect(x, y, w, h);

  bitmap->LockBits(LOCKBITS_RECT_CAST(rect), Gdiplus::ImageLockModeRead,
                   pixelFormat, &bitmapData);
  
  unsigned int stride = bitmapData.Stride - w*numcomponents;
  unsigned char * src = (unsigned char *)bitmapData.Scan0;
  unsigned char * dst = buf + w*h*numcomponents;

  switch (numcomponents) {
  case 1:
    {
      for (unsigned int i = 0; i < h; i++) {
        dst -= w;
        memcpy(dst, src, w);
        src += bitmapData.Stride;
      }
    }
    break;
  case 3:
  case 4:
    {
      for (unsigned int i = 0; i < h; i++) {
        dst -= w*numcomponents;
        for (unsigned int j = 0; j < w*numcomponents; j+=numcomponents) {
          dst[j+2] = *src++; dst[j+1] = *src++; dst[j] = *src++;
          /* ARGB GDI+ buffer, internally really represented as BGRA */
          if (numcomponents == 4) { dst[j+3] = *src++; }
        }
        src += stride;
      }
    }
    break;
  default:
    gdipluserror = ERR_READ;
    return 0;
  }

  bitmap->UnlockBits(&bitmapData);

  return 1;
}
