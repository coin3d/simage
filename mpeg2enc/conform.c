/* conform.c, conformance checks                                            */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"

/* check for (level independent) parameter limits */
void simpeg_encode_range_checks(simpeg_encode_context * context)
{
  int i;

  /* range and value checks */

  if (context->horizontal_size<1 || context->horizontal_size>16383)
    simpeg_encode_error(context, "horizontal_size must be between 1 and 16383");
  if (context->mpeg1 && context->horizontal_size>4095)
    simpeg_encode_error(context, "horizontal_size must be less than 4096 (MPEG-1)");
  if ((context->horizontal_size&4095)==0)
    simpeg_encode_error(context, "horizontal_size must not be a multiple of 4096");
  if (context->chroma_format!=CHROMA444 && context->horizontal_size%2 != 0)
    simpeg_encode_error(context, "horizontal_size must be a even (4:2:0 / 4:2:2)");

  if (context->vertical_size<1 || context->vertical_size>16383)
    simpeg_encode_error(context, "vertical_size must be between 1 and 16383");
  if (context->mpeg1 && context->vertical_size>4095)
    simpeg_encode_error(context, "vertical size must be less than 4096 (MPEG-1)");
  if ((context->vertical_size&4095)==0)
    simpeg_encode_error(context, "vertical_size must not be a multiple of 4096");
  if (context->chroma_format==CHROMA420 && context->vertical_size%2 != 0)
    simpeg_encode_error(context, "vertical_size must be a even (4:2:0)");
  if(context->fieldpic)
  {
    if (context->vertical_size%2 != 0)
      simpeg_encode_error(context, "vertical_size must be a even (field pictures)");
    if (context->chroma_format==CHROMA420 && context->vertical_size%4 != 0)
      simpeg_encode_error(context, "vertical_size must be a multiple of 4 (4:2:0 field pictures)");
  }

  if (context->mpeg1)
  {
    if (context->aspectratio<1 || context->aspectratio>14)
      simpeg_encode_error(context, "pel_aspect_ratio must be between 1 and 14 (MPEG-1)");
  }
  else
  {
    if (context->aspectratio<1 || context->aspectratio>4)
      simpeg_encode_error(context, "aspect_ratio_information must be 1, 2, 3 or 4");
  }

  if (context->frame_rate_code<1 || context->frame_rate_code>8)
    simpeg_encode_error(context, "frame_rate code must be between 1 and 8");

  if (context->bit_rate<=0.0)
    simpeg_encode_error(context, "bit_rate must be positive");
  if (context->bit_rate > ((1<<30)-1)*400.0)
    simpeg_encode_error(context, "bit_rate must be less than 429 Gbit/s");
  if (context->mpeg1 && context->bit_rate > ((1<<18)-1)*400.0)
    simpeg_encode_error(context, "bit_rate must be less than 104 Mbit/s (MPEG-1)");

  if (context->vbv_buffer_size<1 || context->vbv_buffer_size>0x3ffff)
    simpeg_encode_error(context, "vbv_buffer_size must be in range 1..(2^18-1)");
  if (context->mpeg1 && context->vbv_buffer_size>=1024)
    simpeg_encode_error(context, "vbv_buffer_size must be less than 1024 (MPEG-1)");

  if (context->chroma_format<CHROMA420 || context->chroma_format>CHROMA444)
    simpeg_encode_error(context, "chroma_format must be in range 1...3");

  if (context->video_format<0 || context->video_format>4)
    simpeg_encode_error(context, "video_format must be in range 0...4");

  if (context->color_primaries<1 || context->color_primaries>7 || context->color_primaries==3)
    simpeg_encode_error(context, "color_primaries must be in range 1...2 or 4...7");

  if (context->transfer_characteristics<1 || context->transfer_characteristics>7
      || context->transfer_characteristics==3)
    simpeg_encode_error(context, "transfer_characteristics must be in range 1...2 or 4...7");

  if (context->matrix_coefficients<1 || context->matrix_coefficients>7 || context->matrix_coefficients==3)
    simpeg_encode_error(context, "matrix_coefficients must be in range 1...2 or 4...7");

  if (context->display_horizontal_size<0 || context->display_horizontal_size>16383)
    simpeg_encode_error(context, "display_horizontal_size must be in range 0...16383");
  if (context->display_vertical_size<0 || context->display_vertical_size>16383)
    simpeg_encode_error(context, "display_vertical_size must be in range 0...16383");

  if (context->dc_prec<0 || context->dc_prec>3)
    simpeg_encode_error(context, "intra_dc_precision must be in range 0...3");

  for (i=0; i<context->M; i++)
  {
    if (context->motion_data[i].forw_hor_f_code<1 || context->motion_data[i].forw_hor_f_code>9)
      simpeg_encode_error(context, "f_code must be between 1 and 9");
    if (context->motion_data[i].forw_vert_f_code<1 || context->motion_data[i].forw_vert_f_code>9)
      simpeg_encode_error(context, "f_code must be between 1 and 9");
    if (context->mpeg1 && context->motion_data[i].forw_hor_f_code>7)
      simpeg_encode_error(context, "f_code must be le less than 8");
    if (context->mpeg1 && context->motion_data[i].forw_vert_f_code>7)
      simpeg_encode_error(context, "f_code must be le less than 8");
    if (context->motion_data[i].sxf<=0)
      simpeg_encode_error(context, "search window must be positive"); /* doesn't belong here */
    if (context->motion_data[i].syf<=0)
      simpeg_encode_error(context, "search window must be positive");
    if (i!=0)
    {
      if (context->motion_data[i].back_hor_f_code<1 || context->motion_data[i].back_hor_f_code>9)
        simpeg_encode_error(context, "f_code must be between 1 and 9");
      if (context->motion_data[i].back_vert_f_code<1 || context->motion_data[i].back_vert_f_code>9)
        simpeg_encode_error(context, "f_code must be between 1 and 9");
      if (context->mpeg1 && context->motion_data[i].back_hor_f_code>7)
        simpeg_encode_error(context, "f_code must be le less than 8");
      if (context->mpeg1 && context->motion_data[i].back_vert_f_code>7)
        simpeg_encode_error(context, "f_code must be le less than 8");
      if (context->motion_data[i].sxb<=0)
        simpeg_encode_error(context, "search window must be positive");
      if (context->motion_data[i].syb<=0)
        simpeg_encode_error(context, "search window must be positive");
    }
  }
}

