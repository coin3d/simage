/* 
 * GDI+ provides support for the following image formats:
 *   bmp, gif, jpeg, exif, png, tiff, icon, wmf, emf
 * Only the raster images are used through usage of the Bitmap class.
 *
 * It amazes me everytime anew, whenever I get exposed to them, how
 * much a Microsoft API manages to suck stronger than the biggest
 * black hole known to mankind... 20060415 tamer.
 */

#include <simage_gdiplus.h>

#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

static int gdiplus_init(void) 
{
  static int did_init = 0;

  if (!did_init) {
    /* initialize GDI+ */
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    unsigned long gdiplusToken;
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
    strncpy(buffer, "GDI+ loader: Interesting error you got", buflen);
  }
  return gdipluserror;
}

int 
simage_gdiplus_identify(const char * ptr,
			const unsigned char * header,
			int headerlen)
{
  gdipluserror = ERR_NO_ERROR;

  if (!gdiplus_init()) {
    gdipluserror = ERR_INIT;
    return 0;
  }

  /* convert C string to wide char */
  wchar_t * filename = new wchar_t[strlen(ptr)+1];

  if (!filename) { return 0; }
  mbstowcs(filename, ptr, strlen(ptr)+1);
  
  Gdiplus::Bitmap bitmap(filename);

  if (bitmap.GetFlags() == Gdiplus::ImageFlagsNone) {
    delete filename;
    return 0;
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
  /* convert C string filename to wide char */
  wchar_t * filename_wide = new wchar_t[strlen(filename)+1];

  if (!filename_wide) { return NULL; }
  mbstowcs(filename_wide, filename, strlen(filename)+1);

  gdipluserror = ERR_NO_ERROR;

  if (!gdiplus_init()) {
    gdipluserror = ERR_INIT;
    return NULL;
  }
  
  Gdiplus::Bitmap * bitmap = new Gdiplus::Bitmap(filename_wide);
  
  if (bitmap && (bitmap->GetLastStatus() == Gdiplus::Ok)) {
    *width = bitmap->GetWidth();
    *height = bitmap->GetHeight();

    Gdiplus::PixelFormat pixelFormat = bitmap->GetPixelFormat();

    (*numcomponents) = (pixelFormat == PixelFormat8bppIndexed) ? 1 :
      (pixelFormat == PixelFormat24bppRGB) ? 3 : 4;
    /* default to RGBA conversion for other pixel formats */
    if ((*numcomponents) == 4) { pixelFormat = PixelFormat32bppARGB; }
  
    Gdiplus::BitmapData * bitmapData = new Gdiplus::BitmapData;
    Gdiplus::Rect rect(0, 0, *width, *height);
    
    bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
		     pixelFormat, bitmapData);

    unsigned int stride = bitmapData->Stride - (*width)*(*numcomponents);
    unsigned char * src = (unsigned char *)bitmapData->Scan0;
    unsigned char * buffer = (unsigned char *)malloc((*width)*(*height)*(*numcomponents));
    unsigned char * dst = buffer + (*width)*(*height)*(*numcomponents) - (*numcomponents);

    if (!buffer) {
      gdipluserror = ERR_MEM;
      return NULL;
    }

    switch ((*numcomponents)) {
    case 1:
      for (unsigned int y = 0; y < (*height); y++) {
	for (unsigned int x = 0; x < (*width); x++) {
	  dst[0] = src[0];
 	  src += (*numcomponents);
 	  dst -= (*numcomponents);
	}
	src += stride;
      }
      break;
    case 3:
    case 4:
      for (unsigned int y = 0; y < (*height); y++) {
	for (unsigned int x = 0; x < (*width); x++) {
	  dst[0] = src[2]; dst[1] = src[1]; dst[2] = src[0];
	  /* ARGB GDI+ buffer */
	  if ((*numcomponents) == 4) { dst[3] = src[3]; }
 	  src += (*numcomponents);
 	  dst -= (*numcomponents);
	}
	src += stride;
      }
      break;
    default:
      gdipluserror = ERR_OPEN;
      delete bitmapData;
      delete bitmap;
      delete filename_wide;      
      return NULL;
    }

    bitmap->UnlockBits(bitmapData);
    
    delete bitmapData;
    delete bitmap;
    delete filename_wide;
    
    return buffer;
  }

  if (bitmap) { delete bitmap; }
  delete filename_wide;

  gdipluserror = ERR_OPEN;
  return NULL;
}

