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

#define WIDTH 640
#define HEIGHT 480

void error_cb(const char *text)
{
  printf("c2m Error  : %s\n", text);
};

void warning_cb(const char *text)
{
  printf("c2m Warning: %s\n", text);
};

int  progress_cb(float sub, int current_frame, int num_frames)
{
  printf("c2m Progress: sub: %3.0f, curr: %d, tot:%d\n", sub*100.0, current_frame, num_frames);
  return 1;
};

SoOffscreenRenderer * renderer;
SoPerspectiveCamera * camera;
SoSeparator * root;

float x,y,z;
char fname[256];

int
main(
  int argc,
  char ** argv )
{
  QWidget * window = SoQt::init( argv[0] );
  if ( window == NULL )
    return -1;

  root = new SoSeparator;
  root->ref();
  root->addChild( camera = new SoPerspectiveCamera );
  root->addChild( new SoDirectionalLight );
  SoMaterial *myMaterial = new SoMaterial;
  myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
  root->addChild(myMaterial);
  SoNode *node;
  root->addChild(node = new SoCone);
  
  SbViewportRegion vp;
  vp.setWindowSize(SbVec2s(WIDTH, HEIGHT));
  
  renderer = new SoOffscreenRenderer( vp );

  renderer->setBackgroundColor( SbColor( 0.1f, 0.2f, 0.3f )  );

  camera->viewAll( node, renderer->getViewportRegion() );

  bool ret;
  SbVec3f cpos;

  s_params * params = s_params_create();
  s_params_set(params, 
               S_INTEGER_PARAM_TYPE, "width", WIDTH,
               S_INTEGER_PARAM_TYPE, "height", HEIGHT,
               S_FUNCTION_PARAM_TYPE, "error callback", error_cb,
               S_FUNCTION_PARAM_TYPE, "warning callback", warning_cb,
               S_FUNCTION_PARAM_TYPE, "progress callback", progress_cb,
               S_PARAM_END);
               
  s_movie * movie = s_movie_create(argv[1], params);
  assert(movie);

  int imax=30;
  for (int i=0; i<imax; i++)
  { 
    cpos = camera->position.getValue();
    cpos.getValue(x, y, z);
    x=1.0-(float)i/(float)imax *1.0*2.0;
    camera->position.setValue(x, y, z);

    renderer->render(root);
    sprintf(fname, "renderarea%0d.jpg", i+10);
    ret = renderer->writeToFile(fname, "jpg");

    s_image * image = s_image_create(WIDTH, HEIGHT, 3, renderer->getBuffer());
    
    s_movie_put_image(movie, image, NULL);
    
    s_image_destroy(image);    
  };

  s_movie_close(movie);
  s_movie_destroy(movie);
  return 0;
} // main()

