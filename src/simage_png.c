/*
 * Based heavily on example code in libpng. Some bugs fixed though.
 */

#include <config.h>
#ifdef HAVE_PNGLIB

#include <simage_png.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <png.h>

#define ERR_NO_ERROR     0
#define ERR_OPEN         1
#define ERR_MEM          2
#define ERR_PNGLIB       3
#define ERR_OPEN_WRITE   4
#define ERR_PNGLIB_WRITE 5
#define ERR_MEM_WRITE    6

static int pngerror = ERR_NO_ERROR;

/* my setjmp buffer */
static jmp_buf setjmp_buffer;

/* called my libpng */
static void 
warn_callback(png_structp ps, png_const_charp pc)
{
/*   fprintf(stderr,"PNG warn: %s\n", pc); */
  /*FIXME: notify? */
}

static void 
err_callback(png_structp ps, png_const_charp pc)
{
/*   fprintf(stderr,"PNG error: %s\n", pc); */

  /* FIXME: store error message? */
  longjmp(setjmp_buffer, 1);
}

int 
simage_png_error(char * buffer, int buflen)
{
  switch (pngerror) {
  case ERR_OPEN:
    strncpy(buffer, "PNG loader: Error opening file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "PNG loader: Out of memory error", buflen);
    break;
  case ERR_PNGLIB:
    strncpy(buffer, "PNG loader: Illegal png file", buflen);
    break;
  case ERR_OPEN_WRITE:
    strncpy(buffer, "PNG saver: Error opening file", buflen);
    break;
  case ERR_PNGLIB_WRITE:
    strncpy(buffer, "PNG saver: Internal libpng error", buflen);    
    break;
  case ERR_MEM_WRITE:
    strncpy(buffer, "PNG saver: Out of memory error", buflen);
    break;
  }
  return pngerror;

}

int 
simage_png_identify(const char * ptr,
		    const unsigned char *header,
		    int headerlen)
{
  static unsigned char pngcmp[] = {0x89, 'P', 'N', 'G', 0xd, 0xa, 0x1a, 0xa};
  if (headerlen < 8) return 0;
  if (memcmp((const void*)header, 
	     (const void*)pngcmp, 8) == 0) return 1;
  return 0;
}

/* our method that reads from a FILE* and fills up the buffer that
   libpng wants when parsing a PNG file */
static void
user_read_cb(png_structp png_ptr, png_bytep data, png_uint_32 length)
{
  int readlen = fread(data, 1, length, (FILE *)png_get_io_ptr(png_ptr));
  if (readlen != length) {
    /* FIXME: then what? png_error()? 20020821 mortene */
  }
}

/* our method that write compressed png image data to a FILE* */
static void
user_write_cb(png_structp png_ptr, png_bytep data, png_uint_32 length)
{
  int writelen = fwrite(data, 1, length, (FILE *)png_get_io_ptr(png_ptr));
  if (writelen != length) {
    /* FIXME: then what? png_error()? 20020821 mortene */
  }
}

/* our method that flushes written compressed png image data */
static void
user_flush_cb(png_structp png_ptr)
{
  int err = fflush((FILE *)png_get_io_ptr(png_ptr));
  if (err != 0) {
    /* FIXME: then what? png_error()? 20020821 mortene */
  }
}

unsigned char *
simage_png_load(const char *filename,
		 int *width_ret,
		 int *height_ret,
		 int *numComponents_ret)
{
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  
  int bit_depth, color_type, interlace_type;
  FILE *fp;
  unsigned char *buffer;
  int y, bytes_per_row;
  int channels;
  int format;
  png_bytepp row_pointers;

  if ((fp = fopen(filename, "rb")) == NULL) {
    pngerror = ERR_OPEN;
    return NULL;
  }

  /* Create and initialize the png_struct with the desired error handler
   * functions.  If you want to use the default stderr and longjump method,
   * you can supply NULL for the last three parameters.  We also supply the
   * the compiler header file version, so that we know if the application
   * was compiled with a compatible version of the library.  REQUIRED
   */
  /*png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      (void *)user_error_ptr, user_error_fn, user_warning_fn);*/

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				   NULL, err_callback, warn_callback);
  
  if (png_ptr == NULL) {
    pngerror = ERR_MEM;
    fclose(fp);
    return 0;
  }

  /* Allocate/initialize the memory for image information.  REQUIRED. */
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    pngerror = ERR_MEM;
    fclose(fp);
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return 0;
  }
  
  /* Set error handling if you are using the setjmp/longjmp method (this is
   * the normal method of doing things with libpng).  REQUIRED unless you
   * set up your own error handlers in the png_create_read_struct() earlier.
   */

  buffer = NULL;

  if (setjmp(setjmp_buffer)) {
    pngerror = ERR_PNGLIB;
    /* Free all of the memory associated with the png_ptr and info_ptr */
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    fclose(fp);
    /* If we get here, we had a problem reading the file */

    if (buffer) free(buffer);
    return NULL;
  }
  
  /*  we're not using png_init_io(), as we don't want to pass a FILE*
      into libpng, in case it's an MSWindows DLL with a different CRT
      (C run-time library) */
  png_set_read_fn(png_ptr, (void *)fp, (png_rw_ptr)user_read_cb);

  /* The call to png_read_info() gives us all of the information from the
   * PNG file before the first IDAT (image data chunk).  REQUIRED
   */
  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	       &interlace_type, NULL, NULL);

  /**** Set up the data transformations you want.  Note that these are all
  **** optional.  Only call them if you want/need them.  Many of the
  **** transformations only work on specific types of images, and many
  **** are mutually exclusive.
  ****/

  /* tell libpng to strip 16 bit/color files down to 8 bits/color */
  png_set_strip_16(png_ptr);

  /* strip alpha bytes from the input data without combining with th
   * background (not recommended) */
  /* png_set_strip_alpha(png_ptr); */
  
  /* extract multiple pixels with bit depths of 1, 2, and 4 from a single
   * byte into separate bytes (useful for paletted and grayscale images).
   */
  /* png_set_packing(png_ptr); */

  /* change the order of packed pixels to least significant bit first
   * (not useful if you are using png_set_packing). */
  /* png_set_packswap(png_ptr); */
  
  /* expand paletted colors into true RGB triplets */
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_expand(png_ptr);

  /* expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand(png_ptr);
  
  /* expand paletted or RGB images with transparency to full alpha channels
   * so the data will be available as RGBA quartets */
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_expand(png_ptr);
  
  /* Add filler (or alpha) byte (before/after each RGB triplet) */
  /* png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER); */

  png_read_update_info(png_ptr, info_ptr);

  channels = png_get_channels(png_ptr, info_ptr);

  /* allocate the memory to hold the image using the fields of info_ptr. */
  
  bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);

  
  buffer = (unsigned char*) malloc(bytes_per_row*height);

  format = channels;

  row_pointers = (png_bytepp) malloc(height*sizeof(png_bytep));
  for (y = 0; y < height; y++) {
    row_pointers[height-y-1] = buffer + y*bytes_per_row;
  }
  
  png_read_image(png_ptr, row_pointers);
  png_read_end(png_ptr, info_ptr);
  
  free(row_pointers);

  /* clean up after the read, and free any memory allocated - REQUIRED */
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

  /* close the file */
  fclose(fp);

  /* that's it */
  if (buffer) {
    *width_ret = width;
    *height_ret = height;
    *numComponents_ret = format;
    pngerror = ERR_NO_ERROR;
  }
  else {
    pngerror = ERR_MEM;
  }
  return buffer;
}

