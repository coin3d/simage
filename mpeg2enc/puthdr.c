/* puthdr.c, generation of headers                                          */

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
#include <math.h>
#include "global.h"

/* private prototypes */
static int frametotc(simpeg_encode_context * context, int frame);

/* generate sequence header (6.2.2.1, 6.3.3)
 *
 * matrix download not implemented
 */
void 
simpeg_encode_putseqhdr(simpeg_encode_context * context)
{
  int i;

  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, SEQ_START_CODE,32); /* sequence_header_code */
  simpeg_encode_putbits(context, context->horizontal_size,12); /* horizontal_size_value */
  simpeg_encode_putbits(context, context->vertical_size,12); /* vertical_size_value */
  simpeg_encode_putbits(context, context->aspectratio,4); /* aspect_ratio_information */
  simpeg_encode_putbits(context, context->frame_rate_code,4); /* frame_rate_code */
  simpeg_encode_putbits(context, (int)ceil(context->bit_rate/400.0),18); /* bit_rate_value */
  simpeg_encode_putbits(context, 1,1); /* marker_bit */
  simpeg_encode_putbits(context, context->vbv_buffer_size,10); /* vbv_buffer_size_value */
  simpeg_encode_putbits(context, context->constrparms,1); /* constrained_parameters_flag */

  simpeg_encode_putbits(context, context->load_iquant,1); /* load_intra_quantizer_matrix */
  if (context->load_iquant)
    for (i=0; i<64; i++)  /* matrices are always downloaded in zig-zag order */
      simpeg_encode_putbits(context, context->intra_q[context->zig_zag_scan[i]],8); /* intra_quantizer_matrix */
  
  simpeg_encode_putbits(context, context->load_niquant,1); /* load_non_intra_quantizer_matrix */
  if (context->load_niquant)
    for (i=0; i<64; i++)
      simpeg_encode_putbits(context, context->inter_q[context->zig_zag_scan[i]],8); /* non_intra_quantizer_matrix */
}

/* generate sequence extension (6.2.2.3, 6.3.5) header (MPEG-2 only) */
void 
simpeg_encode_putseqext(simpeg_encode_context * context)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, EXT_START_CODE,32); /* extension_start_code */
  simpeg_encode_putbits(context, SEQ_ID,4); /* extension_start_code_identifier */
  simpeg_encode_putbits(context, (context->profile<<4)|context->level,8); /* profile_and_level_indication */
  simpeg_encode_putbits(context, context->prog_seq,1); /* progressive sequence */
  simpeg_encode_putbits(context, context->chroma_format,2); /* chroma_format */
  simpeg_encode_putbits(context, context->horizontal_size>>12,2); /* horizontal_size_extension */
  simpeg_encode_putbits(context, context->vertical_size>>12,2); /* vertical_size_extension */
  simpeg_encode_putbits(context, ((int)ceil(context->bit_rate/400.0))>>18,12); /* bit_rate_extension */
  simpeg_encode_putbits(context, 1,1); /* marker_bit */
  simpeg_encode_putbits(context, context->vbv_buffer_size>>10,8); /* vbv_buffer_size_extension */
  simpeg_encode_putbits(context, 0,1); /* low_delay  -- currently not implemented */
  simpeg_encode_putbits(context, 0,2); /* frame_rate_extension_n */
  simpeg_encode_putbits(context, 0,5); /* frame_rate_extension_d */
}

/* generate sequence display extension (6.2.2.4, 6.3.6)
 *
 * content not yet user setable
 */
void 
simpeg_encode_putseqdispext(simpeg_encode_context * context)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, EXT_START_CODE,32); /* extension_start_code */
  simpeg_encode_putbits(context, DISP_ID,4); /* extension_start_code_identifier */
  simpeg_encode_putbits(context, context->video_format,3); /* video_format */
  simpeg_encode_putbits(context, 1,1); /* colour_description */
  simpeg_encode_putbits(context, context->color_primaries,8); /* colour_primaries */
  simpeg_encode_putbits(context, context->transfer_characteristics,8); /* transfer_characteristics */
  simpeg_encode_putbits(context, context->matrix_coefficients,8); /* matrix_coefficients */
  simpeg_encode_putbits(context, context->display_horizontal_size,14); /* display_horizontal_size */
  simpeg_encode_putbits(context, 1,1); /* marker_bit */
  simpeg_encode_putbits(context, context->display_vertical_size,14); /* display_vertical_size */
}

/* output a zero terminated string as user data (6.2.2.2.2, 6.3.4.1)
 *
 * string must not emulate start codes
 */
void 
simpeg_encode_putuserdata(simpeg_encode_context * context, 
                          char *userdata)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, USER_START_CODE,32); /* user_data_start_code */
  while (*userdata)
    simpeg_encode_putbits(context, *userdata++,8);
}

