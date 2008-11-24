/*
 * a jasper (Jpeg 2000) importer
 * 
 */

#include <config.h>
#ifdef HAVE_JASPER

#include <simage_jasper.h>

/* needed since Japser includes its own config file */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION

#include <jasper/jasper.h>

#define ERR_NO_ERROR        0
#define ERR_OPEN            1
#define ERR_READ            2
#define ERR_MEM             3
#define ERR_OPEN_WRITE      4
#define ERR_WRITE           5
#define ERR_NOT_IMPLEMENTED 6
#define ERR_INIT            7

static int jaspererror = ERR_NO_ERROR;

static int jasper_init(void) 
{
  static int did_init = 0;
  if (!did_init) {
    if (jas_init() == 0) {
      did_init = 1;
    }
  }
  return did_init;
}
static void jasper_copy_matrix(unsigned char * buffer, 
                               jas_matrix_t * data,
                               int w,
                               int h,
                               int bits,
                               int component,
                               int numcomp) 
{
  int x, y;
  unsigned char * dst = buffer + component;

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      int tmp = jas_matrix_get(data, (h-y)-1, x);
      tmp <<= 8;
      tmp >>= bits;

      *dst = (unsigned char) tmp;
      dst += numcomp;
    }
  }
}

int
simage_jasper_error(char * buffer, int buflen)
{
  switch (jaspererror) {
  case ERR_INIT:
    strncpy(buffer, "JASPER loader: Error initializing Jasper", buflen);    
    break;
  case ERR_OPEN:
    strncpy(buffer, "JASPER loader: Error opening file", buflen);
    break;
  case ERR_READ:
    strncpy(buffer, "JASPER loader: Error reading file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "JASPER loader: Out of memory error", buflen);
    break;
  case ERR_OPEN_WRITE:
    strncpy(buffer, "JASPER saver: Error opening file", buflen);
    break;
  case ERR_WRITE:
    strncpy(buffer, "JASPER loader: Error writing file", buflen);
    break;
  case ERR_NOT_IMPLEMENTED:
    strncpy(buffer, "JASPER loader: Feature not implemented", buflen);
    break;
  }
  return jaspererror;
}

int 
simage_jasper_identify(const char *ptr,
                       const unsigned char * header,
                       int headerlen)
{
  static unsigned char jaspercmp[] = {0x00, 0x00, 0x00, 0x0c, 0x6a, 0x50};

  
  if (headerlen < 6) return 0;
  if (memcmp((const void*)header, (const void*)jaspercmp, 6) == 0) return 1;
  return 0;
}

unsigned char *
simage_jasper_load(const char * filename,
                   int * width_ret,
                   int * height_ret,
                   int * numComponents_ret)
{
  int width;
  int height;
  int depth;
  int numcomps;
  int realnumcomp;
  int compno;
  int compfound[4] = {0,0,0,0};

        jas_image_t * image = NULL;
  jas_stream_t * stream = NULL;
        jas_matrix_t * data = NULL;
  unsigned char * buffer = NULL;

  jaspererror = ERR_NO_ERROR;
  if (!jasper_init()) {
    jaspererror = ERR_INIT;
    return NULL;
  }

  do {
    stream = jas_stream_fopen(filename, "rb");
    if (!stream) {
      jaspererror = ERR_OPEN;
      break; /* break out of do/while loop */
    }
    image = jas_image_decode(stream, -1, 0);
    jas_stream_close(stream);
    stream = NULL;

    if (!image) {
      jaspererror = ERR_READ;
      break; /* break out of do/while loop */
    }

    width = jas_image_width(image);
    height = jas_image_height(image);
    numcomps = jas_image_numcmpts(image);
    
    /*  
     * verify components 
     */
    for (compno = 0; compno < numcomps; compno++) {
      int w, h, d;
      int type;
      w = jas_image_cmptwidth(image, compno);
      h = jas_image_cmptheight(image, compno);
      type = jas_image_cmpttype(image, compno);
      d = jas_image_cmptprec(image, compno);
    
      if (w != width || h != height) {
        jaspererror = ERR_READ;
        break; /* break out of for loop  */
      }
      switch (type) {
      case JAS_IMAGE_CT_RGB_R:
        compfound[0] = 1;
        break;
      case JAS_IMAGE_CT_RGB_G:
        compfound[1] = 1;
        break;
      case JAS_IMAGE_CT_RGB_B:
        compfound[2] = 1;
        break;
      case JAS_IMAGE_CT_OPACITY:
        compfound[3] = 1;
        break;
      default:
        /* just ignore */
        break;
      }
    }
    if (jaspererror != ERR_NO_ERROR) {
      break; /* break out of do/while loop */
    }
    
    /*
     * try to figure out the actual number of components 
     */

    realnumcomp = 0;
    if (numcomps >= 3) { /* assume RGB image */
      if (compfound[0] && compfound[1] && compfound[2]) {
        realnumcomp = 3;
        if (compfound[3]) realnumcomp = 4;
      }
    }
    else { /* assume grayscale */
      if (compfound[0]) {
        realnumcomp = 1;
        if (compfound[3]) {
          realnumcomp = 2;
        }
      }
    }
    if (realnumcomp == 0) {
      jaspererror = ERR_READ;
      break; /* break out of do/while loop */
    }
    
    buffer = malloc(width * height * realnumcomp);
    if (buffer == NULL) {
      jaspererror = ERR_MEM;
      break; /* break out of do/while loop */
    }
    data = jas_matrix_create(height, width);
    if (!data) {
      jaspererror = ERR_MEM;
      break; /* break out of do/while loop */
    }
    
    for (compno = 0; compno < numcomps; compno++) {
      int d, type;
      int realcomp;
      type = jas_image_cmpttype(image, compno);
      d = jas_image_cmptprec(image, compno);
      
      if (jas_image_readcmpt(image, compno, 0, 0, width, height, data)) {
        jaspererror = ERR_READ;
        break; /* break out of for loop */
      }
      switch (type) {
      case JAS_IMAGE_CT_RGB_R:
        realcomp = 0;
        break;
      case JAS_IMAGE_CT_RGB_G:
        realcomp = 1;
        break;
      case JAS_IMAGE_CT_RGB_B:
        realcomp = 2;
        break;
      case JAS_IMAGE_CT_OPACITY:
        realcomp = realnumcomp - 1;
        break;
      default:
        /* just ignore */
        break;
      }  
      jasper_copy_matrix(buffer, data, width, height, d, realcomp, realnumcomp);
    }
  } while (0);

  if (stream) jas_stream_close(stream);
  if (image) jas_image_destroy(image);
  if (data) jas_matrix_destroy(data);
  
  if (jaspererror != ERR_NO_ERROR) {
    if (buffer) free(buffer);
    return NULL;
  }
  *width_ret = width;
  *height_ret = height;
  *numComponents_ret = realnumcomp;
  return buffer;
}

int 
simage_jasper_save(const char *filename,
                   const unsigned char * bytes,
                   int width,
                   int height,
                   int numcomponents)
{
  jaspererror = ERR_NOT_IMPLEMENTED;
  return 0;
}

typedef struct {
  int width;
  int height;
  int numcomp;
  int depth;  
} simage_jasper_opendata;

void * 
simage_jasper_open(const char * filename,
                   int * width,
                   int * height,
                   int * numcomponents)
{
  jaspererror = ERR_NOT_IMPLEMENTED;
  return NULL;
}


void 
simage_jasper_close(void * opendata)
{
  simage_jasper_opendata * od = (simage_jasper_opendata*) opendata;
  jaspererror = ERR_NOT_IMPLEMENTED;
}

int 
simage_jasper_read_line(void * opendata, int y, unsigned char * buf)
{
  simage_jasper_opendata * od;
  jaspererror = ERR_NOT_IMPLEMENTED;
   
  od = (simage_jasper_opendata*) opendata;
  return 0;
}

#endif /* HAVE_JASPER */

