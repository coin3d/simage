#ifndef _SIMAGE_H_
#define _SIMAGE_H_

/*
 * simage:
 *
 * A simple library for loading images. This library is designed
 * for loading textures to be used as OpenGL textures. 
 * Only pixel formats directly supported by OpenGL (v1.0) will be 
 * returned. The user is responsible for resizing the texture to
 * a 2^n width/height though.
 *
 * This library does not support the "new" OpenGL v1.1 image
 * formats. In the future this might be supported though. We still
 * have some old SGI HW with only OpenGL v1.0 installed...
 *
 * Usage information:
 *
 * simage_check_supported(filename): 
 *
 *   Will return 1 if filename can be loaded, 0 otherwise. 
 *
 * simage_read_image(filename, width, height, numComponents)
 *
 *   Will (attempt to) read filename, and return a pointer to
 *   the image data. NULL is returned if the image could not
 *   be loaded. The memory is allocated using malloc(), and
 *   it is the callers responsibility to free the memory (using free())
 *   width and height contains the width and height of the image,
 *   and numComponents is a number indicating the following:
 *     1 : Grayscale image (GL_LUMINANCE)
 *     2 : Grayscale with alpha channel (GL_LUMINANCE_ALPHA)
 *     3 : RGB data (GL_RGB)
 *     4 : RGB data with alpha component (GL_RGBA)
 *
 *   Other image formats might be supported in the future. You
 *   should not assume 1 <= numComponents <= 4 :-)
 *
 * A couple of functions for your convenience:
 *
 * simage_resize(imagedata, width, height, numComponents,
 *               newwidth, newheight)
 *
 *   A convenience function that can be used to resize an image.
 *   Since OpenGL textures must have width and height equal to 2^n,
 *   this is often needed. A pointer to the new image data is returned. 
 *   imagedata is not freed. BTW: If you need a high quality resize 
 *   function, don't use this one :-) 
 *
 * simage_next_power_of_two(int val)
 * 
 *   Will return the first 2^n bigger or equal to val. 
 *   If simage_next_power_of_two(size) != size, you'll typically
 *   need to resize your image to be able to use it in an OpenGL app.
 */  

/* Enable to generate SIMAGE.DLL (under Win32). */
#ifdef SIMAGE_MAKE_DLL
#define SIMAGE_DLL_EXPORT __declspec(dllexport)
#endif /* !SIMAGE_MAKE_DLL */

/* Empty define to avoid errors when _not_ compiling as a DLL. */
#ifndef SIMAGE_DLL_EXPORT
#define SIMAGE_DLL_EXPORT
#endif /* !SIMAGE_DLL_EXPORT */


#ifdef __cplusplus
extern "C" {
#endif

  /* user interface */
  SIMAGE_DLL_EXPORT unsigned char * simage_read_image(const char * filename,
						      int * w, int * h,
						      int * numComponents);
  SIMAGE_DLL_EXPORT int simage_check_supported(const char * filename);

  /* check this if simage_read_image returns NULL */
  SIMAGE_DLL_EXPORT const char * simage_get_last_error(void);

  /* convenience functions */
  SIMAGE_DLL_EXPORT unsigned char * simage_resize(unsigned char * imagedata,
						  int width, int height,
						  int numComponents,
						  int newwidth, int newheight);
  
  SIMAGE_DLL_EXPORT int simage_next_power_of_two(int val);


  /* plugin interface */
  struct simage_plugin 
  {
    unsigned char *(*load_func)(const char *,
				int * width,
				int * height,
				int * num_components);
    int (*identify_func)(const char * filename,
			 const unsigned char * header,
			 int headerlen);
    int (*error_func)(char * textbuffer, int bufferlen);
  };
  
  SIMAGE_DLL_EXPORT void * simage_add_plugin_loader(const struct simage_plugin * plugin,
						    int addbefore);
  SIMAGE_DLL_EXPORT void simage_remove_plugin_loader(void * handle);
  
#ifdef __cplusplus
}
#endif

#endif /* _SIMAGE_H_ */
