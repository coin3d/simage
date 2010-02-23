#include <simage_cgimage.h>

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#define ERR_NO_ERROR    0   // no error
#define ERR_LOAD        1   // could not load file

static int cgimageerror = ERR_NO_ERROR;

static CGImageSourceRef create_image_source(const char * file)
{
  CFStringRef cfname;
  CFURLRef image_url;
  CGImageSourceRef image_source;

  cfname = CFStringCreateWithCString(kCFAllocatorDefault,
				     file,
				     kCFStringEncodingUTF8);
  assert(cfname);

  image_url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
					    cfname,
					    kCFURLPOSIXPathStyle,
					    false);
  assert(texture_url);

  image_source = CGImageSourceCreateWithURL(image_url, NULL);
  CFRelease(texture_url);
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
  case ERR_LOAD:
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
    cgimageerror = ERR_LOAD;
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

  context = CGBitmapContextCreate(newpx, *width, *height, 8, *width * *numcomponents, color_space,
				  (*numcomponents == 1) ?
				  kCGImageAlphaNone :
				  kCGImageAlphaPremultipliedLast);
  assert(context);
  // Flip Y axis
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

}

int
simage_cgimage_save(const char *filename,
		    const unsigned char * bytes,
		    int width,
		    int height,
		    int numcomponents,
		    const char * ext)
{

}
