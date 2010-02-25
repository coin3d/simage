#include <simage_cgimage.h>

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

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

static int cgimageerror = ERR_NO_ERROR;

static CGImageSourceRef
create_image_source(const char * file)
{
  CFStringRef cfname;
  CFURLRef image_url;
  CGImageSourceRef image_source;

  cfname = CFStringCreateWithCString(kCFAllocatorDefault,
				     file,
				     kCFStringEncodingUTF8);

  if (!cfname) {
    return NULL;
  }

  image_url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
					    cfname,
					    kCFURLPOSIXPathStyle,
					    false);

  if (!image_url) {
    CFRelease(cfname);
    return NULL;
  }

  image_source = CGImageSourceCreateWithURL(image_url, NULL);
  CFRelease(image_url);
  CFRelease(cfname);

  if (image_source &&
      (CGImageSourceGetStatus(image_source) != kCGImageStatusComplete ||
       CGImageSourceGetCount(image_source) == 0)) {
    CFRelease(image_source);
    image_source = NULL;
  }
  return image_source;
}

int
simage_cgimage_error(char * cstr, int buflen)
{
  int errval = cgimageerror;
  switch (cgimageerror) {
  case ERR_OPEN:
    strncpy(cstr, "CGImage loader: Error loading file", buflen);
    break;
  }
  cgimageerror = ERR_NO_ERROR;
  return errval;
}

int
simage_cgimage_identify(const char * file, const unsigned char * header,
			int headerlen)
{
  CGImageSourceRef image_source = create_image_source(file);
  if (image_source) {
    CFRelease(image_source);
    return 1;
  }
  return 0;
}

unsigned char *
simage_cgimage_load(const char * file, int * width, int * height, int * numcomponents)
{
  CGColorSpaceRef color_space;
  CGImageSourceRef image_source;
  CGImageRef image;
  CGContextRef context;
  unsigned char * newpx;

  image_source = create_image_source(file);
  if (!image_source) {
    cgimageerror = ERR_OPEN;
    return NULL;
  }

  image = CGImageSourceCreateImageAtIndex(image_source, 0, NULL);
  assert(image);

  *width = CGImageGetWidth(image);
  *height = CGImageGetHeight(image);
  *numcomponents = CGImageGetBitsPerPixel(image)/CGImageGetBitsPerComponent(image);

  if (*numcomponents <= 2) {
    *numcomponents = 1;
    color_space = CGColorSpaceCreateDeviceGray();
  }
  else {
    *numcomponents = 4;
    color_space = CGColorSpaceCreateDeviceRGB();
  }
  assert(color_space);

  newpx = (unsigned char *)malloc(*width * *height * *numcomponents);

  context = CGBitmapContextCreate(newpx, *width, *height, 8, *width * *numcomponents,
				  color_space,
				  (*numcomponents == 1) ?
				  kCGImageAlphaNone :
				  kCGImageAlphaPremultipliedLast);
  assert(context);
  /* flip Y axis */
  CGContextScaleCTM(context, 1.0f, -1.0f);
  CGContextTranslateCTM(context, 0.0f, -*height);

  CGContextDrawImage(context, CGRectMake(0, 0, *width, *height), image);

  CFRelease(context);
  CFRelease(color_space);
  CFRelease(image);
  CFRelease(image_source);

  return newpx;
}

char *
simage_cgimage_get_savers(void)
{
  unsigned int idx;
  unsigned int formats_size = 0;
  char * format;
  char * formats = NULL;

  CFArrayRef destinationTypes;
  CFIndex fileext_len = 0;
  CFStringRef fileext;
  CFStringRef jpegStr;
  CFStringRef tiffStr;

  format = (char*)malloc(5);
  memset(format, 0, 5);

  jpegStr = CFSTR("jpeg");
  tiffStr = CFSTR("tiff");

  destinationTypes = CGImageDestinationCopyTypeIdentifiers();
  for (idx = 0; idx < CFArrayGetCount(destinationTypes); idx++) {
    fileext = UTTypeCopyPreferredTagWithClass(CFArrayGetValueAtIndex(destinationTypes, idx),
					      kUTTagClassFilenameExtension);

    fileext_len = CFStringGetLength(fileext);

    /* make JPEG and TIFF strings 3 letter extensions */
    /* FIXME: dirty. wash it! 20100224 tamer. */
    if (fileext_len == 4) {
      if (!CFStringCompare(fileext, jpegStr, 0)) {
	CFRelease(fileext);
	fileext = CFSTR("jpg");
	fileext_len = 3;
      } else if (!CFStringCompare(fileext, tiffStr, 0)) {
	CFRelease(fileext);
	fileext = CFSTR("tif");
	fileext_len = 3;
      }
    }

    formats_size += fileext_len + ((idx == 0) ? 1 : 2);
    formats = (char *)realloc(formats, formats_size);

    /* Assumption: file extension tag not bigger than 4 characters,
       therefore the hardcoded value of 5. 20100224 tamer. */
    CFStringGetCString(fileext, format, 5, kCFStringEncodingUTF8);

    if (idx == 0) {
      strncpy(formats, format, fileext_len+1);
    } else {
      strncat(formats, ",", 2);
      strncat(formats, format, fileext_len+1);
    }

    CFRelease(fileext);
  }

  CFRelease(destinationTypes);
  free(format);

  return formats;
}

