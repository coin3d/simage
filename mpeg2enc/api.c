#include "api.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "global.h"
#include "putseq.h"
#include <simage_private.h>

static int
SimpegWrite_encode(const char * output_filename,
                   const char * parameter_filename,
                   SimpegWrite_error_cb error_cb,
                   SimpegWrite_warning_cb warning_cb,
                   SimpegWrite_progress_cb progress_cb,
                   void * cbuserdata);


static void *
SimpegWrite_begin_encode(const char * output_filename,
                         const char * parameter_filename,
                         SimpegWrite_error_cb error_cb,
                         SimpegWrite_warning_cb warning_cb,
                         SimpegWrite_progress_cb progress_cb,
                         void * cbuserdata,
                         int w, int h, int numframes, int mpeg1, int level, float bitrate);

static int
SimpegWrite_encode_bitmap(void * handle, const unsigned char * rgb_buffer);

static int
SimpegWrite_end_encode(void * handle);

/* simage movie interface */
int
mpeg2enc_movie_create(const char * filename, s_movie * movie, s_params * params)
{
  void * handle;
  const char * paramfile;
  void * cb0, *cb1, *cb2;
  void * cbdata;
  int w, h, numframes;
  int mpeg1;
  int level;
  int bitrate;
  const char *mime_type;


  w = h = 0;
  numframes = 0;
  mpeg1 = 0;
  level=0;
  bitrate = -1;

  paramfile = NULL;
  cb0 = NULL;
  cb1 = NULL;
  cb2 = NULL;
  mime_type = NULL;

  s_params_get(params,
               "mime-type", S_STRING_PARAM_TYPE, &mime_type, NULL);

  if (!mime_type || (strcmp(mime_type, "video/mpeg") != 0)) {
    return 0;
  }

  /* Note 20020321 thammer:
     The video/mpeg mime type is defined in RFC 2046 "MIME Part Two: Media Types".
     http://www.nacs.uci.edu/indiv/ehood/MIME/2046/rfc2046.html
  */

  s_params_get(params,
               "parameter file", S_STRING_PARAM_TYPE, &paramfile, NULL);

  s_params_get(params,
               "error callback", S_FUNCTION_PARAM_TYPE, &cb0, NULL);

  s_params_get(params,
               "warning callback", S_FUNCTION_PARAM_TYPE, &cb1, NULL);

  s_params_get(params,
               "progress callback", S_FUNCTION_PARAM_TYPE, &cb2, NULL);

  s_params_get(params,
               "callback userdata", S_POINTER_PARAM_TYPE, &cbdata, NULL);

  s_params_get(params,
               "width", S_INTEGER_PARAM_TYPE, &w, NULL);

  s_params_get(params,
              "height", S_INTEGER_PARAM_TYPE, &h, NULL);

  s_params_get(params,
               "num frames", S_INTEGER_PARAM_TYPE, &numframes, NULL);

  s_params_get(params,
               "mpeg1", S_BOOL_PARAM_TYPE, &mpeg1, NULL);

  s_params_get(params,
               "level", S_INTEGER_PARAM_TYPE, &level, NULL);


  s_params_get(params,
  			   "bitrate", S_INTEGER_PARAM_TYPE, &bitrate, NULL);

/*
  Level ID

  Specifies coded parameter constraints, such as bitrate, sample rate, and
  maximum allowed motion vector range.

  Code  Meaning         Typical use
  ----  --------------- -----------------------------------------------
  4     High Level      HDTV production rates: e.g. 1920 x 1080 x 30 Hz
  6     High 1440 Level HDTV consumer rates: e.g. 1440 x 960 x 30 Hz
  8     Main Level      CCIR 601 rates: e.g. 720 x 480 x 30 Hz
  10    Low Level       SIF video rate: e.g. 352 x 240 x 30 Hz
*/

  handle = SimpegWrite_begin_encode(filename,
                                    paramfile,
                                    (SimpegWrite_error_cb) cb0,
                                    (SimpegWrite_warning_cb) cb1,
                                    (SimpegWrite_progress_cb) cb2,
                                    cbdata,
                                    w, h, numframes, mpeg1, level, (float) bitrate);
  if (handle == NULL) return 0;

  s_params_set(s_movie_params(movie), "mpeg2enc handle", S_POINTER_PARAM_TYPE, handle, 0);
  return 1;
}

int
mpeg2enc_movie_put(s_movie * movie, s_image * image, s_params * params)
{
  void * handle;

  if (s_params_get(s_movie_params(movie), "mpeg2enc handle", S_POINTER_PARAM_TYPE, &handle, 0)) {
    return SimpegWrite_encode_bitmap(handle, s_image_data(image));
  }
  return 0;
}

void
mpeg2enc_movie_close(s_movie * movie)
{
  void * handle;
  if (s_params_get(s_movie_params(movie), "mpeg2enc handle", S_POINTER_PARAM_TYPE, &handle, 0)) {
    SimpegWrite_end_encode(handle);
  }
}


/* private prototypes */
static void init(simpeg_encode_context * context);
static void init_context_data(simpeg_encode_context * context);
static void readparmfile(simpeg_encode_context * context, const char *fname,
                         int w, int h, int numframes, int mpeg1, int level, float bitrate);
static void readquantmat(simpeg_encode_context * context);
static void cleanup(simpeg_encode_context * context);



/* fixme: statfile -> ignore */

