/* putseq.c, sequence level routines                                        */

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
#include <string.h>
#include "global.h"

#include "putseq.h"

void 
simpeg_encode_putseq(simpeg_encode_context * context)
{
  /* this routine assumes (N % M) == 0 */
  int i, j, k, f, f0, n, np, nb, sxf, syf, sxb, syb;
  int ipflag;
  unsigned char *neworg[3], *newref[3];
  static char ipb[5] = {' ','I','P','B','D'};
  char name[256];
  int N, M;

  N = context->N;
  M = context->M;

  simpeg_encode_rc_init_seq(context); /* initialize rate control */

  /* sequence header, sequence extension and sequence display extension */
  simpeg_encode_putseqhdr(context);
  if (!context->mpeg1)
  {
    simpeg_encode_putseqext(context);
    simpeg_encode_putseqdispext(context);
  }

  /* optionally output some text data (description, copyright or whatever) */
  if (strlen(context->id_string) > 1)
    simpeg_encode_putuserdata(context, context->id_string);

  /* loop through all frames in encoding/decoding order */
  for (i=0; i<context->nframes; i++)
  {
    context->SimpegWrite_current_frame = i;
    if (!context->quiet) {
      /*   fprintf(stderr,"Encoding frame %d ",i);
           fflush(stderr); */
    }

    /* f0: lowest frame number in current GOP
     *
     * first GOP contains N-(M-1) frames,
     * all other GOPs contain N frames
     */
    f0 = N*((i+(M-1))/N) - (M-1);

    if (f0<0)
      f0=0;

    if (i==0 || (i-1)%M==0)
    {
      /* I or P frame */
      for (j=0; j<3; j++)
      {
        /* shuffle reference frames */
        neworg[j] = context->oldorgframe[j];
        newref[j] = context->oldrefframe[j];
        context->oldorgframe[j] = context->neworgframe[j];
        context->oldrefframe[j] = context->newrefframe[j];
        context->neworgframe[j] = neworg[j];
        context->newrefframe[j] = newref[j];
      }

      /* f: frame number in display order */
      f = (i==0) ? 0 : i+M-1;
      if (f>=context->nframes)
        f = context->nframes - 1;

      if (i==f0) /* first displayed frame in GOP is I */
      {
        /* I frame */
        context->pict_type = I_TYPE;
        context->forw_hor_f_code = context->forw_vert_f_code = 15;
        context->back_hor_f_code = context->back_vert_f_code = 15;

        /* n: number of frames in current GOP
         *
         * first GOP contains (M-1) less (B) frames
         */
        n = (i==0) ? N-(M-1) : N;

        /* last GOP may contain less frames */
        if (n > context->nframes-f0)
          n = context->nframes-f0;

        /* number of P frames */
        if (i==0)
          np = (n + 2*(M-1))/M - 1; /* first GOP */
        else
          np = (n + (M-1))/M - 1;

        /* number of B frames */
        nb = n - np - 1;

        simpeg_encode_rc_init_GOP(context,np,nb);

        simpeg_encode_putgophdr(context,f0,i==0); /* set closed_GOP in first GOP only */
      }
      else
      {
        /* P frame */
        context->pict_type = P_TYPE;
        context->forw_hor_f_code = context->motion_data[0].forw_hor_f_code;
        context->forw_vert_f_code = context->motion_data[0].forw_vert_f_code;
        context->back_hor_f_code = context->back_vert_f_code = 15;
        sxf = context->motion_data[0].sxf;
        syf = context->motion_data[0].syf;
      }
    }
    else
    {
      /* B frame */
      for (j=0; j<3; j++)
      {
        neworg[j] = context->auxorgframe[j];
        newref[j] = context->auxframe[j];
      }

      /* f: frame number in display order */
      f = i - 1;
      context->pict_type = B_TYPE;
      n = (i-2)%M + 1; /* first B: n=1, second B: n=2, ... */
      context->forw_hor_f_code = context->motion_data[n].forw_hor_f_code;
      context->forw_vert_f_code = context->motion_data[n].forw_vert_f_code;
      context->back_hor_f_code = context->motion_data[n].back_hor_f_code;
      context->back_vert_f_code = context->motion_data[n].back_vert_f_code;
      sxf = context->motion_data[n].sxf;
      syf = context->motion_data[n].syf;
      sxb = context->motion_data[n].sxb;
      syb = context->motion_data[n].syb;
    }

    context->temp_ref = f - f0;
    context->frame_pred_dct = context->frame_pred_dct_tab[context->pict_type-1];
    context->q_scale_type = context->qscale_tab[context->pict_type-1];
    context->intravlc = context->intravlc_tab[context->pict_type-1];
    context->altscan = context->altscan_tab[context->pict_type-1];

    if (context->statfile != NULL)
    {
      fprintf(context->statfile,"\nFrame %d (#%d in display order):\n",i,f);
      fprintf(context->statfile," picture_type=%c\n",ipb[context->pict_type]);
      fprintf(context->statfile," temporal_reference=%d\n",context->temp_ref);
      fprintf(context->statfile," frame_pred_frame_dct=%d\n",context->frame_pred_dct);
      fprintf(context->statfile," q_scale_type=%d\n",context->q_scale_type);
      fprintf(context->statfile," intra_vlc_format=%d\n",context->intravlc);
      fprintf(context->statfile," alternate_scan=%d\n",context->altscan);
    };

    if (context->pict_type!=I_TYPE)
    {
      if (context->statfile != NULL)
        fprintf(context->statfile," forward search window: %d...%d / %d...%d\n",
          -sxf,sxf,-syf,syf);
      if (context->statfile != NULL)
        fprintf(context->statfile," forward vector range: %d...%d.5 / %d...%d.5\n",
        -(4<<context->forw_hor_f_code),(4<<context->forw_hor_f_code)-1,
        -(4<<context->forw_vert_f_code),(4<<context->forw_vert_f_code)-1);
    }

    if (context->pict_type==B_TYPE)
    {
      if (context->statfile != NULL)
        fprintf(context->statfile," backward search window: %d...%d / %d...%d\n",
        -sxb,sxb,-syb,syb);
      if (context->statfile != NULL)
        fprintf(context->statfile," backward vector range: %d...%d.5 / %d...%d.5\n",
        -(4<<context->back_hor_f_code),(4<<context->back_hor_f_code)-1,
        -(4<<context->back_vert_f_code),(4<<context->back_vert_f_code)-1);
    }

    sprintf(name,context->tplorg,f+context->frame0);
    simpeg_encode_readframe(context,name,neworg);

    if (context->fieldpic)
    {
      if (!context->quiet)
      {
        /*
          fprintf(stderr,"\nfirst field  (%s) ",context->topfirst ? "top" : "bot");
          fflush(stderr); 
        */
      }

      context->pict_struct = context->topfirst ? TOP_FIELD : BOTTOM_FIELD;

      simpeg_encode_motion_estimation(context,context->oldorgframe[0],context->neworgframe[0],
                                      context->oldrefframe[0],context->newrefframe[0],
                                      neworg[0],newref[0],
                                      sxf,syf,sxb,syb,context->mbinfo,0,0);

      simpeg_encode_predict(context,context->oldrefframe,context->newrefframe,
                            context->predframe,0,context->mbinfo);
      simpeg_encode_dct_type_estimation(context,context->predframe[0],neworg[0],context->mbinfo);
      simpeg_encode_transform(context,context->predframe,neworg,context->mbinfo,context->blocks);

      simpeg_encode_putpict(context,neworg[0]);

      for (k=0; k<context->mb_height2*context->mb_width; k++)
      {
        if (context->mbinfo[k].mb_type & MB_INTRA)
          for (j=0; j<context->block_count; j++)
            simpeg_encode_iquant_intra(context,
                                       context->blocks[k*context->block_count+j],
                                       context->blocks[k*context->block_count+j],
                                       context->dc_prec,
                                       context->intra_q,
                                       context->mbinfo[k].mquant);
        else
          for (j=0;j<context->block_count;j++)
            simpeg_encode_iquant_non_intra(context,
                                           context->blocks[k*context->block_count+j],
                                           context->blocks[k*context->block_count+j],
                                           context->inter_q,
                                           context->mbinfo[k].mquant);
      }

      simpeg_encode_itransform(context,context->predframe,newref,context->mbinfo,context->blocks);
      simpeg_encode_calcSNR(context,neworg,newref);
      simpeg_encode_stats(context);

      if (!context->quiet)
      {
        /*
          fprintf(stderr,"second field (%s) ",context->topfirst ? "bot" : "top");
          fflush(stderr);
        */
      }

      context->pict_struct = context->topfirst ? BOTTOM_FIELD : TOP_FIELD;

      ipflag = (context->pict_type==I_TYPE);
      if (ipflag)
      {
        /* first field = I, second field = P */
        context->pict_type = P_TYPE;
        context->forw_hor_f_code = context->motion_data[0].forw_hor_f_code;
        context->forw_vert_f_code = context->motion_data[0].forw_vert_f_code;
        context->back_hor_f_code = context->back_vert_f_code = 15;
        sxf = context->motion_data[0].sxf;
        syf = context->motion_data[0].syf;
      }

      simpeg_encode_motion_estimation(context,
                                      context->oldorgframe[0],
                                      context->neworgframe[0],
                                      context->oldrefframe[0],
                                      context->newrefframe[0],
                                      neworg[0],newref[0],
                                      sxf,syf,sxb,syb,
                                      context->mbinfo,1,
                                      ipflag);

      simpeg_encode_predict(context,
                            context->oldrefframe,
                            context->newrefframe,
                            context->predframe,1,
                            context->mbinfo);
      simpeg_encode_dct_type_estimation(context,
                                        context->predframe[0],neworg[0],
                                        context->mbinfo);
      simpeg_encode_transform(context,context->predframe,neworg,
                              context->mbinfo,context->blocks);

      simpeg_encode_putpict(context,neworg[0]);

      for (k=0; k<context->mb_height2*context->mb_width; k++)
      {
        if (context->mbinfo[k].mb_type & MB_INTRA)
          for (j=0; j<context->block_count; j++)
            simpeg_encode_iquant_intra(context,
                                       context->blocks[k*context->block_count+j],
                                       context->blocks[k*context->block_count+j],
                                       context->dc_prec,
                                       context->intra_q,
                                       context->mbinfo[k].mquant);
        else
          for (j=0;j<context->block_count;j++)
            simpeg_encode_iquant_non_intra(context,
                                           context->blocks[k*context->block_count+j],
                                           context->blocks[k*context->block_count+j],
                                           context->inter_q,
                                           context->mbinfo[k].mquant);
      }

      simpeg_encode_itransform(context,
                               context->predframe,newref,
                               context->mbinfo,context->blocks);
      simpeg_encode_calcSNR(context,neworg,newref);
      simpeg_encode_stats(context);
    }
    else
    {
      context->pict_struct = FRAME_PICTURE;

      /* do motion_estimation
       *
       * uses source frames (...orgframe) for full pel search
       * and reconstructed frames (...refframe) for half pel search
       */

      simpeg_encode_motion_estimation(context,
                                      context->oldorgframe[0],
                                      context->neworgframe[0],
                                      context->oldrefframe[0],
                                      context->newrefframe[0],
                                      neworg[0],newref[0],
                                      sxf,syf,sxb,syb,
                                      context->mbinfo,0,0);

      simpeg_encode_predict(context,context->oldrefframe,
                            context->newrefframe,context->predframe,0,
                            context->mbinfo);
      simpeg_encode_dct_type_estimation(context,
                                        context->predframe[0],neworg[0],
                                        context->mbinfo);
      simpeg_encode_transform(context,context->predframe,neworg,
                              context->mbinfo,context->blocks);

      simpeg_encode_putpict(context, neworg[0]);

      for (k=0; k<context->mb_height*context->mb_width; k++)
      {
        if (context->mbinfo[k].mb_type & MB_INTRA)
          for (j=0; j<context->block_count; j++)
            simpeg_encode_iquant_intra(context,
                                       context->blocks[k*context->block_count+j],
                                       context->blocks[k*context->block_count+j],
                                       context->dc_prec,
                                       context->intra_q,
                                       context->mbinfo[k].mquant);
        else
          for (j=0;j<context->block_count;j++)
            simpeg_encode_iquant_non_intra(context,
                                           context->blocks[k*context->block_count+j],
                                           context->blocks[k*context->block_count+j],
                                           context->inter_q,
                                           context->mbinfo[k].mquant);
      }

      simpeg_encode_itransform(context,context->predframe,newref,
                               context->mbinfo,context->blocks);
      simpeg_encode_calcSNR(context,neworg,newref);
      simpeg_encode_stats(context);
    }

    sprintf(name,context->tplref,f+context->frame0);
    simpeg_encode_writeframe(context,name,newref);

  }

  simpeg_encode_putseqend(context);
}


