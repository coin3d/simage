/* putpic.c, block and motion vector encoding routines                      */

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
#include "global.h"

/* private prototypes */
static void putmvs(simpeg_encode_context * context,
                   int MV[2][2][2], int PMV[2][2][2],
                   int mv_field_sel[2][2], int dmvector[2], int s, int motion_type,
                   int hor_f_code, int vert_f_code);

/* quantization / variable length encoding of a complete picture */
void 
simpeg_encode_putpict(simpeg_encode_context * context,
                      unsigned char *frame)
{
  int i, j, k, comp, cc;
  int mb_type;
  int PMV[2][2][2];
  int prev_mquant;
  int cbp, MBAinc;
  short (*blocks)[64];
  struct mbinfo * mbinfo;
  int block_count;

  mbinfo = context->mbinfo;
  blocks = context->blocks;
  block_count = context->block_count;

  simpeg_encode_rc_init_pict(context, frame); /* set up rate control */

  /* picture header and picture coding extension */
  simpeg_encode_putpicthdr(context);

  if (!context->mpeg1)
    simpeg_encode_putpictcodext(context);

  prev_mquant = simpeg_encode_rc_start_mb(context); /* initialize quantization parameter */

  k = 0;

  for (j=0; j<context->mb_height2; j++)
  {
    /* macroblock row loop */

    for (i=0; i<context->mb_width; i++)
    {
      /* macroblock loop */
      if (i==0)
      {
        /* slice header (6.2.4) */
        simpeg_encode_alignbits(context);

        if (context->mpeg1 || context->vertical_size<=2800)
          simpeg_encode_putbits(context,SLICE_MIN_START+j,32); /* slice_start_code */
        else
        {
          simpeg_encode_putbits(context,SLICE_MIN_START+(j&127),32); /* slice_start_code */
          simpeg_encode_putbits(context,j>>7,3); /* slice_vertical_position_extension */
        }
  
        /* quantiser_scale_code */
        simpeg_encode_putbits(context,context->q_scale_type ? context->map_non_linear_mquant[prev_mquant]
                             : prev_mquant >> 1, 5);
  
        simpeg_encode_putbits(context,0,1); /* extra_bit_slice */
  
        /* reset predictors */

        for (cc=0; cc<3; cc++)
          context->dc_dct_pred[cc] = 0;

        PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
        PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;
  
        MBAinc = i + 1; /* first MBAinc denotes absolute position */
      }

      mb_type = mbinfo[k].mb_type;

      /* determine mquant (rate control) */
      mbinfo[k].mquant = simpeg_encode_rc_calc_mquant(context,k);

      /* quantize macroblock */
      if (mb_type & MB_INTRA)
      {
        for (comp=0; comp<block_count; comp++)
          simpeg_encode_quant_intra(context,blocks[k*block_count+comp],blocks[k*block_count+comp],
                                    context->dc_prec,context->intra_q,mbinfo[k].mquant);
        mbinfo[k].cbp = cbp = (1<<block_count) - 1;
      }
      else
      {
        cbp = 0;
        for (comp=0;comp<block_count;comp++)
          cbp = (cbp<<1) | simpeg_encode_quant_non_intra(context,blocks[k*block_count+comp],
                                                         blocks[k*block_count+comp],
                                                         context->inter_q,mbinfo[k].mquant);

        mbinfo[k].cbp = cbp;

        if (cbp)
          mb_type|= MB_PATTERN;
      }

      /* output mquant if it has changed */
      if (cbp && prev_mquant!=mbinfo[k].mquant)
        mb_type|= MB_QUANT;

      /* check if macroblock can be skipped */
      if (i!=0 && i!=context->mb_width-1 && !cbp)
      {
        /* no DCT coefficients and neither first nor last macroblock of slice */

        if (context->pict_type==P_TYPE && !(mb_type&MB_FORWARD))
        {
          /* P picture, no motion vectors -> skip */

          /* reset predictors */

          for (cc=0; cc<3; cc++)
            context->dc_dct_pred[cc] = 0;

          PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
          PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;

          mbinfo[k].mb_type = mb_type;
          mbinfo[k].skipped = 1;
          MBAinc++;
          k++;
          continue;
        }

        if (context->pict_type==B_TYPE && context->pict_struct==FRAME_PICTURE
            && mbinfo[k].motion_type==MC_FRAME
            && ((mbinfo[k-1].mb_type^mb_type)&(MB_FORWARD|MB_BACKWARD))==0
            && (!(mb_type&MB_FORWARD) ||
                (PMV[0][0][0]==mbinfo[k].MV[0][0][0] &&
                 PMV[0][0][1]==mbinfo[k].MV[0][0][1]))
            && (!(mb_type&MB_BACKWARD) ||
                (PMV[0][1][0]==mbinfo[k].MV[0][1][0] &&
                 PMV[0][1][1]==mbinfo[k].MV[0][1][1])))
        {
          /* conditions for skipping in B frame pictures:
           * - must be frame predicted
           * - must be the same prediction type (forward/backward/interp.)
           *   as previous macroblock
           * - relevant vectors (forward/backward/both) have to be the same
           *   as in previous macroblock
           */

          mbinfo[k].mb_type = mb_type;
          mbinfo[k].skipped = 1;
          MBAinc++;
          k++;
          continue;
        }

        if (context->pict_type==B_TYPE && context->pict_struct!=FRAME_PICTURE
            && mbinfo[k].motion_type==MC_FIELD
            && ((mbinfo[k-1].mb_type^mb_type)&(MB_FORWARD|MB_BACKWARD))==0
            && (!(mb_type&MB_FORWARD) ||
                (PMV[0][0][0]==mbinfo[k].MV[0][0][0] &&
                 PMV[0][0][1]==mbinfo[k].MV[0][0][1] &&
                 mbinfo[k].mv_field_sel[0][0]==(context->pict_struct==BOTTOM_FIELD)))
            && (!(mb_type&MB_BACKWARD) ||
                (PMV[0][1][0]==mbinfo[k].MV[0][1][0] &&
                 PMV[0][1][1]==mbinfo[k].MV[0][1][1] &&
                 mbinfo[k].mv_field_sel[0][1]==(context->pict_struct==BOTTOM_FIELD))))
        {
          /* conditions for skipping in B field pictures:
           * - must be field predicted
           * - must be the same prediction type (forward/backward/interp.)
           *   as previous macroblock
           * - relevant vectors (forward/backward/both) have to be the same
           *   as in previous macroblock
           * - relevant motion_vertical_field_selects have to be of same
           *   parity as current field
           */

          mbinfo[k].mb_type = mb_type;
          mbinfo[k].skipped = 1;
          MBAinc++;
          k++;
          continue;
        }
      }

      /* macroblock cannot be skipped */
      mbinfo[k].skipped = 0;

      /* there's no VLC for 'No MC, Not Coded':
       * we have to transmit (0,0) motion vectors
       */
      if (context->pict_type==P_TYPE && !cbp && !(mb_type&MB_FORWARD))
        mb_type|= MB_FORWARD;

      simpeg_encode_putaddrinc(context,MBAinc); /* macroblock_address_increment */
      MBAinc = 1;

      simpeg_encode_putmbtype(context, context->pict_type,mb_type); /* macroblock type */

      if (mb_type & (MB_FORWARD|MB_BACKWARD) && !context->frame_pred_dct)
        simpeg_encode_putbits(context,mbinfo[k].motion_type,2);

      if (context->pict_struct==FRAME_PICTURE && cbp && !context->frame_pred_dct)
        simpeg_encode_putbits(context,mbinfo[k].dct_type,1);

      if (mb_type & MB_QUANT)
      {
        simpeg_encode_putbits(context,context->q_scale_type ? 
                              context->map_non_linear_mquant[mbinfo[k].mquant]
                              : mbinfo[k].mquant>>1,5);
        prev_mquant = mbinfo[k].mquant;
      }

      if (mb_type & MB_FORWARD)
      {
        /* forward motion vectors, update predictors */
        putmvs(context,mbinfo[k].MV,PMV,mbinfo[k].mv_field_sel,
               mbinfo[k].dmvector,0,
               mbinfo[k].motion_type,context->forw_hor_f_code,context->forw_vert_f_code);
      }

      if (mb_type & MB_BACKWARD)
      {
        /* backward motion vectors, update predictors */
        putmvs(context,mbinfo[k].MV,PMV,mbinfo[k].mv_field_sel,
               mbinfo[k].dmvector,1,
               mbinfo[k].motion_type,context->back_hor_f_code,context->back_vert_f_code);
      }

      if (mb_type & MB_PATTERN)
      {
        simpeg_encode_putcbp(context,(cbp >> (block_count-6)) & 63);
        if (context->chroma_format!=CHROMA420)
          simpeg_encode_putbits(context,cbp,block_count-6);
      }

      for (comp=0; comp<block_count; comp++)
      {
        /* block loop */
        if (cbp & (1<<(block_count-1-comp)))
        {
          if (mb_type & MB_INTRA)
          {
            cc = (comp<4) ? 0 : (comp&1)+1;
            simpeg_encode_putintrablk(context,blocks[k*block_count+comp],cc);
          }
          else
            simpeg_encode_putnonintrablk(context,blocks[k*block_count+comp]);
        }
      }

      /* reset predictors */
      if (!(mb_type & MB_INTRA))
        for (cc=0; cc<3; cc++)
          context->dc_dct_pred[cc] = 0;

      if (mb_type & MB_INTRA || (context->pict_type==P_TYPE && !(mb_type & MB_FORWARD)))
      {
        PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
        PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;
      }

      mbinfo[k].mb_type = mb_type;
      k++;
    }
  }

  simpeg_encode_rc_update_pict(context);
  simpeg_encode_vbv_end_of_picture(context);
}