static int
SimpegWrite_encode(const char *output_filename,
                   const char *parameter_filename,
                   SimpegWrite_error_cb error_cb,
                   SimpegWrite_warning_cb warning_cb,
                   SimpegWrite_progress_cb progress_cb,
                   void * cbuserdata)
{
  simpeg_encode_context * context;

  context = (simpeg_encode_context*) malloc(sizeof(simpeg_encode_context));

  if (setjmp(context->jmpbuf)) {
    /* error if we get here */
    cleanup(context);
    free(context);
    return 0;
  }

  init_context_data(context);

  context->SimpegWrite_error_cb_user = error_cb;
  context->SimpegWrite_warning_cb_user = warning_cb;
  context->SimpegWrite_progress_cb_user = progress_cb;
  context->cbuserdata = cbuserdata;

  context->SimpegWrite_current_frame = 0;

  context->read_ppm_u444 = NULL;
  context->read_ppm_v444 = NULL;
  context->read_ppm_u422 = NULL;
  context->read_ppm_v422 = NULL;


  /* read parameter file */
  readparmfile(context, parameter_filename, 0, 0, 0, 0, 0, 0.0f);

  /* read quantization matrices */
  readquantmat(context);

  /* open output file */
  if (!(context->outfile=fopen(output_filename,"wb")))
  {
    /* FIXME: can overflow context->errortext buffer. 20010918 mortene. */
    sprintf(context->errortext,"Couldn't create output file %s",output_filename);
    simpeg_encode_error(context,context->errortext);
  }

  init(context);

  simpeg_encode_putseq(context);

  cleanup(context);
  free(context);

  return 1; /* ok */
}


static void *
SimpegWrite_begin_encode(const char *output_filename,
                         const char *parameter_filename,
                         SimpegWrite_error_cb error_cb,
                         SimpegWrite_warning_cb warning_cb,
                         SimpegWrite_progress_cb progress_cb,
                         void * cbuserdata,
                         int w, int h, int numframes, int mpeg1, int level, float bitrate)
{
  int i;
  simpeg_encode_context * context;

  context = (simpeg_encode_context*) malloc(sizeof(simpeg_encode_context));
  init_context_data(context);

  if (setjmp(context->jmpbuf)) {
    /* error if we get here */
    cleanup(context);
    free(context);
    return NULL;
  }

  context->SimpegWrite_error_cb_user = error_cb;
  context->SimpegWrite_warning_cb_user = warning_cb;
  context->SimpegWrite_progress_cb_user = progress_cb;
  context->cbuserdata = cbuserdata;

  context->SimpegWrite_current_frame = 0;
  context->SimpegWrite_current_input_frame = 0;

  context->read_ppm_u444 = NULL;
  context->read_ppm_v444 = NULL;
  context->read_ppm_u422 = NULL;
  context->read_ppm_v422 = NULL;

  context->bufbuf = NULL;

  /* read parameter file */
  readparmfile(context, parameter_filename, w, h, numframes, mpeg1, level, bitrate);

  /* read quantization matrices */
  readquantmat(context);

  /* open output file */
  if (!(context->outfile=fopen(output_filename,"wb")))
    {
    sprintf(context->errortext,"Couldn't create output file %s",output_filename);
    simpeg_encode_error(context, context->errortext);
  }

  init(context);

  SimpegWrite_putseq_begin(context);

  context->bufbuf = (unsigned char **)
    malloc(sizeof(unsigned char *) * context->M);
  for (i=0; i<context->M; i++)
    context->bufbuf[i] = NULL;

  return (void*) context;
}

static int
SimpegWrite_encode_bitmap(void * handle, const unsigned char *rgb_buffer)
{
  int i;
  simpeg_encode_context * context = (simpeg_encode_context*) handle;

  if (setjmp(context->jmpbuf)) {
    /* error if we get here */
    return 0;
  }

  if ((context->SimpegWrite_current_input_frame)%context->M == 0) {
    /* encode this */
    SimpegWrite_putseq_encode_bitmap((simpeg_encode_context*) handle, rgb_buffer);
    /* encode the buffered ones */
    for (i = 0; i < context->M; i++) {
      if (context->bufbuf[i] != NULL)
        SimpegWrite_putseq_encode_bitmap((simpeg_encode_context*) handle,
                                         context->bufbuf[i]);
    }
    /* clean buffer */
    for (i = 0; i < context->M; i++) {
      if (context->bufbuf[i] != NULL) {
        free(context->bufbuf[i]);
        context->bufbuf[i] = NULL;
      }
    }
  }
  else {
    context->bufbuf[(context->SimpegWrite_current_input_frame)%context->M] =
      (unsigned char *) malloc(context->vertical_size * context->horizontal_size * 3);
    memcpy(context->bufbuf[(context->SimpegWrite_current_input_frame)%context->M],
           rgb_buffer, context->vertical_size * context->horizontal_size * 3);
  }

  context->SimpegWrite_current_input_frame++;

  return 1; /* ok */
}

static int
SimpegWrite_end_encode(void * handle)
{
  int i;
  simpeg_encode_context * context = (simpeg_encode_context*) handle;

  if (setjmp(context->jmpbuf)) {
    /* error if we get here */
    cleanup(context);
    free(context);
    return 0;
  }

  /* before we're done - encode any buffered frames */
  for (i=0; i<context->M; i++) {
    if (context->bufbuf[i] != NULL) {
      SimpegWrite_putseq_encode_bitmap((simpeg_encode_context*) handle,
                                       context->bufbuf[i]);
    }
  }

  /* finish sequence */

  SimpegWrite_putseq_end(context);

  cleanup(context);
  free(context);
  return 1;
}

