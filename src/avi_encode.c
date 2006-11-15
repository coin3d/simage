#include <config.h>
#ifdef SIMAGE_AVIENC_SUPPORT

#include "avi_encode.h"

#define HAVE_VFW

#ifdef HAVE_VFW

#include <windows.h>
#include <vfw.h>
#include <stdio.h>

/* fixme 20020227 thammer: are there any problems with specifying this library
   here as opposed to in the makefile? */
#pragma comment(lib, "vfw32.lib")

typedef struct {
  PAVIFILE pfile; 
  PAVISTREAM ps;
  PAVISTREAM pscomp;
  int framenumber;
  int width;
  int height;
} avi_encode_context;

static void avi_init_context(avi_encode_context *context)
{
  context->pfile = NULL;
  context->ps = NULL;
  context->pscomp = NULL;
  context->framenumber = 0;
  context->width = 0;
  context->height = 0;
}

static void avi_cleanup_context(avi_encode_context *context)
{
  if (context->pscomp)
    AVIStreamRelease(context->pscomp);
  context->pscomp = NULL;

  if (context->ps)
    AVIStreamRelease(context->ps);
  context->ps = NULL;

  if (context->pfile)
    AVIFileRelease(context->pfile);
  context->pfile = NULL;
}

#endif /* HAVE_VFW */


void *
avi_begin_encode(const char *filename, int width, int height, int fps, const char *preferences_filename)
{
#ifdef HAVE_VFW
  avi_encode_context *context;
  HRESULT hr;
  BOOL ret;
  AVISTREAMINFO strhdr;
  BITMAPINFO bi;
  AVICOMPRESSOPTIONS opts;
  AVICOMPRESSOPTIONS * aopts[1];
  int rowsize;
  int imagesize;
  int numbits;
  int prefsReadFromFile;
  
  if ( (width % 4 != 0) || (height % 4 != 0) )
    return NULL; /* width and height must be divisible by 4 (several codecs crashes if this is not true) */

  context = (avi_encode_context *) malloc(sizeof(avi_encode_context));
  avi_init_context(context);

  context->width = width;
  context->height = height;

  AVIFileInit();

  /* Open file */
  hr = AVIFileOpen(&context->pfile , filename, OF_WRITE | OF_CREATE, NULL); 
  if (hr != AVIERR_OK) {
    avi_cleanup_context(context);
    free(context);
    return NULL;
  }

  /*
  fixme 20020304 thammer: Investigate what happens if the file allready exists. 
  Preliminary tests indicate that the new stream is just added to the existing 
  file (increasing the file size), instead of truncating the file first, as the
  documentation for AVIFileOpen states.
  */

  numbits = 24;
  rowsize = (width * numbits + 31) / 32 * 4; /* aligned to 32 bits */
  imagesize = rowsize * height;

  memset(&strhdr, 0, sizeof(strhdr));
  strhdr.fccType = streamtypeVIDEO;
  strhdr.fccHandler = 0;
  strhdr.dwScale = 1;
  strhdr.dwRate = fps;
  strhdr.dwSuggestedBufferSize = imagesize;
  strhdr.rcFrame.left = 0; 
  strhdr.rcFrame.top = 0;
  strhdr.rcFrame.right = width;
  strhdr.rcFrame.bottom = height;

  /* Create stream */
  hr = AVIFileCreateStream(context->pfile, &context->ps, &strhdr);
  if (hr != AVIERR_OK) {
    avi_cleanup_context(context);
    free(context);
    return NULL;
  }

  aopts[0] = &opts;
  memset(&opts, 0, sizeof(opts));

  prefsReadFromFile = 0;
  if ( (preferences_filename != NULL) && (strlen(preferences_filename)>0) ) {
    FILE *file;
    int size;
    file = fopen(preferences_filename, "rb");
    if (file==NULL) {
      /* file doesn't exist, must pop up GUI to get options */
      ret = AVISaveOptions(NULL, ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE, 1, &context->ps, (LPAVICOMPRESSOPTIONS *) &aopts);
      if (!ret) {
        /* User pressed [Cancel] */
        avi_cleanup_context(context);
        free(context);
        return NULL;
      }
      /* Save options to file*/
      file = fopen(preferences_filename, "wb");
      if (file == NULL) {
        avi_cleanup_context(context);
        free(context);
        return NULL;
      }

      /* write AVICOMPRESSOPTIONS struct */
      size = fwrite(&opts, sizeof(AVICOMPRESSOPTIONS), 1, file);
      
      /* write AVICOMPRESSOPTIONS.cbFormat */
      size = fwrite(&opts.cbFormat, 4, 1, file);
      
      /* write AVICOMPRESSOPTIONS.lpFormat */
      size = fwrite(opts.lpFormat, opts.cbFormat, 1, file);
      
      /* write AVICOMPRESSOPTIONS.cbParms */
      size = fwrite(&opts.cbParms, 4, 1, file);
      
      /* write AVICOMPRESSOPTIONS.lpParms */
      size = fwrite(opts.lpParms, opts.cbParms, 1, file);

      fclose(file);
    } else {
      /* Read options from file */
      file = fopen(preferences_filename, "rb");
      if (file == NULL) {
        avi_cleanup_context(context);
        free(context);
        return NULL;
      }

      /* read AVICOMPRESSOPTIONS struct */
      size = fread(&opts, sizeof(AVICOMPRESSOPTIONS), 1, file);
      
      /* read AVICOMPRESSOPTIONS.cbFormat */
      size = fread(&opts.cbFormat, 4, 1, file);
      
      /* read AVICOMPRESSOPTIONS.lpFormat */
      opts.lpFormat = (void *) malloc(opts.cbFormat);
      size = fread(opts.lpFormat, opts.cbFormat, 1, file);
      
      /* read AVICOMPRESSOPTIONS.cbParms */
      size = fread(&opts.cbParms, 4, 1, file);
      
      /* read AVICOMPRESSOPTIONS.lpParms */
      opts.lpParms = (void *) malloc(opts.cbParms);
      size = fread(opts.lpParms, opts.cbParms, 1, file);

      fclose(file);

      prefsReadFromFile = 1;
    }
  }
  else {
    ret = AVISaveOptions(NULL, ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE, 1, &context->ps, (LPAVICOMPRESSOPTIONS *) &aopts);
    if (!ret) {
      /* User pressed [Cancel] */
      avi_cleanup_context(context);
      free(context);
      return NULL;
    }
  };

  hr = AVIMakeCompressedStream( &context->pscomp, context->ps, &opts, NULL);
  if (hr != AVIERR_OK) {
    avi_cleanup_context(context);
    free(context);
    return NULL;
  }

  if (prefsReadFromFile)
  {
    /* Since we don't know if our method of allocating memory (malloc) differs from
       whatever AVISaveOptions() uses, we free what we created ourselves.
    */
    free(opts.lpFormat);
    opts.lpFormat = NULL;
    free(opts.lpParms);
    opts.lpParms = NULL;
  }
  else
    AVISaveOptionsFree(1, (LPAVICOMPRESSOPTIONS *) &aopts);


  memset(&bi, 0, sizeof(BITMAPINFO));

  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
  bi.bmiHeader.biWidth = width ;
  bi.bmiHeader.biHeight = height ;
  bi.bmiHeader.biPlanes = 1 ;
  bi.bmiHeader.biBitCount = numbits ;
  bi.bmiHeader.biCompression = BI_RGB ;
  bi.bmiHeader.biSizeImage = imagesize ;
  bi.bmiHeader.biXPelsPerMeter = 0 ;
  bi.bmiHeader.biYPelsPerMeter = 0 ;
  bi.bmiHeader.biClrUsed = (numbits <= 8) ? 1 << numbits : 0;
  bi.bmiHeader.biClrImportant = 0 ;

  hr = AVIStreamSetFormat(context->pscomp, 0, &bi, bi.bmiHeader.biSize + bi.bmiHeader.biClrUsed * sizeof(RGBQUAD));

  if (hr != AVIERR_OK) {
    avi_cleanup_context(context);
    free(context);
    return NULL;
  }

  return (void *)context;

#else /* HAVE_VFW */
  return NULL;
#endif /* HAVE_VFW*/

}