int
simage_cgimage_save(const char *filename,
		    const unsigned char * bytes,
		    int width,
		    int height,
		    int numcomponents,
		    const char * ext)
{
  CFStringRef cfname;
  CFStringRef file_ext;
  CFStringRef type_name;
  CFURLRef image_url;
  CGImageRef image_source;
  CGImageDestinationRef image_dest;
  CGColorSpaceRef color_space;
  CGDataProviderRef provider;

  int bitsPerComponent, bitsPerPixel, bytesPerRow;
  int finalized;

  unsigned int pos = 0, imgbufsize = width*height*numcomponents;
  unsigned char * bytes_flipped = NULL;

  /* FIXME: Flipping manually as CGBitmapContexts leads to headaches
     by not supporting 24bits per pixel buffers. 20100225 tamer. */
  bytes_flipped = (unsigned char *)malloc(imgbufsize);
  /* flip buffer horizontally */
  for (pos = 0; pos < imgbufsize; pos+=width*numcomponents) {
    memcpy(bytes_flipped+pos,
	   bytes+imgbufsize-width*numcomponents-pos,
	   width*numcomponents);
  }

  provider = CGDataProviderCreateWithData(NULL, bytes_flipped,
					  imgbufsize,
					  NULL);

  bitsPerComponent = 8;
  bitsPerPixel = bitsPerComponent*numcomponents;
  bytesPerRow = numcomponents * width;
  color_space = CGColorSpaceCreateDeviceRGB();
  image_source = CGImageCreate(width, height, 8, 8*numcomponents,
			       numcomponents*width,
			       color_space,
			       kCGBitmapByteOrderDefault,
			       provider,
			       NULL, 0,
			       kCGRenderingIntentDefault);

  CGDataProviderRelease(provider);
  CGColorSpaceRelease(color_space);
  free(bytes_flipped);

  file_ext = CFStringCreateWithCString(kCFAllocatorDefault,
				       ext,
				       kCFStringEncodingUTF8);
  type_name = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension,
						    file_ext,
						    kUTTypeImage);

  CFRelease(file_ext);

  if (!type_name) {
    cgimageerror = ERR_WRITE;
    CFRelease(image_source);
    return 0;
  }

  cfname = CFStringCreateWithCString(kCFAllocatorDefault,
				     filename,
				     kCFStringEncodingUTF8);

  if (!cfname) {
    cgimageerror = ERR_WRITE;
    CFRelease(type_name);
    CFRelease(image_source);
    return 0;
  }

  image_url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
					    cfname,
					    kCFURLPOSIXPathStyle,
					    false);

  CFRelease(cfname);

  if (!image_url) {
    cgimageerror = ERR_WRITE;
    CFRelease(type_name);
    CFRelease(image_source);
    return 0;
  }

  image_dest = CGImageDestinationCreateWithURL(image_url, type_name, 1, NULL);

  CFRelease(type_name);
  CFRelease(image_url);

  if (!image_dest) {
    cgimageerror = ERR_WRITE;
    CFRelease(image_source);
    return 0;
  }

  CGImageDestinationAddImage(image_dest, image_source, NULL);

  finalized = CGImageDestinationFinalize(image_dest);
  if (!finalized) {
    cgimageerror = ERR_WRITE;
  }

  CGImageRelease(image_source);
  CFRelease(image_dest);

  return finalized;
}