int 
simage_png_save(const char *filename,
                const unsigned char * bytes,
                int width,
                int height,
                int numcomponents)
{
  FILE * fp;
  png_structp png_ptr;
  png_infop info_ptr;
  int colortype;
  int y, bytesperrow;
#ifdef PNG_TEXT_SUPPORTED
  png_text text_ptr[3];
#endif  

  /* open the file */
  fp = fopen(filename, "wb");
  if (fp == NULL) {
    pngerror = ERR_OPEN_WRITE;
    return 0;
  }
  
  /* Create and initialize the png_struct with the desired error handler
   * functions.  If you want to use the default stderr and longjump method,
   * you can supply NULL for the last three parameters.  We also check that
   * the library version is compatible with the one used at compile time,
   * in case we are using dynamically linked libraries.  REQUIRED.
   */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				    NULL, NULL, NULL);

  if (png_ptr == NULL) {
    pngerror = ERR_OPEN_WRITE;
    fclose(fp);
    return 0;
  }

  /* Allocate/initialize the image information data.  REQUIRED */
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
    pngerror = ERR_MEM_WRITE;
    return 0;
  }

  /* Set error handling.  REQUIRED if you aren't supplying your own
   * error hadnling functions in the png_create_write_struct() call.
   */
  if (setjmp(png_ptr->jmpbuf)) {
    /* If we get here, we had a problem reading the file */
    fclose(fp);
    png_destroy_write_struct(&png_ptr,  (png_infopp)info_ptr);
    pngerror = ERR_PNGLIB_WRITE;
    return 0;
  }

  /*  we're not using png_init_io(), as we don't want to pass a FILE*
      into libpng, in case it's an MSWindows DLL with a different CRT
      (C run-time library) */
  png_set_write_fn(png_ptr, (void *)fp, (png_rw_ptr)user_write_cb,
                   (png_flush_ptr)user_flush_cb);
  
  /* Set the image information here.  Width and height are up to 2^31,
   * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
   * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
   * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
   * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
   * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
   * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
   */

  switch (numcomponents) {
  case 1:
    colortype = PNG_COLOR_TYPE_GRAY;
    break;
  case 2:
    colortype = PNG_COLOR_TYPE_GRAY_ALPHA;
    break;
  case 3:
    colortype = PNG_COLOR_TYPE_RGB;
    break;
  default:
  case 4:
    colortype = PNG_COLOR_TYPE_RGB_ALPHA;
    break;
  }

  png_set_IHDR(png_ptr, info_ptr, width, height, 8, colortype,
	       PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  /* Optional gamma chunk is strongly suggested if you have any guess
   * as to the correct gamma of the image. */
  /* png_set_gAMA(png_ptr, info_ptr, gamma); */

#if defined(PNG_TEXT_SUPPORTED)
  /* Optionally write comments into the image */
  text_ptr[0].key = "Title";
  text_ptr[0].text = (char*)filename;
  text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
  text_ptr[1].key = "Author";
  text_ptr[1].text = "simage (http://www.coin3d.org)";
  text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
  text_ptr[2].key = "Description";
  text_ptr[2].text = "Image saved using simage.";
  text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
  png_set_text(png_ptr, info_ptr, text_ptr, 3);
#endif /* PNG_TEXT_SUPPORTED */

  /* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */

  /* Write the file header information.  REQUIRED */
  png_write_info(png_ptr, info_ptr);

  /* Once we write out the header, the compression type on the text
   * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
   * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
   * at the end.
   */

  /* set up the transformations you want.  Note that these are
   * all optional.  Only call them if you want them. */

  /* invert monocrome pixels */
  /* png_set_invert(png_ptr); */

  /* Shift the pixels up to a legal bit depth and fill in
   * as appropriate to correctly scale the image */
  /* png_set_shift(png_ptr, &sig_bit);*/

  /* pack pixels into bytes */
  /* png_set_packing(png_ptr); */

  /* swap location of alpha bytes from ARGB to RGBA */
  /* png_set_swap_alpha(png_ptr); */

  /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
   * RGB (4 channels -> 3 channels). The second parameter is not used. */
  /* png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE); */

  /* flip BGR pixels to RGB */
  /* png_set_bgr(png_ptr); */

  /* swap bytes of 16-bit files to most significant byte first */
  /* png_set_swap(png_ptr); */

  /* swap bits of 1, 2, 4 bit packed pixel formats */
  /* png_set_packswap(png_ptr); */


  /* The easiest way to write the image (you may have a different memory
   * layout, however, so choose what fits your needs best).  You need to
   * use the first method if you aren't handling interlacing yourself.
   */

  /* If you are only writing one row at a time, this works */
  
  bytesperrow = width * numcomponents;

  for (y = 0; y < height; y++) {
    png_write_row(png_ptr, (png_bytep) bytes + bytesperrow * (height-y-1));
  }
  
  /* You can write optional chunks like tEXt, zTXt, and tIME at the end
   * as well.
   */
  
  /* It is REQUIRED to call this to finish writing the rest of the file */
  png_write_end(png_ptr, info_ptr);

  /* if you allocated any text comments, free them here */

  /* clean up after the write, and free any memory allocated */
  png_destroy_write_struct(&png_ptr, &info_ptr);

  /* close the file */
  fclose(fp);

  /* that's it */
  return 1;
}

#endif /* HAVE_PNGLIB */