int 
avi_encode_bitmap(void *handle, unsigned char *buf, int rgb2bgr)
{
  /* Note: buf must be a 24-bit RGB (or BGR) image with the same size as
     given to avi_begin_encode. It is the caller's responsibility to
     check this.
     
     If rgb2bgr, the color ordering in the buffer will be modified from RGB to BGR.
     NB: this means that the buffer will be modified.  
     
     2003-03-14 thammer.
*/
#ifdef HAVE_VFW
  HRESULT hr;
  avi_encode_context *context;

  context = (avi_encode_context *)handle;

  /* For some reason, AVIStreamWrite requires the color components to
  be ordered BGR instead of RGB */
  if (rgb2bgr) {
    int x, y, r, nc, w, h;
    nc = 3;
    w = context->width;
    h = context->height;
    for (x=0; x<w; x++)
      for (y=0; y<h; y++) {
        r = buf[x * nc + y * nc * w + 0];
        buf[x * nc + y * nc * w + 0] = buf[x * nc + y * nc * w + 2];
        buf[x * nc + y * nc * w + 2] = r;
      }
  }

  hr = AVIStreamWrite(context->pscomp,
                      context->framenumber,
                      1,        
                      (LPBYTE) buf,
                      context->width * context->height * 3, /* nc = 3 (24 bit) */  
                      AVIIF_KEYFRAME,
                      NULL,
                      NULL);

  /*
  fixme 20020227 thammer: Is it correct / smart to let every frame be
  a keyframe? Check avi doc and virtualdub.  */

  if (hr != AVIERR_OK)
    return 0;

  context->framenumber++;

  return 1;
#else /* HAVE_VFW */
  return 0;
#endif /* HAVE_VFW */
};

int 
avi_end_encode(void *handle)
{
#ifdef HAVE_VFW
  avi_encode_context *context;

  context = (avi_encode_context *)handle;
  avi_cleanup_context(context);
  free(context);
  AVIFileExit();
  return 1; 
#else /* HAVE_VFW */
  return 0
#endif /* HAVE_VFW */

};

#endif /* SIMAGE_AVIENC_SUPPORT */