char * 
simage_gdiplus_get_savers(void)
{
  unsigned int num, size;

  Gdiplus::ImageCodecInfo * pImageCodecInfo;

  Gdiplus::GetImageDecodersSize(&num, &size);
  pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

  Gdiplus::GetImageDecoders(num, size, pImageCodecInfo);

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
    formats = (char *)realloc(formats, formats_size);

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
      strncpy(formats, format, format_len);
    } else {
      strncat(formats, ",", 1);
      strncat(formats, format, format_len);
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
  
  if (!filename_wide) { return NULL; }
  mbstowcs(filename_wide, filename, strlen(filename)+1);

  Gdiplus::PixelFormat pixelFormat =
    ((numcomponents == 1) || (numcomponents == 3)) ?
    PixelFormat24bppRGB : PixelFormat32bppARGB;

  Gdiplus::Bitmap * bitmap = new Gdiplus::Bitmap(width, height, pixelFormat);
  Gdiplus::BitmapData  * bitmapData = new Gdiplus::BitmapData;
  Gdiplus::Rect rect(0, 0, width, height);

  bitmap->LockBits(&rect,
		   Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite,
		   pixelFormat, bitmapData);

  unsigned char * dst = ((unsigned char *)bitmapData->Scan0);
  const unsigned char * src = bytes + (width*height*numcomponents) - numcomponents;

  int numcomp34 = ((numcomponents==1) || (numcomponents==3)) ? 3 : 4;
  unsigned int stride = bitmapData->Stride - width*numcomp34;
      
  switch (numcomponents) {
  case 1:
  case 2:
    /* FIXME: code for 2 components case has not been ested. comp 1
       should be rather written out as PixelFormat8bppIndexed. comp 2? 
       20060420 tamer. */
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
	*dst++ = src[0]; *dst++ = src[0]; *dst++ = src[0];
	/* ARGB GDI+ buffer */
	if (numcomponents == 2) { *dst++ = src[1]; }
	src -= numcomponents;
      }
      dst += stride;
    }
    break;
  case 3:
  case 4:
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
	*dst++ = src[2]; *dst++ = src[1]; *dst++ = src[0];
	/* ARGB GDI+ buffer */
	if (numcomponents == 4) { *dst++ = src[3]; }
	src -= numcomponents;
      }
      dst += stride;
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
  CLSID pngClsid;

  /* handle special JPEG and TIFF naming cases */
  /* FIXME: too dirty. wash it! 20060418 tamer. */
  if (!strncmp(filetypeext, "jpg", 3)) {
    ret = GetEncoderClsid("jpeg", &pngClsid);
  } else if (!strncmp(filetypeext, "tif", 3)) {
    ret = GetEncoderClsid("tiff", &pngClsid);
  } else {
    ret = GetEncoderClsid(filetypeext, &pngClsid);
  }
    
  if (ret != -1) { 
    bitmap->Save(filename_wide, &pngClsid, NULL);
  }

  delete bitmapData;
  delete bitmap;
  delete filename_wide;

  return 1;
}

typedef struct {
  int width;
  int height;
  int numcomp;
  int depth;  
} simage_gdiplus_opendata;

void * 
simage_gdiplus_open(const char * filename,
                   int * width,
                   int * height,
                   int * numcomponents)
{
  gdipluserror = ERR_NOT_IMPLEMENTED;
  return NULL;
}


void 
simage_gdiplus_close(void * opendata)
{
  simage_gdiplus_opendata * od = (simage_gdiplus_opendata*) opendata;
  gdipluserror = ERR_NOT_IMPLEMENTED;
}

int 
simage_gdiplus_read_line(void * opendata, int y, unsigned char * buf)
{
  simage_gdiplus_opendata * od;
  gdipluserror = ERR_NOT_IMPLEMENTED;
   
  od = (simage_gdiplus_opendata*)opendata;
  return 0;
}