/* identifies valid profile / level combinations */
static char profile_level_defined[5][4] =
{
/* HL   H-14 ML   LL  */
  {1,   1,   1,   0},  /* HP   */
  {0,   1,   0,   0},  /* Spat */
  {0,   0,   1,   1},  /* SNR  */
  {1,   1,   1,   1},  /* MP   */
  {0,   0,   1,   0}   /* SP   */
};

static struct level_limits {
  int hor_f_code;
  int vert_f_code;
  int hor_size;
  int vert_size;
  int sample_rate;
  int bit_rate; /* Mbit/s */
  int vbv_buffer_size; /* 16384 bit steps */
} maxval_tab[4] =
{
  {9, 5, 1920, 1152, 62668800, 80, 597}, /* HL */
  {9, 5, 1440, 1152, 47001600, 60, 448}, /* H-14 */
  {8, 5,  720,  576, 10368000, 15, 112}, /* ML */
  {7, 4,  352,  288,  3041280,  4,  29}  /* LL */
};

#define SP   5
#define MP   4
#define SNR  3
#define SPAT 2
#define HP   1

#define LL  10
#define ML   8
#define H14  6
#define HL   4

void 
simpeg_encode_profile_and_level_checks(simpeg_encode_context * context)
{
  int i;
  struct level_limits * maxval;

  if (context->profile<0 || context->profile>15)
    simpeg_encode_error(context, "profile must be between 0 and 15");

  if (context->level<0 || context->level>15)
    simpeg_encode_error(context, "level must be between 0 and 15");

  if (context->profile>=8)
  {
    if (!context->quiet)
      SimpegWrite_warning(context,"profile uses a reserved value, conformance checks skipped");
    return;
  }

  if (context->profile<HP || context->profile>SP)
    simpeg_encode_error(context, "undefined Profile");

  if (context->profile==SNR || context->profile==SPAT)
    simpeg_encode_error(context, "This encoder currently generates no scalable bitstreams");

  if (context->level<HL || context->level>LL || context->level&1)
    simpeg_encode_error(context, "undefined Level");

  maxval = &maxval_tab[(context->level-4) >> 1];

  /* check profile@level combination */
  if(!profile_level_defined[context->profile-1][(context->level-4) >> 1])
    simpeg_encode_error(context, "unsupported profile + level combination");
  

  /* profile (syntax) constraints */

  if (context->profile==SP && context->M!=1)
    simpeg_encode_error(context, "Simple Profile does not allow B pictures");

  if (context->profile!=HP && context->chroma_format!=CHROMA420)
    simpeg_encode_error(context, "chroma format must be 4:2:0 in specified Profile");

  if (context->profile==HP && context->chroma_format==CHROMA444)
    simpeg_encode_error(context, "chroma format must be 4:2:0 or 4:2:2 in High Profile");

  if (context->profile>=MP) /* SP, MP: constrained repeat_first_field */
  {
    if (context->frame_rate_code<=2 && context->repeatfirst)
      simpeg_encode_error(context, "repeat_first_first must be zero");
    if (context->frame_rate_code<=6 && context->prog_seq && context->repeatfirst)
      simpeg_encode_error(context, "repeat_first_first must be zero");
  }

  if (context->profile!=HP && context->dc_prec==3)
    simpeg_encode_error(context, "11 bit DC precision only allowed in High Profile");


  /* level (parameter value) constraints */

  /* Table 8-8 */
  if (context->frame_rate_code>5 && context->level>=ML)
    simpeg_encode_error(context, "Picture rate greater than permitted in specified Level");

  for (i=0; i<context->M; i++)
  {
    if (context->motion_data[i].forw_hor_f_code > maxval->hor_f_code)
      simpeg_encode_error(context, "forward horizontal f_code greater than permitted in specified Level");

    if (context->motion_data[i].forw_vert_f_code > maxval->vert_f_code)
      simpeg_encode_error(context, "forward vertical f_code greater than permitted in specified Level");

    if (i!=0)
    {
      if (context->motion_data[i].back_hor_f_code > maxval->hor_f_code)
        simpeg_encode_error(context, "backward horizontal f_code greater than permitted in specified Level");
  
      if (context->motion_data[i].back_vert_f_code > maxval->vert_f_code)
        simpeg_encode_error(context, "backward vertical f_code greater than permitted in specified Level");
    }
  }

  /* Table 8-10 */
  if (context->horizontal_size > maxval->hor_size)
    simpeg_encode_error(context, "Horizontal size is greater than permitted in specified Level");

  if (context->vertical_size > maxval->vert_size)
    simpeg_encode_error(context, "Vertical size is greater than permitted in specified Level");

  /* Table 8-11 */
  if (context->horizontal_size*context->vertical_size*context->frame_rate > maxval->sample_rate)
    simpeg_encode_error(context, "Sample rate is greater than permitted in specified Level");

  /* Table 8-12 */
  if (context->bit_rate> 1.0e6 * maxval->bit_rate)
    simpeg_encode_error(context, "Bit rate is greater than permitted in specified Level");

  /* Table 8-13 */
  if (context->vbv_buffer_size > maxval->vbv_buffer_size)
    simpeg_encode_error(context, "vbv_buffer_size exceeds High Level limit");
}
