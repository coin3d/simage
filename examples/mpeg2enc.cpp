/*
 * Demonstrates how to use the simage API to write MPEG movies of
 * frames from the SoOffscreenRenderer.
 *
 * Note that at the moment, the executable generated from this code
 * must be run from the same directory as the parameters file
 * 'ntsc_coin.par'.
 */

#include <Inventor/SoInteraction.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <simage.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Params {
  int width;
  int height;
  int clocktime;
  int constraintslevel;
};

static const struct Params defaultpars = {
  640, // width
  480, // height
  1, // clocktime = 1 sec
  8 // constraintslevel
};

static void
error_cb(void * userdata, const char *text)
{
  (void)fprintf(stderr, "Error: %s\n", text);
  (void)fflush(stderr);
}

static void
warning_cb(void * userdata, const char *text)
{
  (void)fprintf(stderr, "Warning: %s\n", text);
  (void)fflush(stderr);
}

static int
progress_cb(void * userdata, float sub, int current_frame, int num_frames)
{
  char buffer[256];

  int logframes = (int)log10(num_frames) + 1;
  (void)sprintf(buffer, "\rwriting frame: %%%dd / %%%dd  -- %%03.1f%%%%  ",
                logframes, logframes);

  (void)fprintf(stdout, buffer, current_frame + 1, num_frames, sub * 100.0);
  (void)fflush(stdout);
  return 1;
}

static void
print_usage(const char * appname)
{
  if (appname == NULL) { appname = "mpeg2enc"; }
  (void)fprintf(stderr, "\n\tUsage: %s [options] <moviefile>\n\n", appname);
  (void)fprintf(stderr, "\t\t--width <xsize> (default %d)\n", defaultpars.width);
  (void)fprintf(stderr, "\t\t--height <ysize> (default %d)\n", defaultpars.height);
  (void)fprintf(stderr, "\t\t--clocktime <length of movie in seconds> (default %d)\n", defaultpars.clocktime);
  (void)fprintf(stderr, "\t\t--level <constraints level: 4, 6, 8 or 10> (default %d)\n", defaultpars.constraintslevel);
  (void)fprintf(stderr, "\n");
}

static SbBool
optcmp(const char * arg, const char * optionname)
{
  return ((strncmp(arg, "--", 2) == 0) &&
          (strcmp(&arg[2], optionname) == 0));
}

int
main(int argc, char ** argv)
{
  struct Params userpars;
  (void)memcpy(&userpars, &defaultpars, sizeof(struct Params));

  char * APPNAME = (argc >= 1) ? argv[0] : NULL;
  char * MPGOUT = NULL;

  int optidx;
  for (optidx=1; optidx < argc - 1; optidx++) {
    if (optcmp(argv[optidx], "width")) { userpars.width = atoi(argv[++optidx]); }
    else if (optcmp(argv[optidx], "height")) { userpars.height = atoi(argv[++optidx]); }
    else if (optcmp(argv[optidx], "clocktime")) { userpars.clocktime = atoi(argv[++optidx]); }
    else if (optcmp(argv[optidx], "level")) { userpars.constraintslevel = atoi(argv[++optidx]); }
    else {
      (void)fprintf(stderr, "Error: unknown option '%s'\n", argv[optidx]);
      print_usage(APPNAME);
      exit(1);
    }
  }

  // FIXME: sanity check input arguments:
  //
  //  * width, height, clocktime > 0
  //
  //  * level = { 4, 6, 8, 10 }
  //
  // 20010918 mortene.

  if (optidx < argc) {
    if (strncmp("--", argv[optidx], 2) == 0) {
      print_usage(APPNAME);
      exit(1);
    }
    else {
      MPGOUT = argv[optidx];
    }
  }

  if (MPGOUT == NULL) {
    print_usage(APPNAME);
    exit(1);
  }

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild( camera );

  root->addChild( new SoDirectionalLight );
  SoMaterial *myMaterial = new SoMaterial;
  myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
  root->addChild(myMaterial);
  SoNode *node;
  root->addChild(node = new SoCone);
  
  SbViewportRegion vp;
  vp.setWindowSize(SbVec2s(userpars.width, userpars.height));
  
  SoOffscreenRenderer * renderer = new SoOffscreenRenderer( vp );

  renderer->setBackgroundColor( SbColor( 0.1f, 0.2f, 0.3f )  );

  camera->viewAll( node, renderer->getViewportRegion() );

  // the animation is 30Hz
  int nr_frames = userpars.clocktime * 30;

  s_params * params = s_params_create();
  s_params_set(params, 
               "mime-type", S_STRING_PARAM_TYPE, "video/mpeg",
               "width", S_INTEGER_PARAM_TYPE, userpars.width,
               "height", S_INTEGER_PARAM_TYPE, userpars.height,

               "num frames", S_INTEGER_PARAM_TYPE, nr_frames,

               "error callback", S_FUNCTION_PARAM_TYPE, error_cb,
               "warning callback", S_FUNCTION_PARAM_TYPE, warning_cb,
               "progress callback", S_FUNCTION_PARAM_TYPE, progress_cb,
               /* use to specify userdata for all callbacks */
               "callback userdata", S_POINTER_PARAM_TYPE, NULL,

               /* use to encode as mpeg1 instead of mpeg2 */
               "mpeg1", S_BOOL_PARAM_TYPE, 0,

               /* use to specify a parameter file */
               "parameter file", S_STRING_PARAM_TYPE, "ntsc_coin.par",

               /* use to specify constraints coded parameter constraints for mpeg2 files, 
                  such as bitrate, sample rate, and maximum allowed motion vector range.

                  Value Meaning         Typical use
                  ----  --------------- -----------------------------------------------
                  4     High Level      HDTV production rates: e.g. 1920 x 1080 x 30 Hz
                  6     High 1440 Level HDTV consumer rates: e.g. 1440 x 960 x 30 Hz
                  8     Main Level      CCIR 601 rates: e.g. 720 x 480 x 30 Hz
                  10    Low Level       SIF video rate: e.g. 352 x 240 x 30 Hz
               */
               "level", S_INTEGER_PARAM_TYPE, userpars.constraintslevel,

               /* NULL means no more params */
               NULL);
               
  s_movie * movie = s_movie_create(MPGOUT, params);
  s_params_destroy(params);
  if (movie == NULL) {
    error_cb(NULL, "could not create movie file");
    if (simage_get_last_error()) { error_cb(NULL, simage_get_last_error()); }
    exit(1);
  }
  

  s_image * image = NULL;

  for (int i=0; i < nr_frames; i++)
  { 
    SbVec3f cpos = camera->position.getValue();
    float x, y, z;
    cpos.getValue(x, y, z);
    x = 1.0f - (float)i / (float)nr_frames * 2.0f;
    camera->position.setValue(x, y, z);

    renderer->render(root);

#if DEBUG_SAVE_FRAMES // debug
    /* just save jpeg images for debugging */
    char fname[256];
    sprintf(fname, "renderarea%0d.jpg", i);
    SbBool ret = renderer->writeToFile(fname, "jpg");
#endif // debug

    if (image == NULL) {
      image = s_image_create(userpars.width, userpars.height, 3, renderer->getBuffer());
    }
    else s_image_set(image, userpars.width, userpars.height, 3, renderer->getBuffer(), 0);
    s_movie_put_image(movie, image, NULL);
  }
  
  if (image) s_image_destroy(image);    
  s_movie_close(movie);
  s_movie_destroy(movie);

  (void)fprintf(stdout, "\n");
  return 0;
}
