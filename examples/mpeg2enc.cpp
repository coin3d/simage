#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCone.h>

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/SoOffscreenRenderer.h>

#include <simage.h>
#include <stdlib.h>
#include <stdio.h>

static void
error_cb(void * userdata, const char *text)
{
  (void)fprintf(stderr, "c2m Error  : %s\n", text);
  (void)fflush(stderr);
}

static void
warning_cb(void * userdata, const char *text)
{
  (void)fprintf(stderr, "c2m Warning: %s\n", text);
  (void)fflush(stderr);
}

static int
progress_cb(void * userdata, float sub, int current_frame, int num_frames)
{
  (void)fprintf(stdout, "c2m Progress: sub: %3.0f, curr: %d, tot:%d\n",
                sub * 100.0, current_frame, num_frames);
  (void)fflush(stdout);
  return 1;
}

int
main(int argc, char ** argv)
{
  const int WIDTH = 640;
  const int HEIGHT = 480;
  const int NUMFRAMES = 30;

  QWidget * window = SoQt::init( argv[0] );
  if ( window == NULL )
    return -1;

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
  vp.setWindowSize(SbVec2s(WIDTH, HEIGHT));
  
  SoOffscreenRenderer * renderer = new SoOffscreenRenderer( vp );

  renderer->setBackgroundColor( SbColor( 0.1f, 0.2f, 0.3f )  );

  camera->viewAll( node, renderer->getViewportRegion() );

  s_params * params = s_params_create();
  s_params_set(params, 
               "width", S_INTEGER_PARAM_TYPE, WIDTH,
               "height", S_INTEGER_PARAM_TYPE, HEIGHT,
               "num frames", S_INTEGER_PARAM_TYPE, NUMFRAMES,
               "error callback", S_FUNCTION_PARAM_TYPE, error_cb,
               "warning callback", S_FUNCTION_PARAM_TYPE, warning_cb,
               "progress callback", S_FUNCTION_PARAM_TYPE, progress_cb,
               /* use to specify userdata for all callbacks */
               "callback userdata", S_POINTER_PARAM_TYPE, NULL,

               /* use to encode as mpeg1 instead of mpeg2 */
               /* "mpeg1", S_BOOL_PARAM_TYPE, 1, */

               /* use to specify a parameter file */
               /* "parameter file", S_STRING_PARAM_TYPE, "ntsc_coin.par", */

               /* use to specify constraints coded parameter constraints for mpeg2 files, 
                  such as bitrate, sample rate, and maximum allowed motion vector range.

                  Value Meaning         Typical use
                  ----  --------------- -----------------------------------------------
                  4     High Level      HDTV production rates: e.g. 1920 x 1080 x 30 Hz
                  6     High 1440 Level HDTV consumer rates: e.g. 1440 x 960 x 30 Hz
                  8     Main Level      CCIR 601 rates: e.g. 720 x 480 x 30 Hz
                  10    Low Level       SIF video rate: e.g. 352 x 240 x 30 Hz
               */
               /* "level", S_INTEGER_PARAM_TYPE, 6, */

               /* NULL means no more params */
               NULL);
               
  s_movie * movie = s_movie_create(argv[1], params);
  assert(movie);

  s_image * image = NULL;

  int imax = NUMFRAMES;
  for (int i=0; i<imax; i++)
  { 
    SbVec3f cpos = camera->position.getValue();
    float x, y, z;
    cpos.getValue(x, y, z);
    x=1.0-(float)i/(float)imax *1.0*2.0;
    camera->position.setValue(x, y, z);

    renderer->render(root);

    /* just save jpeg images for debugging */
    char fname[256];
    sprintf(fname, "renderarea%0d.jpg", i+10);
    SbBool ret = renderer->writeToFile(fname, "jpg");

    if (image == NULL) {
      image = s_image_create(WIDTH, HEIGHT, 3, renderer->getBuffer());
    }
    else s_image_set(image, WIDTH, HEIGHT, 3, renderer->getBuffer(), 0);
    s_movie_put_image(movie, image, NULL);
  };
  
  if (image) s_image_destroy(image);    
  s_movie_close(movie);
  s_movie_destroy(movie);
  return 0;
}