static void
cleanup(simpeg_encode_context * context)
{
  int i;

  /* cleanup after mallocs in readpic.c read_ppm() */
  if (context->chroma_format==CHROMA444) {
    /* do nothing */
  }
  else {
    if (context->read_ppm_u444 != NULL)
      free(context->read_ppm_u444);
    if (context->read_ppm_v444 != NULL)
      free(context->read_ppm_v444);
    if (context->chroma_format==CHROMA420) {
      if (context->read_ppm_u422 != NULL)
        free(context->read_ppm_u422);
      if (context->read_ppm_v422 != NULL)
        free(context->read_ppm_v422);
    }
  }

  if (context->outfile) {
    fclose(context->outfile);
    context->outfile = NULL;
  }
  if (context->statfile) {
    fclose(context->statfile);
    context->statfile = NULL;
  }

  if (context->mbinfo) free(context->mbinfo);
  if (context->motion_data) free(context->motion_data);
  if (context->blocks) free(context->blocks);
  if (context->clp_org) free(context->clp_org);

  for (i=0; i < 3; i++) {
    if (context->newrefframe[i]) free(context->newrefframe[i]);
    if (context->oldrefframe[i]) free(context->oldrefframe[i]);
    if (context->predframe[i]) free(context->predframe[i]);
    if (context->auxframe[i]) free(context->auxframe[i]);
    if (context->auxorgframe[i]) free(context->auxorgframe[i]);
    if (context->neworgframe[i]) free(context->neworgframe[i]);
    if (context->oldorgframe[i]) free(context->oldorgframe[i]);
  }

  if (context->bufbuf != NULL) {
    for (i=0; i<context->M; i++) {
      if (context->bufbuf[i] != NULL) {
        free(context->bufbuf[i]);
      }
    }
    free(context->bufbuf);
    context->bufbuf = NULL;
  }
}


void SimpegWrite_error(simpeg_encode_context * context, const char *text, ...)
{
  if (context->SimpegWrite_error_cb_user != NULL) {
    char buf[256];
    va_list p;
    va_start(p, text);
    vsprintf(buf, text, p);
    va_end(p);

    context->SimpegWrite_error_cb_user(context->cbuserdata, buf);
  }
  longjmp(context->jmpbuf, 1);
}

void SimpegWrite_warning(simpeg_encode_context * context,const char *text, ...)
{
  if (context->SimpegWrite_warning_cb_user != NULL) {
    char buf[256];
    va_list p;
    va_start(p, text);
    vsprintf(buf, text, p);
    va_end(p);

    context->SimpegWrite_warning_cb_user(context->cbuserdata, buf);
  }
}

int SimpegWrite_progress(simpeg_encode_context * context, float sub, int current_frame, int num_frames)
{
  if (context->SimpegWrite_progress_cb_user != NULL)
    return context->SimpegWrite_progress_cb_user(context->cbuserdata, sub, current_frame, num_frames);
  else
    return 1;
}