/* output motion vectors (6.2.5.2, 6.3.16.2)
 *
 * this routine also updates the predictions for motion vectors (PMV)
 */
 
static void putmvs(simpeg_encode_context * context,
                   int (*MV)[2][2], int (*PMV)[2][2], int (*mv_field_sel)[2], int *dmvector, int s, 
                   int motion_type, int hor_f_code, int vert_f_code)
{
  if (context->pict_struct==FRAME_PICTURE)
  {
    if (motion_type==MC_FRAME)
    {
      /* frame prediction */
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putmv(context,MV[0][s][1]-PMV[0][s][1],vert_f_code);
      PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
      PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
    }
    else if (motion_type==MC_FIELD)
    {
      /* field prediction */
      simpeg_encode_putbits(context,mv_field_sel[0][s],1);
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putmv(context,(MV[0][s][1]>>1)-(PMV[0][s][1]>>1),vert_f_code);
      simpeg_encode_putbits(context,mv_field_sel[1][s],1);
      simpeg_encode_putmv(context,MV[1][s][0]-PMV[1][s][0],hor_f_code);
      simpeg_encode_putmv(context,(MV[1][s][1]>>1)-(PMV[1][s][1]>>1),vert_f_code);
      PMV[0][s][0]=MV[0][s][0];
      PMV[0][s][1]=MV[0][s][1];
      PMV[1][s][0]=MV[1][s][0];
      PMV[1][s][1]=MV[1][s][1];
    }
    else
    {
      /* dual prime prediction */
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putdmv(context,dmvector[0]);
      simpeg_encode_putmv(context,(MV[0][s][1]>>1)-(PMV[0][s][1]>>1),vert_f_code);
      simpeg_encode_putdmv(context,dmvector[1]);
      PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
      PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
    }
  }
  else
  {
    /* field picture */
    if (motion_type==MC_FIELD)
    {
      /* field prediction */
      simpeg_encode_putbits(context,mv_field_sel[0][s],1);
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putmv(context,MV[0][s][1]-PMV[0][s][1],vert_f_code);
      PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
      PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
    }
    else if (motion_type==MC_16X8)
    {
      /* 16x8 prediction */
      simpeg_encode_putbits(context,mv_field_sel[0][s],1);
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putmv(context,MV[0][s][1]-PMV[0][s][1],vert_f_code);
      simpeg_encode_putbits(context,mv_field_sel[1][s],1);
      simpeg_encode_putmv(context,MV[1][s][0]-PMV[1][s][0],hor_f_code);
      simpeg_encode_putmv(context,MV[1][s][1]-PMV[1][s][1],vert_f_code);
      PMV[0][s][0]=MV[0][s][0];
      PMV[0][s][1]=MV[0][s][1];
      PMV[1][s][0]=MV[1][s][0];
      PMV[1][s][1]=MV[1][s][1];
    }
    else
    {
      /* dual prime prediction */
      simpeg_encode_putmv(context,MV[0][s][0]-PMV[0][s][0],hor_f_code);
      simpeg_encode_putdmv(context,dmvector[0]);
      simpeg_encode_putmv(context,MV[0][s][1]-PMV[0][s][1],vert_f_code);
      simpeg_encode_putdmv(context,dmvector[1]);
      PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
      PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
    }
  }
}
