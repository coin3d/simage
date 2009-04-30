#include <simage_cgimage.h>

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#define ERR_NO_ERROR    0   // no error
#define ERR_LOAD        1   // could not load file

static int cgimageerror = ERR_NO_ERROR;

static CGImageSourceRef create_image_source(const char * file)
{
  CFStringRef cfname = CFStringCreateWithCString(kCFAllocatorDefault, 
                                                file, 
                                                kCFStringEncodingUTF8);
  assert(cfname);

  CFURLRef texture_url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                       cfname,
                                                       kCFURLPOSIXPathStyle,
                                                       false);
  assert(texture_url);
  
  CGImageSourceRef image_source = CGImageSourceCreateWithURL(texture_url, NULL);
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
  switch (cgimageerror) {
   case ERR_LOAD:
     strncpy(cstr, "CGImage loader: Error loading file", buflen);
     break;
  }
  int errval = cgimageerror;
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
  CGImageSourceRef image_source = create_image_source(file);
  if (!image_source) {
    cgimageerror = ERR_LOAD;
    return NULL;
  }

  CGImageRef image = CGImageSourceCreateImageAtIndex(image_source, 0, NULL);
  assert(image);
    
  int w = CGImageGetWidth(image);
  int h = CGImageGetHeight(image);
  int nc = CGImageGetBitsPerPixel(image)/CGImageGetBitsPerComponent(image);
  CGColorSpaceRef color_space;
  if (nc <= 2) {
    nc = 1;
    color_space = CGColorSpaceCreateDeviceGray();
  }
  else {
    nc = 4;
    color_space = CGColorSpaceCreateDeviceRGB();
  }
  assert(color_space);

  unsigned char * newpx = (unsigned char *)malloc(w * h * nc);
    
  CGContextRef context = CGBitmapContextCreate(newpx, w, h, 8, w * nc, color_space,
                                               (nc == 1) ? 
                                               kCGImageAlphaNone : 
                                               kCGImageAlphaPremultipliedLast);
  assert(context);
  // Flip Y axis
  CGContextScaleCTM(context, 1.0f, -1.0f);
  CGContextTranslateCTM(context, 0.0f, -h);
 
  CGContextDrawImage(context, CGRectMake(0, 0, w, h), image);

  CFRelease(context);
  CFRelease(color_space);
  CFRelease(image);
  CFRelease(image_source);

  *width = w;
  *height = h;
  *numcomponents = nc;

  return newpx;
}
