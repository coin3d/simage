#include <config.h>
#ifdef SIMAGE_AVIENC_SUPPORT

#include <stdio.h>
#include <string.h>

#include <simage_private.h>
#include <simage_avi.h>
#include <avi_encode.h>

int 
avienc_movie_create(const char * filename, s_movie * movie, s_params * params)
{
  void * handle;

  int width;
  int height;
  int fps;
  const char *preferences_filename;
  const char *mime_type;

  width = height = fps = 0;
  preferences_filename = NULL;
  mime_type = NULL;

  s_params_get(params, 
               "mime-type", S_STRING_PARAM_TYPE, &mime_type, NULL);

  if (!mime_type ||
      ((strcmp(mime_type, "video/x-msvideo") != 0) &&
       (strcmp(mime_type, "video/msvideo") != 0) &&
       (strcmp(mime_type, "video/avi") != 0))) {
    return 0;
  }

  /* Note 20020321 thammer: 
     I was unable to find any RFC with a MIME video subtype for avi-files.
     However, the three subtypes above seems to be widely used.
     Some examples:
     Mosaic - http://archive.ncsa.uiuc.edu/SDG/Software/Mosaic/Docs/extension-map.html
     Netscape - http://developer.netscape.com/docs/manuals/enterprise/40/nsapi/0c_mime.htm#1017298
              - http://www.ulis.ac.jp/newsys/man/mimetypes.html
     Jumpline - http://support.jumpline.com/mimetype_list.phtml <Extensive list of extension <-> mimetype mappings>
     Pure Mac - http://www.eskimo.com/~pristine/extkey.html
  */

  s_params_get(params, 
               "parameter file", S_STRING_PARAM_TYPE, &preferences_filename, NULL);

  s_params_get(params,
               "width", S_INTEGER_PARAM_TYPE, &width, NULL);
  
  s_params_get(params,
              "height", S_INTEGER_PARAM_TYPE, &height, NULL);

  s_params_get(params,
               "fps", S_INTEGER_PARAM_TYPE, &fps, NULL);

  /*
  if preferences_filename == NULL or a file named preferences_filename does not exist,
    a GUI dialog box will pop up and ask for preferences (compression method and parameters)
    if preferences_filename != NULL and user didn't press [Cancel]
      preferences will be saved to a new file named preferences_filename
  else (a file named preferences_filename exists)
    the preferences will be read from the specified file and no GUI will pop up

  */

  handle = (void *)avi_begin_encode(filename, width, height, fps, preferences_filename);
  
  if (handle == NULL) return 0;
  
  s_params_set(s_movie_params(movie), "avienc handle", S_POINTER_PARAM_TYPE, handle, 0);
  return 1;
}

int 
avienc_movie_put(s_movie * movie, s_image * image, s_params * params)
{
  void * handle;
  int retval;
  s_image *temp;

  if (s_params_get(s_movie_params(movie), "avienc handle", S_POINTER_PARAM_TYPE, &handle, 0)) {
    int mod;
    int order;
    order = s_image_get_component_order(image);
    if (order == SIMAGE_ORDER_BGR)
      return avi_encode_bitmap(handle, s_image_data(image), 0);
    else {        
      if ( (params != NULL) &&
           (s_params_get(params, "allow image modification", S_INTEGER_PARAM_TYPE, &mod, 0) && 
           mod) ) {
        if (avi_encode_bitmap(handle, s_image_data(image), 1)) {
          s_image_set_component_order(image, SIMAGE_ORDER_BGR);
          return 1;
        }
        else
          return 0;
      }
      else {
        /* Default to making a copy of the data */
        temp = s_image_create(s_image_width(image), s_image_height(image), 
                              s_image_components(image), NULL);
        s_image_set(temp, s_image_width(image), s_image_height(image), 
                    s_image_components(image), s_image_data(image), 1);
        retval = avi_encode_bitmap(handle, s_image_data(temp), 1);
        s_image_destroy(temp);
        return retval;
      }
    }
  }
  return 0;
}

void 
avienc_movie_close(s_movie * movie)
{
  void * handle;
  if (s_params_get(s_movie_params(movie), "avienc handle", S_POINTER_PARAM_TYPE, &handle, 0)) {
    avi_end_encode(handle);
  }
}

#endif /* SIMAGE_AVIENC_SUPPORT */