static void
init(simpeg_encode_context * context)
{
  int i, size;
  static int block_count_tab[3] = {6,8,12};

  simpeg_encode_initbits(context);
  simpeg_encode_init_fdct(context);
  simpeg_encode_init_idct(context);

  /* round picture dimensions to nearest multiple of 16 or 32 */
  context->mb_width = (context->horizontal_size+15)/16;
  context->mb_height = context->prog_seq ? (context->vertical_size+15)/16 : 2*((context->vertical_size+31)/32);
  context->mb_height2 = context->fieldpic ? context->mb_height>>1 : context->mb_height; /* for field pictures */
  context->width = 16*context->mb_width;
  context->height = 16*context->mb_height;

  context->chrom_width = (context->chroma_format==CHROMA444) ? context->width : context->width>>1;
  context->chrom_height = (context->chroma_format!=CHROMA420) ? context->height : context->height>>1;

  context->height2 = context->fieldpic ? context->height>>1 : context->height;
  context->width2 = context->fieldpic ? context->width<<1 : context->width;
  context->chrom_width2 = context->fieldpic ? context->chrom_width<<1 : context->chrom_width;

  context->block_count = block_count_tab[context->chroma_format-1];

  /* clip table */
  if (!(context->clp = (unsigned char *)malloc(1024)))
    simpeg_encode_error(context, "malloc failed\n");
  context->clp_org = context->clp;
  context->clp+= 384;
  for (i=-384; i<640; i++)
    context->clp[i] = (i<0) ? 0 : ((i>255) ? 255 : i);

  for (i=0; i < 3; i++) {
    size = (i==0) ? context->width*context->height : context->chrom_width*context->chrom_height;

    if (!(context->newrefframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
    if (!(context->oldrefframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
    if (!(context->auxframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
    if (!(context->neworgframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context,"malloc failed\n");
    if (!(context->oldorgframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
    if (!(context->auxorgframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
    if (!(context->predframe[i] = (unsigned char *)malloc(size)))
      simpeg_encode_error(context, "malloc failed\n");
  }

  context->mbinfo = (struct mbinfo *)malloc(context->mb_width*context->mb_height2*sizeof(struct mbinfo));

  if (!context->mbinfo)
    simpeg_encode_error(context,"malloc failed\n");

  context->blocks =
    (short (*)[64])malloc(context->mb_width*context->mb_height2*context->block_count*sizeof(short [64]));

  if (!context->blocks)
    simpeg_encode_error(context, "malloc failed\n");

  /* open statistics output file */
  if (context->statname[0]=='-')
    context->statfile = stdout;
  else if (context->statname[0]=='%') /* 20010516 thammer we don't want to output to a statfile */
    context->statfile = NULL;
  else if (!(context->statfile = fopen(context->statname,"w")))
  {
    sprintf(context->errortext,"Couldn't create statistics output file %s",context->statname);
    simpeg_encode_error(context,context->errortext);
  }
}

void simpeg_encode_error(simpeg_encode_context * context, char *text)
{
  SimpegWrite_error(context, text);
}

static void
readparmfile(simpeg_encode_context * context,
             const char *fname, int width, int height, int numframes, int mpeg1, int level, float bitrate)
{
  int i;
  int h, m, s, f;
  FILE * fd;
  char line[256];
  static double ratetab[8]=
    {24000.0/1001.0,24.0,25.0,30000.0/1001.0,30.0,50.0,60000.0/1001.0,60.0};

  if (fname) {
    if (!(fd = fopen(fname,"r"))) {
      sprintf(context->errortext,"Couldn't open parameter file %s",fname);
        simpeg_encode_error(context, context->errortext);
    }

    fgets(context->id_string,254,fd);
    fgets(line,254,fd); sscanf(line,"%s",context->tplorg);
    fgets(line,254,fd); sscanf(line,"%s",context->tplref);
    fgets(line,254,fd); sscanf(line,"%s",context->iqname);
    fgets(line,254,fd); sscanf(line,"%s",context->niqname);
    fgets(line,254,fd); sscanf(line,"%s",context->statname);
    fgets(line,254,fd); sscanf(line,"%d",&context->inputtype);
    fgets(line,254,fd); sscanf(line,"%d",&context->nframes);
    if (numframes > 0) context->nframes = numframes;
    fgets(line,254,fd); sscanf(line,"%d",&context->frame0);
    fgets(line,254,fd); sscanf(line,"%d:%d:%d:%d",&h,&m,&s,&f);
    fgets(line,254,fd); sscanf(line,"%d",&context->N);
    fgets(line,254,fd); sscanf(line,"%d",&context->M);
    fgets(line,254,fd); sscanf(line,"%d",&context->mpeg1);
    fgets(line,254,fd); sscanf(line,"%d",&context->fieldpic);
    fgets(line,254,fd); sscanf(line,"%d",&context->horizontal_size);
    fgets(line,254,fd); sscanf(line,"%d",&context->vertical_size);
    if (width > 0 && height > 0) {
      context->horizontal_size = width;
      context->vertical_size = height;
    }
    fgets(line,254,fd); sscanf(line,"%d",&context->aspectratio);
    fgets(line,254,fd); sscanf(line,"%d",&context->frame_rate_code);
    fgets(line,254,fd); sscanf(line,"%lf",&context->bit_rate);
    fgets(line,254,fd); sscanf(line,"%d",&context->vbv_buffer_size);
    fgets(line,254,fd); sscanf(line,"%d",&context->low_delay);
    fgets(line,254,fd); sscanf(line,"%d",&context->constrparms);
    fgets(line,254,fd); sscanf(line,"%d",&context->profile);
    fgets(line,254,fd); sscanf(line,"%d",&context->level);
    if (level>0) context->level = level;
    fgets(line,254,fd); sscanf(line,"%d",&context->prog_seq);
    fgets(line,254,fd); sscanf(line,"%d",&context->chroma_format);
    fgets(line,254,fd); sscanf(line,"%d",&context->video_format);
    fgets(line,254,fd); sscanf(line,"%d",&context->color_primaries);
    fgets(line,254,fd); sscanf(line,"%d",&context->transfer_characteristics);
    fgets(line,254,fd); sscanf(line,"%d",&context->matrix_coefficients);
    fgets(line,254,fd); sscanf(line,"%d",&context->display_horizontal_size);
    fgets(line,254,fd); sscanf(line,"%d",&context->display_vertical_size);
    if (width > 0 && height > 0) {
      context->display_horizontal_size = width;
      context->display_vertical_size = height;
    }

    fgets(line,254,fd); sscanf(line,"%d",&context->dc_prec);
    fgets(line,254,fd); sscanf(line,"%d",&context->topfirst);
    fgets(line,254,fd); sscanf(line,"%d %d %d",
                               context->frame_pred_dct_tab,
                               context->frame_pred_dct_tab+1,
                               context->frame_pred_dct_tab+2);

    fgets(line,254,fd); sscanf(line,"%d %d %d",
                               context->conceal_tab,
                               context->conceal_tab+1,
                               context->conceal_tab+2);

    fgets(line,254,fd); sscanf(line,"%d %d %d",
                               context->qscale_tab,
                               context->qscale_tab+1,
                               context->qscale_tab+2);

    fgets(line,254,fd); sscanf(line,"%d %d %d",
                               context->intravlc_tab,
                               context->intravlc_tab+1,
                               context->intravlc_tab+2);
    fgets(line,254,fd); sscanf(line,"%d %d %d",
                               context->altscan_tab,
                               context->altscan_tab+1,
                               context->altscan_tab+2);

    fgets(line,254,fd); sscanf(line,"%d",&context->repeatfirst);
    fgets(line,254,fd); sscanf(line,"%d",&context->prog_frame);
    /* intra slice interval refresh period */
    fgets(line,254,fd); sscanf(line,"%d",&context->P);
    fgets(line,254,fd); sscanf(line,"%d",&context->r);
    fgets(line,254,fd); sscanf(line,"%lf",&context->avg_act);
    fgets(line,254,fd); sscanf(line,"%d",&context->Xi);
    fgets(line,254,fd); sscanf(line,"%d",&context->Xp);
    fgets(line,254,fd); sscanf(line,"%d",&context->Xb);
    fgets(line,254,fd); sscanf(line,"%d",&context->d0i);
    fgets(line,254,fd); sscanf(line,"%d",&context->d0p);
    fgets(line,254,fd); sscanf(line,"%d",&context->d0b);

    if (context->N<1)
      simpeg_encode_error(context, "N must be positive");
    if (context->M<1)
      simpeg_encode_error(context,"M must be positive");
    if (context->N%context->M != 0)
      simpeg_encode_error(context,"N must be an integer multiple of M");

    context->motion_data = (struct motion_data *)malloc(context->M*sizeof(struct motion_data));
    if (!context->motion_data)
      simpeg_encode_error(context, "malloc failed\n");

    for (i=0; i<context->M; i++) {
      fgets(line,254,fd);
      sscanf(line,"%d %d %d %d",
             &context->motion_data[i].forw_hor_f_code,
             &context->motion_data[i].forw_vert_f_code,
             &context->motion_data[i].sxf,
             &context->motion_data[i].syf);

      if (i!=0) {
        fgets(line,254,fd);
        sscanf(line,"%d %d %d %d",
               &context->motion_data[i].back_hor_f_code,
               &context->motion_data[i].back_vert_f_code,
               &context->motion_data[i].sxb,
               &context->motion_data[i].syb);
      }
    }

    fclose(fd);
  }
  else  if (mpeg1) { /* fill in some default values for mpeg1 */
    strcpy(context->id_string, "MPEG-1 sequence, created using simage (www.coin3d.org)");
    strcpy(context->tplorg, "orgimage%d");
    strcpy(context->tplref, "-");
    strcpy(context->iqname, "-");
    strcpy(context->niqname, "-");
    strcpy(context->statname, "%");
    context->inputtype = 3;
    context->nframes = 10;
    if (numframes > 0) context->nframes = numframes;
    context->frame0 = 0;
    h = m = s = f = 0;

    context->N = 6;
    context->M = 3;

    context->mpeg1 = 1;
    context->fieldpic = 0;

    context->horizontal_size = 704;
    context->vertical_size = 480;

    if (width > 0 && height > 0) {
      context->horizontal_size = width;
      context->vertical_size = height;
    }

    context->aspectratio = 2;
    context->frame_rate_code = 3;
    context->bit_rate = 1152000.0;
    if (bitrate > 0.0f) context->bit_rate = bitrate;
    context->vbv_buffer_size = 20;
    context->low_delay = 0;
    context->constrparms = 0;
    context->profile = 4;
    context->level = 8;
    if (level>0) context->level = level;
    context->prog_seq = 1;
    context->chroma_format = 1;
    context->video_format = 2;
    context->color_primaries = 5;
    context->transfer_characteristics = 5;
    context->matrix_coefficients = 5;
    context->display_horizontal_size = 704;
    context->display_vertical_size = 480;

    if (width > 0 && height > 0) {
      context->display_horizontal_size = width;
      context->display_vertical_size = height;
    }

    context->dc_prec = 0;
    context->topfirst = 0;
    context->frame_pred_dct_tab[0] = 1;
    context->frame_pred_dct_tab[1] = 1;
    context->frame_pred_dct_tab[2] = 1;

    context->conceal_tab[0] = 0;
    context->conceal_tab[1] = 0;
    context->conceal_tab[2] = 0;

    context->qscale_tab[0] = 0;
    context->qscale_tab[1] = 0;
    context->qscale_tab[2] = 0;

    context->intravlc_tab[0] = 0;
    context->intravlc_tab[1] = 0;
    context->intravlc_tab[2] = 0;

    context->altscan_tab[0] = 0;
    context->altscan_tab[1] = 0;
    context->altscan_tab[2] = 0;

    context->repeatfirst = 0;
    context->prog_frame = 1;

    context->P = 0;
    context->r = 0;
    context->avg_act = 0;
    context->Xi = 0;
    context->Xp = 0;
    context->Xb = 0;
    context->d0i = 0;
    context->d0p = 0;
    context->d0b = 0;

    if (context->N<1)
      simpeg_encode_error(context, "N must be positive");
    if (context->M<1)
      simpeg_encode_error(context,"M must be positive");
    if (context->N%context->M != 0)
      simpeg_encode_error(context,"N must be an integer multiple of M");

    context->motion_data = (struct motion_data *)malloc(context->M*sizeof(struct motion_data));
    if (!context->motion_data)
      simpeg_encode_error(context, "malloc failed\n");

    context->motion_data[0].forw_hor_f_code = 2;
    context->motion_data[0].forw_vert_f_code = 2;
    context->motion_data[0].sxf = 11;
    context->motion_data[0].syf = 11;

    context->motion_data[1].forw_hor_f_code = 1;
    context->motion_data[1].forw_vert_f_code = 1;
    context->motion_data[1].sxf = 3;
    context->motion_data[1].syf = 3;

    context->motion_data[1].back_hor_f_code = 1;
    context->motion_data[1].back_vert_f_code = 1;
    context->motion_data[1].sxb = 7;
    context->motion_data[1].syb = 7;

    context->motion_data[2].forw_hor_f_code = 1;
    context->motion_data[2].forw_vert_f_code = 1;
    context->motion_data[2].sxf = 7;
    context->motion_data[2].syf = 7;

    context->motion_data[2].back_hor_f_code = 1;
    context->motion_data[2].back_vert_f_code = 1;
    context->motion_data[2].sxb = 3;
    context->motion_data[2].syb = 3;
  }
  else { /* fill in some default values for mpeg2 */
    strcpy(context->id_string, "MPEG-2 sequence, created using simage (www.coin3d.org)");
    strcpy(context->tplorg, "orgimage%d");
    strcpy(context->tplref, "-");
    strcpy(context->iqname, "-");
    strcpy(context->niqname, "-");
    strcpy(context->statname, "%");
    context->inputtype = 3;
    context->nframes = 10;
    if (numframes > 0) context->nframes = numframes;
    context->frame0 = 0;
    h = m = s = f = 0;

    context->N = 6;
    context->M = 3;

    context->mpeg1 = 0;
    context->fieldpic = 0;

    context->horizontal_size = 704;
    context->vertical_size = 480;

    if (width > 0 && height > 0) {
      context->horizontal_size = width;
      context->vertical_size = height;
    }

    context->aspectratio = 2;
    context->frame_rate_code = 5;
    context->bit_rate = 5000000.0;
    if (bitrate > 0.0f) context->bit_rate = bitrate;
    context->vbv_buffer_size = 112;
    context->low_delay = 0;
    context->constrparms = 0;
    context->profile = 4;
    context->level = 8;
    if (level>0) context->level = level;
    context->prog_seq = 0;
    context->chroma_format = 1;
    context->video_format = 2;
    context->color_primaries = 5;
    context->transfer_characteristics = 5;
    context->matrix_coefficients = 4;
    context->display_horizontal_size = 704;
    context->display_vertical_size = 480;

    if (width > 0 && height > 0) {
      context->display_horizontal_size = width;
      context->display_vertical_size = height;
    }

    context->dc_prec = 0;
    context->topfirst = 1;
    context->frame_pred_dct_tab[0] = 0;
    context->frame_pred_dct_tab[1] = 0;
    context->frame_pred_dct_tab[2] = 0;

    context->conceal_tab[0] = 0;
    context->conceal_tab[1] = 0;
    context->conceal_tab[2] = 0;

    context->qscale_tab[0] = 1;
    context->qscale_tab[1] = 1;
    context->qscale_tab[2] = 1;

    context->intravlc_tab[0] = 1;
    context->intravlc_tab[1] = 0;
    context->intravlc_tab[2] = 0;

    context->altscan_tab[0] = 0;
    context->altscan_tab[1] = 0;
    context->altscan_tab[2] = 0;

    context->repeatfirst = 0;
    context->prog_frame = 0;

    context->P = 0;
    context->r = 0;
    context->avg_act = 0;
    context->Xi = 0;
    context->Xp = 0;
    context->Xb = 0;
    context->d0i = 0;
    context->d0p = 0;
    context->d0b = 0;

    if (context->N<1)
      simpeg_encode_error(context, "N must be positive");
    if (context->M<1)
      simpeg_encode_error(context,"M must be positive");
    if (context->N%context->M != 0)
      simpeg_encode_error(context,"N must be an integer multiple of M");

    context->motion_data = (struct motion_data *)malloc(context->M*sizeof(struct motion_data));
    if (!context->motion_data)
      simpeg_encode_error(context, "malloc failed\n");

    context->motion_data[0].forw_hor_f_code = 2;
    context->motion_data[0].forw_vert_f_code = 2;
    context->motion_data[0].sxf = 11;
    context->motion_data[0].syf = 11;

    context->motion_data[1].forw_hor_f_code = 1;
    context->motion_data[1].forw_vert_f_code = 1;
    context->motion_data[1].sxf = 3;
    context->motion_data[1].syf = 3;

    context->motion_data[1].back_hor_f_code = 1;
    context->motion_data[1].back_vert_f_code = 1;
    context->motion_data[1].sxb = 7;
    context->motion_data[1].syb = 7;

    context->motion_data[2].forw_hor_f_code = 1;
    context->motion_data[2].forw_vert_f_code = 1;
    context->motion_data[2].sxf = 7;
    context->motion_data[2].syf = 7;

    context->motion_data[2].back_hor_f_code = 1;
    context->motion_data[2].back_vert_f_code = 1;
    context->motion_data[2].sxb = 3;
    context->motion_data[2].syb = 3;
  }

  /* make flags boolean (x!=0 -> x=1) */
  context->mpeg1 = !!context->mpeg1;
  context->fieldpic = !!context->fieldpic;
  context->low_delay = !!context->low_delay;
  context->constrparms = !!context->constrparms;
  context->prog_seq = !!context->prog_seq;
  context->topfirst = !!context->topfirst;

  for (i = 0; i < 3; i++) {
    context->frame_pred_dct_tab[i] = !!context->frame_pred_dct_tab[i];
    context->conceal_tab[i] = !!context->conceal_tab[i];
    context->qscale_tab[i] = !!context->qscale_tab[i];
    context->intravlc_tab[i] = !!context->intravlc_tab[i];
    context->altscan_tab[i] = !!context->altscan_tab[i];
  }
  context->repeatfirst = !!context->repeatfirst;
  context->prog_frame = !!context->prog_frame;

  /* make sure MPEG specific parameters are valid */
  simpeg_encode_range_checks(context);

  context->frame_rate = ratetab[context->frame_rate_code-1];

  /* timecode -> frame number */
  context->tc0 = h;
  context->tc0 = 60*context->tc0 + m;
  context->tc0 = 60*context->tc0 + s;
  context->tc0 = (int)(context->frame_rate+0.5)*context->tc0 + f;

  if (!context->mpeg1) {
    simpeg_encode_profile_and_level_checks(context);
  }
  else {
    /* MPEG-1 */
    if (context->constrparms) {
      if (context->horizontal_size > 768
          || context->vertical_size > 576
          || ((context->horizontal_size+15)/16)*((context->vertical_size+15)/16)>396
          || ((context->horizontal_size+15)/16)*((context->vertical_size+15)/16)*context->frame_rate>396*25.0
          || context->frame_rate>30.0) {
        if (!context->quiet)
          SimpegWrite_warning(context, "setting constrained_parameters_flag = 0");
        context->constrparms = 0;
      }
    }

    if (context->constrparms) {
      for (i=0; i<context->M; i++) {
        if (context->motion_data[i].forw_hor_f_code > 4) {
          if (!context->quiet)
            SimpegWrite_warning(context,"setting constrained_parameters_flag = 0");
          context->constrparms = 0;
          break;
        }

        if (context->motion_data[i].forw_vert_f_code > 4) {
          if (!context->quiet)
            SimpegWrite_warning(context,"setting constrained_parameters_flag = 0");
          context->constrparms = 0;
          break;
        }

        if (i!=0) {
          if (context->motion_data[i].back_hor_f_code>4) {
            if (!context->quiet)
              SimpegWrite_warning(context,"setting constrained_parameters_flag = 0");
            context->constrparms = 0;
            break;
          }

          if (context->motion_data[i].back_vert_f_code>4) {
            if (!context->quiet)
              SimpegWrite_warning(context,"setting constrained_parameters_flag = 0");
            context->constrparms = 0;
            break;
          }
        }
      }
    }
  }

  /* relational checks */

  if (context->mpeg1) {
    if (!context->prog_seq) {
      if (!context->quiet)
        SimpegWrite_warning(context,"setting progressive_sequence = 1");
      context->prog_seq = 1;
    }

    if (context->chroma_format!=CHROMA420) {
      if (!context->quiet)
        SimpegWrite_warning(context,"setting chroma_format = 1 (4:2:0)");
      context->chroma_format = CHROMA420;
    }

    if (context->dc_prec!=0) {
      if (!context->quiet)
        SimpegWrite_warning(context,"setting intra_dc_precision = 0");
      context->dc_prec = 0;
    }

    for (i=0; i<3; i++)
      if (context->qscale_tab[i]) {
        if (!context->quiet)
          SimpegWrite_warning(context,"setting qscale_tab[%d] = 0", i);
        context->qscale_tab[i] = 0;
      }

    for (i=0; i<3; i++)
      if (context->intravlc_tab[i]) {
        if (!context->quiet)
          SimpegWrite_warning(context,"setting intravlc_tab[%d] = 0", i);
        context->intravlc_tab[i] = 0;
      }

    for (i=0; i<3; i++)
      if (context->altscan_tab[i]) {
        if (!context->quiet)
          SimpegWrite_warning(context,"setting altscan_tab[%d] = 0", i);
        context->altscan_tab[i] = 0;
      }
  }

  if (!context->mpeg1 && context->constrparms) {
    if (!context->quiet)
      SimpegWrite_warning(context,"setting constrained_parameters_flag = 0");
    context->constrparms = 0;
  }

  if (context->prog_seq && !context->prog_frame) {
    if (!context->quiet)
      SimpegWrite_warning(context,"setting progressive_frame = 1");
    context->prog_frame = 1;
  }

  if (context->prog_frame && context->fieldpic) {
    if (!context->quiet)
      SimpegWrite_warning(context,"setting field_pictures = 0");
    context->fieldpic = 0;
  }

  if (!context->prog_frame && context->repeatfirst) {
    if (!context->quiet)
      SimpegWrite_warning(context,"setting repeat_first_field = 0");
    context->repeatfirst = 0;
  }

  if (context->prog_frame) {
    for (i=0; i<3; i++)
      if (!context->frame_pred_dct_tab[i]) {
        if (!context->quiet)
          SimpegWrite_warning(context,"setting frame_pred_frame_dct[%d] = 1", i);
        context->frame_pred_dct_tab[i] = 1;
      }
  }

  if (context->prog_seq && !context->repeatfirst && context->topfirst) {
    if (!context->quiet)
      SimpegWrite_warning(context,"setting top_field_first = 0");
    context->topfirst = 0;
  }

  /* search windows */
  for (i=0; i<context->M; i++) {
    if (context->motion_data[i].sxf > (4<<context->motion_data[i].forw_hor_f_code)-1) {
      if (!context->quiet)
        SimpegWrite_warning(context,
                            "Warning: reducing forward horizontal search width to %d",
                            (4<<context->motion_data[i].forw_hor_f_code)-1);
      context->motion_data[i].sxf = (4<<context->motion_data[i].forw_hor_f_code)-1;
    }

    if (context->motion_data[i].syf > (4<<context->motion_data[i].forw_vert_f_code)-1) {
      if (!context->quiet)
        SimpegWrite_warning(context,
                            "Warning: reducing forward vertical search width to %d",
                            (4<<context->motion_data[i].forw_vert_f_code)-1);
      context->motion_data[i].syf = (4<<context->motion_data[i].forw_vert_f_code)-1;
    }

    if (i!=0) {
      if (context->motion_data[i].sxb > (4<<context->motion_data[i].back_hor_f_code)-1) {
        if (!context->quiet)
          SimpegWrite_warning(context,
            "Warning: reducing backward horizontal search width to %d",
            (4<<context->motion_data[i].back_hor_f_code)-1);
        context->motion_data[i].sxb = (4<<context->motion_data[i].back_hor_f_code)-1;
      }

      if (context->motion_data[i].syb > (4<<context->motion_data[i].back_vert_f_code)-1) {
        if (!context->quiet)
          SimpegWrite_warning(context,
                              "Warning: reducing backward vertical search width to %d",
                              (4<<context->motion_data[i].back_vert_f_code)-1);
        context->motion_data[i].syb = (4<<context->motion_data[i].back_vert_f_code)-1;
      }
    }
  }
}

static void readquantmat(simpeg_encode_context * context)
{
  int i,v;
  FILE *fd;

  if (context->iqname[0]=='-') {
    /* use default intra matrix */
    context->load_iquant = 0;
    for (i=0; i<64; i++)
      context->intra_q[i] = context->default_intra_quantizer_matrix[i];
  }
  else {
    /* read customized intra matrix */
    context->load_iquant = 1;
    if (!(fd = fopen(context->iqname,"r"))) {
      sprintf(context->errortext,"Couldn't open quant matrix file %s",context->iqname);
      simpeg_encode_error(context,context->errortext);
    }

    for (i=0; i<64; i++) {
      fscanf(fd,"%d",&v);
      if (v<1 || v>255)
        simpeg_encode_error(context,"invalid value in quant matrix");
      context->intra_q[i] = v;
    }

    fclose(fd);
  }

  if (context->niqname[0]=='-') {
    /* use default non-intra matrix */
    context->load_niquant = 0;
    for (i=0; i<64; i++)
      context->inter_q[i] = 16;
  }
  else {
    /* read customized non-intra matrix */
    context->load_niquant = 1;
    if (!(fd = fopen(context->niqname,"r"))) {
      sprintf(context->errortext,"Couldn't open quant matrix file %s",context->niqname);
      simpeg_encode_error(context, context->errortext);
    }

    for (i=0; i<64; i++) {
      fscanf(fd,"%d",&v);
      if (v<1 || v>255)
        simpeg_encode_error(context,"invalid value in quant matrix");
      context->inter_q[i] = v;
    }

    fclose(fd);
  }
}

static void init_context_data(simpeg_encode_context * context)
{
  static unsigned char zig_zag_scan[] =
  {
    0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
    12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
    35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
    58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
  };

  static unsigned char alternate_scan[] =
  {
    0,8,16,24,1,9,2,10,17,25,32,40,48,56,57,49,
    41,33,26,18,3,11,4,12,19,27,34,42,50,58,35,43,
    51,59,20,28,5,13,6,14,21,29,36,44,52,60,37,45,
    53,61,22,30,7,15,23,31,38,46,54,62,39,47,55,63
  };
  static unsigned char default_intra_quantizer_matrix[] =
  {
    8, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83
  };
  static unsigned char non_linear_mquant_table[] =
  {
    0, 1, 2, 3, 4, 5, 6, 7,
    8,10,12,14,16,18,20,22,
    24,28,32,36,40,44,48,52,
    56,64,72,80,88,96,104,112
  };
  static unsigned char map_non_linear_mquant[] =
  {
    0,1,2,3,4,5,6,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,
    16,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,22,22,
    22,22,23,23,23,23,24,24,24,24,24,24,24,25,25,25,25,25,25,25,26,26,
    26,26,26,26,26,26,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,29,
      29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,31,31,31,31,31
  };

  /* initialize integers and pointers to 0 */
  memset(context, 0, sizeof(simpeg_encode_context));

  strcpy(context->version, "mpeg2encode V1.2, 96/07/19");
  strcpy(context->author,"(C) 1996, MPEG Software Simulation Group");


  memcpy(context->zig_zag_scan, zig_zag_scan, sizeof(zig_zag_scan));


  memcpy(context->alternate_scan, alternate_scan, sizeof(alternate_scan));

  memcpy(context->default_intra_quantizer_matrix,
         default_intra_quantizer_matrix,
         sizeof(default_intra_quantizer_matrix));

  memcpy(context->non_linear_mquant_table,
         non_linear_mquant_table,
         sizeof(non_linear_mquant_table));

  memcpy(context->map_non_linear_mquant,
         map_non_linear_mquant,
         sizeof(map_non_linear_mquant));

  /* just initialize doubles to 0 */
  context->frame_rate = 0.0;
  context->bit_rate = 0.0;
  context->avg_act = 0.0;
  context->actsum = 0.0;

  context->next_ip_delay = 0.0;
  context->decoding_time = 0.0;

  context->statname[0] = '%';
}