/* generate group of pictures header (6.2.2.6, 6.3.9)
 *
 * uses tc0 (timecode of first frame) and frame0 (number of first frame)
 */
void 
simpeg_encode_putgophdr(simpeg_encode_context * context, 
                        int frame, int closed_gop)
{
  int tc;

  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, GOP_START_CODE,32); /* group_start_code */
  tc = frametotc(context, context->tc0+frame);
  simpeg_encode_putbits(context, tc,25); /* time_code */
  simpeg_encode_putbits(context, closed_gop,1); /* closed_gop */
  simpeg_encode_putbits(context, 0,1); /* broken_link */
}

/* convert frame number to time_code
 *
 * drop_frame not implemented
 */
static int frametotc(simpeg_encode_context * context, int frame)
{
  int fps, pict, sec, minute, hour, tc;

  fps = (int)(context->frame_rate+0.5);
  pict = frame%fps;
  frame = (frame-pict)/fps;
  sec = frame%60;
  frame = (frame-sec)/60;
  minute = frame%60;
  frame = (frame-minute)/60;
  hour = frame%24;
  tc = (hour<<19) | (minute<<13) | (1<<12) | (sec<<6) | pict;

  return tc;
}

/* generate picture header (6.2.3, 6.3.10) */
void 
simpeg_encode_putpicthdr(simpeg_encode_context * context)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, PICTURE_START_CODE,32); /* picture_start_code */
  simpeg_encode_calc_vbv_delay(context);
  simpeg_encode_putbits(context, context->temp_ref,10); /* temporal_reference */
  simpeg_encode_putbits(context, context->pict_type,3); /* picture_coding_type */
  simpeg_encode_putbits(context, context->vbv_delay,16); /* vbv_delay */

  if (context->pict_type==P_TYPE || context->pict_type==B_TYPE)
  {
    simpeg_encode_putbits(context, 0,1); /* full_pel_forward_vector */
    if (context->mpeg1)
      simpeg_encode_putbits(context, context->forw_hor_f_code,3);
    else
      simpeg_encode_putbits(context, 7,3); /* forward_f_code */
  }

  if (context->pict_type==B_TYPE)
  {
    simpeg_encode_putbits(context, 0,1); /* full_pel_backward_vector */
    if (context->mpeg1)
      simpeg_encode_putbits(context, context->back_hor_f_code,3);
    else
      simpeg_encode_putbits(context, 7,3); /* backward_f_code */
  }

  simpeg_encode_putbits(context, 0,1); /* extra_bit_picture */
}

/* generate picture coding extension (6.2.3.1, 6.3.11)
 *
 * composite display information (v_axis etc.) not implemented
 */
void 
simpeg_encode_putpictcodext(simpeg_encode_context * context)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, EXT_START_CODE,32); /* extension_start_code */
  simpeg_encode_putbits(context, CODING_ID,4); /* extension_start_code_identifier */
  simpeg_encode_putbits(context, context->forw_hor_f_code,4); /* forward_horizontal_f_code */
  simpeg_encode_putbits(context, context->forw_vert_f_code,4); /* forward_vertical_f_code */
  simpeg_encode_putbits(context, context->back_hor_f_code,4); /* backward_horizontal_f_code */
  simpeg_encode_putbits(context, context->back_vert_f_code,4); /* backward_vertical_f_code */
  simpeg_encode_putbits(context, context->dc_prec,2); /* intra_dc_precision */
  simpeg_encode_putbits(context, context->pict_struct,2); /* picture_structure */
  simpeg_encode_putbits(context, (context->pict_struct==FRAME_PICTURE)?context->topfirst:0,1); /* top_field_first */
  simpeg_encode_putbits(context, context->frame_pred_dct,1); /* frame_pred_frame_dct */
  simpeg_encode_putbits(context, 0,1); /* concealment_motion_vectors  -- currently not implemented */
  simpeg_encode_putbits(context, context->q_scale_type,1); /* q_scale_type */
  simpeg_encode_putbits(context, context->intravlc,1); /* intra_vlc_format */
  simpeg_encode_putbits(context, context->altscan,1); /* alternate_scan */
  simpeg_encode_putbits(context, context->repeatfirst,1); /* repeat_first_field */
  simpeg_encode_putbits(context, context->prog_frame,1); /* chroma_420_type */
  simpeg_encode_putbits(context, context->prog_frame,1); /* progressive_frame */
  simpeg_encode_putbits(context, 0,1); /* composite_display_flag */
}

/* generate sequence_end_code (6.2.2) */
void
simpeg_encode_putseqend(simpeg_encode_context * context)
{
  simpeg_encode_alignbits(context);
  simpeg_encode_putbits(context, SEQ_END_CODE,32);
}