void SimpegWrite_putseq_begin(simpeg_encode_context * context)
{
  simpeg_encode_rc_init_seq(context); /* initialize rate control */

  /* sequence header, sequence extension and sequence display extension */
  simpeg_encode_putseqhdr(context);
  if (!context->mpeg1)
  {
    simpeg_encode_putseqext(context);
    simpeg_encode_putseqdispext(context);
  }

  /* optionally output some text data (description, copyright or whatever) */
  if (strlen(context->id_string) > 1)
    simpeg_encode_putuserdata(context, context->id_string);

  context->SimpegWrite_current_frame = 0;

  context->putseq_encode_sxf=0;
  context->putseq_encode_syf=0;
  context->putseq_encode_sxb=0;
  context->putseq_encode_syb=0;
  context->putseq_encode_f=0;
  context->putseq_encode_neworg[0] = NULL;
  context->putseq_encode_neworg[1] = NULL;
  context->putseq_encode_neworg[2] = NULL;
  context->putseq_encode_newref[0] = NULL;
  context->putseq_encode_newref[1] = NULL;
  context->putseq_encode_newref[2] = NULL;
}

void SimpegWrite_putseq_encode_bitmap(simpeg_encode_context * context,
                                      const unsigned char *buffer)
{
  /* this routine assumes (N % M) == 0 */
  int j, k, f0, n, np, nb;
  int ipflag;
  int N,M;

  static char ipb[5] = {' ','I','P','B','D'};

  N = context->N;
  M = context->M;

  
  

  if (!context->quiet)
  {
    /*
      fprintf(stderr,"Encoding frame %d ", context->SimpegWrite_current_frame);
      fflush(stderr);
    */
  }

  /* f0: lowest frame number in current GOP
   *
   * first GOP contains N-(M-1) frames,
   * all other GOPs contain N frames
   */
  f0 = N*((context->SimpegWrite_current_frame+(M-1))/N) - (M-1);

  if (f0<0)
    f0=0;
  
  if (context->SimpegWrite_current_frame==0 || (context->SimpegWrite_current_frame-1)%M==0)
  {
    /* I or P frame */
    for (j=0; j<3; j++)
    {
      /* shuffle reference frames */
      context->putseq_encode_neworg[j] = context->oldorgframe[j];
      context->putseq_encode_newref[j] = context->oldrefframe[j];
      context->oldorgframe[j] = context->neworgframe[j];
      context->oldrefframe[j] = context->newrefframe[j];
      context->neworgframe[j] = context->putseq_encode_neworg[j];
      context->newrefframe[j] = context->putseq_encode_newref[j];
    }

    /* f: frame number in display order */
    context->putseq_encode_f = (context->SimpegWrite_current_frame==0) ? 0 : context->SimpegWrite_current_frame+M-1;
    if (context->putseq_encode_f>=context->nframes)
      context->putseq_encode_f = context->nframes - 1;

    if (context->SimpegWrite_current_frame==f0) /* first displayed frame in GOP is I */
    {
      /* I frame */
      context->pict_type = I_TYPE;
      context->forw_hor_f_code = context->forw_vert_f_code = 15;
      context->back_hor_f_code = context->back_vert_f_code = 15;

      /* n: number of frames in current GOP
       *
       * first GOP contains (M-1) less (B) frames
       */
      n = (context->SimpegWrite_current_frame==0) ? N-(M-1) : N;

      /* last GOP may contain less frames */
      if (n > context->nframes-f0)
        n = context->nframes-f0;

      /* number of P frames */
      if (context->SimpegWrite_current_frame==0)
        np = (n + 2*(M-1))/M - 1; /* first GOP */
      else
        np = (n + (M-1))/M - 1;

      /* number of B frames */
      nb = n - np - 1;

      simpeg_encode_rc_init_GOP(context,np,nb);

      simpeg_encode_putgophdr(context,f0,context->SimpegWrite_current_frame==0); /* set closed_GOP in first GOP only */
    }
    else
    {
      /* P frame */
      context->pict_type = P_TYPE;
      context->forw_hor_f_code = context->motion_data[0].forw_hor_f_code;
      context->forw_vert_f_code = context->motion_data[0].forw_vert_f_code;
      context->back_hor_f_code = context->back_vert_f_code = 15;
      context->putseq_encode_sxf = context->motion_data[0].sxf;
      context->putseq_encode_syf = context->motion_data[0].syf;
    }
  }
  else
  {
    /* B frame */
    for (j=0; j<3; j++)
    {
      context->putseq_encode_neworg[j] = context->auxorgframe[j];
      context->putseq_encode_newref[j] = context->auxframe[j];
    }

    /* f: frame number in display order */
    context->putseq_encode_f = context->SimpegWrite_current_frame - 1;
    context->pict_type = B_TYPE;
    n = (context->SimpegWrite_current_frame-2)%M + 1; /* first B: n=1, second B: n=2, ... */
    context->forw_hor_f_code = context->motion_data[n].forw_hor_f_code;
    context->forw_vert_f_code = context->motion_data[n].forw_vert_f_code;
    context->back_hor_f_code = context->motion_data[n].back_hor_f_code;
    context->back_vert_f_code = context->motion_data[n].back_vert_f_code;
    context->putseq_encode_sxf = context->motion_data[n].sxf;
    context->putseq_encode_syf = context->motion_data[n].syf;
    context->putseq_encode_sxb = context->motion_data[n].sxb;
    context->putseq_encode_syb = context->motion_data[n].syb;
  }

  context->temp_ref = context->putseq_encode_f - f0;
  context->frame_pred_dct = context->frame_pred_dct_tab[context->pict_type-1];
  context->q_scale_type = context->qscale_tab[context->pict_type-1];
  context->intravlc = context->intravlc_tab[context->pict_type-1];
  context->altscan = context->altscan_tab[context->pict_type-1];

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\nFrame %d (#%d in display order):\n",context->SimpegWrite_current_frame,context->putseq_encode_f);
    fprintf(context->statfile," picture_type=%c\n",ipb[context->pict_type]);
    fprintf(context->statfile," temporal_reference=%d\n",context->temp_ref);
    fprintf(context->statfile," frame_pred_frame_dct=%d\n",context->frame_pred_dct);
    fprintf(context->statfile," q_scale_type=%d\n",context->q_scale_type);
    fprintf(context->statfile," intra_vlc_format=%d\n",context->intravlc);
    fprintf(context->statfile," alternate_scan=%d\n",context->altscan);
  };

  if (context->pict_type!=I_TYPE)
  {
    if (context->statfile != NULL)
      fprintf(context->statfile," forward search window: %d...%d / %d...%d\n",
        -context->putseq_encode_sxf,context->putseq_encode_sxf,-context->putseq_encode_syf,context->putseq_encode_syf);
    if (context->statfile != NULL)
      fprintf(context->statfile," forward vector range: %d...%d.5 / %d...%d.5\n",
      -(4<<context->forw_hor_f_code),(4<<context->forw_hor_f_code)-1,
      -(4<<context->forw_vert_f_code),(4<<context->forw_vert_f_code)-1);
  }

  if (context->pict_type==B_TYPE)
  {
    if (context->statfile != NULL)
      fprintf(context->statfile," backward search window: %d...%d / %d...%d\n",
      -context->putseq_encode_sxb,context->putseq_encode_sxb,-context->putseq_encode_syb,context->putseq_encode_syb);
    if (context->statfile != NULL)
      fprintf(context->statfile," backward vector range: %d...%d.5 / %d...%d.5\n",
      -(4<<context->back_hor_f_code),(4<<context->back_hor_f_code)-1,
      -(4<<context->back_vert_f_code),(4<<context->back_vert_f_code)-1);
  }

  simpeg_encode_readframe(context, (char*) buffer, context->putseq_encode_neworg);
  
  if (context->fieldpic)
  {
    if (!context->quiet)
    {
      /*
        fprintf(stderr,"\nfirst field  (%s) ",context->topfirst ? "top" : "bot");
        fflush(stderr);
      */
    }

    context->pict_struct = context->topfirst ? TOP_FIELD : BOTTOM_FIELD;

    simpeg_encode_motion_estimation(context,
                                    context->oldorgframe[0],
                                    context->neworgframe[0],
                                    context->oldrefframe[0],
                                    context->newrefframe[0],
                                    context->putseq_encode_neworg[0],
                                    context->putseq_encode_newref[0],
                                    context->putseq_encode_sxf,
                                    context->putseq_encode_syf,
                                    context->putseq_encode_sxb,
                                    context->putseq_encode_syb,
                                    context->mbinfo,0,0);
    
    simpeg_encode_predict(context,context->oldrefframe,context->newrefframe,
                          context->predframe,0,context->mbinfo);
    simpeg_encode_dct_type_estimation(context,
                                      context->predframe[0],
                                      context->putseq_encode_neworg[0],
                                      context->mbinfo);
    simpeg_encode_transform(context,context->predframe,
                            context->putseq_encode_neworg,
                            context->mbinfo,context->blocks);

    simpeg_encode_putpict(context,
                          context->putseq_encode_neworg[0]);

    for (k=0; k<context->mb_height2*context->mb_width; k++)
    {
      if (context->mbinfo[k].mb_type & MB_INTRA)
        for (j=0; j<context->block_count; j++)
          simpeg_encode_iquant_intra(context,
                                     context->blocks[k*context->block_count+j],
                                     context->blocks[k*context->block_count+j],
                                     context->dc_prec,
                                     context->intra_q,
                                     context->mbinfo[k].mquant);
      else
        for (j=0;j<context->block_count;j++)
          simpeg_encode_iquant_non_intra(context,
                                         context->blocks[k*context->block_count+j],
                                         context->blocks[k*context->block_count+j],
                                         context->inter_q,context->mbinfo[k].mquant);
    }

    simpeg_encode_itransform(context,
                             context->predframe,
                             context->putseq_encode_newref,
                             context->mbinfo,context->blocks);
    simpeg_encode_calcSNR(context,
                          context->putseq_encode_neworg,
                          context->putseq_encode_newref);
    simpeg_encode_stats(context);

    if (!context->quiet)
    {
      /*
        fprintf(stderr,"second field (%s) ",context->topfirst ? "bot" : "top");
        fflush(stderr);
      */
    }

    context->pict_struct = context->topfirst ? BOTTOM_FIELD : TOP_FIELD;

    ipflag = (context->pict_type==I_TYPE);
    if (ipflag)
    {
      /* first field = I, second field = P */
      context->pict_type = P_TYPE;
      context->forw_hor_f_code = context->motion_data[0].forw_hor_f_code;
      context->forw_vert_f_code = context->motion_data[0].forw_vert_f_code;
      context->back_hor_f_code = context->back_vert_f_code = 15;
      context->putseq_encode_sxf = context->motion_data[0].sxf;
      context->putseq_encode_syf = context->motion_data[0].syf;
    }

    simpeg_encode_motion_estimation(context,
                                    context->oldorgframe[0],context->neworgframe[0],
                                    context->oldrefframe[0],context->newrefframe[0],
                                    context->putseq_encode_neworg[0],
                                    context->putseq_encode_newref[0],
                                    context->putseq_encode_sxf,
                                    context->putseq_encode_syf,
                                    context->putseq_encode_sxb,
                                    context->putseq_encode_syb,
                                    context->mbinfo,1,
                                    ipflag);

    simpeg_encode_predict(context,
                          context->oldrefframe,
                          context->newrefframe,
                          context->predframe,1,
                          context->mbinfo);
    simpeg_encode_dct_type_estimation(context,
                                      context->predframe[0],
                                      context->putseq_encode_neworg[0],
                                      context->mbinfo);
    simpeg_encode_transform(context,
                            context->predframe,
                            context->putseq_encode_neworg,
                            context->mbinfo,context->blocks);

    simpeg_encode_putpict(context, context->putseq_encode_neworg[0]);

    for (k=0; k<context->mb_height2*context->mb_width; k++)
    {
      if (context->mbinfo[k].mb_type & MB_INTRA)
        for (j=0; j<context->block_count; j++)
          simpeg_encode_iquant_intra(context,
                                     context->blocks[k*context->block_count+j],
                                     context->blocks[k*context->block_count+j],
                                     context->dc_prec,
                                     context->intra_q,
                                     context->mbinfo[k].mquant);
      else
        for (j=0;j<context->block_count;j++)
          simpeg_encode_iquant_non_intra(context,
                                         context->blocks[k*context->block_count+j],
                                         context->blocks[k*context->block_count+j],
                                         context->inter_q,
                                         context->mbinfo[k].mquant);
    }

    simpeg_encode_itransform(context,
                             context->predframe,
                             context->putseq_encode_newref,
                             context->mbinfo,context->blocks);
    simpeg_encode_calcSNR(context,context->putseq_encode_neworg,
                          context->putseq_encode_newref);
    simpeg_encode_stats(context);
  }
  else
  {
    context->pict_struct = FRAME_PICTURE;

    /* do motion_estimation
     *
     * uses source frames (...orgframe) for full pel search
     * and reconstructed frames (...refframe) for half pel search
     */

    simpeg_encode_motion_estimation(context,
                                    context->oldorgframe[0],context->neworgframe[0],
                                    context->oldrefframe[0],context->newrefframe[0],
                                    context->putseq_encode_neworg[0],
                                    context->putseq_encode_newref[0],
                                    context->putseq_encode_sxf,
                                    context->putseq_encode_syf,
                                    context->putseq_encode_sxb,
                                    context->putseq_encode_syb,
                                    context->mbinfo,0,0);

    simpeg_encode_predict(context,
                          context->oldrefframe,
                          context->newrefframe,
                          context->predframe,0,
                          context->mbinfo);
    simpeg_encode_dct_type_estimation(context,
                                      context->predframe[0],
                                      context->putseq_encode_neworg[0],context->mbinfo);
    simpeg_encode_transform(context,context->predframe,
                            context->putseq_encode_neworg,context->mbinfo,context->blocks);

    simpeg_encode_putpict(context,context->putseq_encode_neworg[0]);

    for (k=0; k<context->mb_height*context->mb_width; k++)
    {
      if (context->mbinfo[k].mb_type & MB_INTRA)
        for (j=0; j<context->block_count; j++)
          simpeg_encode_iquant_intra(context,
                                     context->blocks[k*context->block_count+j],
                                     context->blocks[k*context->block_count+j],
                                     context->dc_prec,context->intra_q,context->mbinfo[k].mquant);
      else
        for (j=0;j<context->block_count;j++)
          simpeg_encode_iquant_non_intra(context,context->blocks[k*context->block_count+j],
                                         context->blocks[k*context->block_count+j],
                                         context->inter_q,context->mbinfo[k].mquant);
    }

    simpeg_encode_itransform(context,context->predframe,
                             context->putseq_encode_newref,context->mbinfo,context->blocks);
    simpeg_encode_calcSNR(context,context->putseq_encode_neworg,context->putseq_encode_newref);
    simpeg_encode_stats(context);
  }

  /* fixme: is this necessary ?
     sprintf(name,tplref,f+frame0);
     writeframe(name,putseq_encode_newref);
  */
  context->SimpegWrite_current_frame++;

};

void SimpegWrite_putseq_end(simpeg_encode_context * context)
{
  simpeg_encode_putseqend(context);
}
