/* stats.c, coding statistics                                               */

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
static void calcSNR1(unsigned char *org, unsigned char *rec,
                     int lx, int w, int h, double *pv, double *pe);


void 
simpeg_encode_calcSNR(simpeg_encode_context * context,
                      unsigned char **org, unsigned char **rec)
{
  int w,h,offs;
  double v,e;

  w = context->horizontal_size;
  h = (context->pict_struct==FRAME_PICTURE) ? context->vertical_size : (context->vertical_size>>1);
  offs = (context->pict_struct==BOTTOM_FIELD) ? context->width : 0;

  calcSNR1(org[0]+offs,rec[0]+offs,context->width2,w,h,&v,&e);
  if (context->statfile != NULL)
    fprintf(context->statfile,"Y: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
            v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));

  if (context->chroma_format!=CHROMA444)
  {
    w >>= 1;
    offs >>= 1;
  }

  if (context->chroma_format==CHROMA420)
    h >>= 1;

  calcSNR1(org[1]+offs,rec[1]+offs,context->chrom_width2,w,h,&v,&e);
  if (context->statfile != NULL)
    fprintf(context->statfile,"U: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
            v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));

  calcSNR1(org[2]+offs,rec[2]+offs,context->chrom_width2,w,h,&v,&e);
  if (context->statfile != NULL)
    fprintf(context->statfile,"V: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
            v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));
}

static void calcSNR1(unsigned char *org, unsigned char *rec, int lx, int w, int h, double *pv, double *pe)
{
  int i, j;
  double v1, s1, s2, e2;

  s1 = s2 = e2 = 0.0;

  for (j=0; j<h; j++)
  {
    for (i=0; i<w; i++)
    {
      v1 = org[i];
      s1+= v1;
      s2+= v1*v1;
      v1-= rec[i];
      e2+= v1*v1;
    }
    org += lx;
    rec += lx;
  }

  s1 /= w*h;
  s2 /= w*h;
  e2 /= w*h;

  /* prevent division by zero in calcSNR() */
  if(e2==0.0)
    e2 = 0.00001;

  *pv = s2 - s1*s1; /* variance */
  *pe = e2;         /* MSE */
}

void 
simpeg_encode_stats(simpeg_encode_context * context)
{
  int i, j, k, nmb, mb_type;
  int n_skipped, n_intra, n_ncoded, n_blocks, n_interp, n_forward, n_backward;
  struct mbinfo *mbi;

  nmb = context->mb_width*context->mb_height2;

  n_skipped=n_intra=n_ncoded=n_blocks=n_interp=n_forward=n_backward=0;

  for (k=0; k<nmb; k++)
  {
    mbi = context->mbinfo+k;
    if (mbi->skipped)
      n_skipped++;
    else if (mbi->mb_type & MB_INTRA)
      n_intra++;
    else if (!(mbi->mb_type & MB_PATTERN))
      n_ncoded++;

    for (i=0; i<context->block_count; i++)
      if (mbi->cbp & (1<<i))
        n_blocks++;

    if (mbi->mb_type & MB_FORWARD)
    {
      if (mbi->mb_type & MB_BACKWARD)
        n_interp++;
      else
        n_forward++;
    }
    else if (mbi->mb_type & MB_BACKWARD)
      n_backward++;
  }

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\npicture statistics:\n");
    fprintf(context->statfile," # of intra coded macroblocks:  %4d (%.1f%%)\n",
            n_intra,100.0*(double)n_intra/nmb);
    fprintf(context->statfile," # of coded blocks:             %4d (%.1f%%)\n",
            n_blocks,100.0*(double)n_blocks/(context->block_count*nmb));
    fprintf(context->statfile," # of not coded macroblocks:    %4d (%.1f%%)\n",
            n_ncoded,100.0*(double)n_ncoded/nmb);
    fprintf(context->statfile," # of skipped macroblocks:      %4d (%.1f%%)\n",
            n_skipped,100.0*(double)n_skipped/nmb);
    fprintf(context->statfile," # of forw. pred. macroblocks:  %4d (%.1f%%)\n",
            n_forward,100.0*(double)n_forward/nmb);
    fprintf(context->statfile," # of backw. pred. macroblocks: %4d (%.1f%%)\n",
            n_backward,100.0*(double)n_backward/nmb);
    fprintf(context->statfile," # of interpolated macroblocks: %4d (%.1f%%)\n",
            n_interp,100.0*(double)n_interp/nmb);
    
    fprintf(context->statfile,"\nmacroblock_type map:\n");
  };

  k = 0;

  for (j=0; j<context->mb_height2; j++)
  {
    for (i=0; i<context->mb_width; i++)
    {
      mbi = context->mbinfo + k;
      mb_type = mbi->mb_type;
      if (mbi->skipped)
      {
        if (context->statfile != NULL)
          putc('S',context->statfile);
      }
      else if (mb_type & MB_INTRA)
      {
        if (context->statfile != NULL)
          putc('I',context->statfile);
      }
      else switch (mb_type & (MB_FORWARD|MB_BACKWARD))
      {
      case MB_FORWARD:
        if (context->statfile != NULL)
          putc(mbi->motion_type==MC_FIELD ? 'f' :
               mbi->motion_type==MC_DMV   ? 'p' :
                                          'F',context->statfile); break;
      case MB_BACKWARD:
        if (context->statfile != NULL)
          putc(mbi->motion_type==MC_FIELD ? 'b' :
                                          'B',context->statfile); break;
      case MB_FORWARD|MB_BACKWARD:
        if (context->statfile != NULL)
          putc(mbi->motion_type==MC_FIELD ? 'd' :
               'D',context->statfile); break;
      default:
        if (context->statfile != NULL)
          putc('0',context->statfile); break;
      }
      
      if (mb_type & MB_QUANT)
      {
        if (context->statfile != NULL)
          putc('Q',context->statfile);
      }
      else if (mb_type & (MB_PATTERN|MB_INTRA))
        {
          if (context->statfile != NULL)
            putc(' ',context->statfile);
        }
      else
        {
          if (context->statfile != NULL)
            putc('N',context->statfile);
        }
      
      if (context->statfile != NULL)
        putc(' ',context->statfile);
      
      k++;
    }
    if (context->statfile != NULL)
      putc('\n',context->statfile);
  }
  
  if (context->statfile != NULL)
    fprintf(context->statfile,"\nmquant map:\n");
  
  k=0;
  for (j=0; j<context->mb_height2; j++)
  {
    for (i=0; i<context->mb_width; i++)
    {
      if (i==0 || context->mbinfo[k].mquant!=context->mbinfo[k-1].mquant)
      {
        if (context->statfile != NULL)
          fprintf(context->statfile,"%3d",context->mbinfo[k].mquant);
      }
      else
      {
        if (context->statfile != NULL)
          fprintf(context->statfile,"   ");
      }

      k++;
    }
    if (context->statfile != NULL)
      putc('\n',context->statfile);
  }

#if 0
  if (context->statfile != NULL)
    fprintf(context->statfile,"\ncbp map:\n");

  k=0;
  for (j=0; j<mb_height2; j++)
  {
    for (i=0; i<mb_width; i++)
    {
      if (context->statfile != NULL)
        fprintf(context->statfile,"%02x ",mbinfo[k].cbp);

      k++;
    }
    if (context->statfile != NULL)
      putc('\n',context->statfile);
  }

  if (pict_struct==FRAME_PICTURE && !frame_pred_dct)
  {
    if (context->statfile != NULL)
      fprintf(context->statfile,"\ndct_type map:\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & (MB_PATTERN|MB_INTRA))
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%d  ",mbinfo[k].dct_type);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   ");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }
  }

  if (pict_type!=I_TYPE)
  {
    if (context->statfile != NULL)
      fprintf(context->statfile,"\nforward motion vectors (first vector, horizontal):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_FORWARD)
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[0][0][0]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nforward motion vectors (first vector, vertical):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_FORWARD)
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[0][0][1]);
        }
        else
        {
          if (context->statfile != NULL)
           fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nforward motion vectors (second vector, horizontal):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_FORWARD
            && ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
                (pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[1][0][0]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nforward motion vectors (second vector, vertical):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_FORWARD
            && ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
                (pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[1][0][1]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }


  }
    
  if (pict_type==B_TYPE)
  {
    if (context->statfile != NULL)
      fprintf(context->statfile,"\nbackward motion vectors (first vector, horizontal):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_BACKWARD)
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[0][1][0]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nbackward motion vectors (first vector, vertical):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_BACKWARD)
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[0][1][1]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nbackward motion vectors (second vector, horizontal):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_BACKWARD
            && ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
                (pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[1][1][0]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }

    if (context->statfile != NULL)
      fprintf(context->statfile,"\nbackward motion vectors (second vector, vertical):\n");

    k=0;
    for (j=0; j<mb_height2; j++)
    {
      for (i=0; i<mb_width; i++)
      {
        if (mbinfo[k].mb_type & MB_BACKWARD
            && ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
                (pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"%4d",mbinfo[k].MV[1][1][1]);
        }
        else
        {
          if (context->statfile != NULL)
            fprintf(context->statfile,"   .");
        }
  
        k++;
      }
      if (context->statfile != NULL)
        putc('\n',context->statfile);
    }


  }
#endif
    
#if 0
  /* useful for debugging */
  if (context->statfile != NULL)
    fprintf(context->statfile,"\nmacroblock info dump:\n");

  k=0;
  for (j=0; j<mb_height2; j++)
  {
    for (i=0; i<mb_width; i++)
    {
      if (context->statfile != NULL)
        fprintf(context->statfile,"%d: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        k,
        mbinfo[k].mb_type,
        mbinfo[k].motion_type,
        mbinfo[k].dct_type,
        mbinfo[k].mquant,
        mbinfo[k].cbp,
        mbinfo[k].skipped,
        mbinfo[k].MV[0][0][0],
        mbinfo[k].MV[0][0][1],
        mbinfo[k].MV[0][1][0],
        mbinfo[k].MV[0][1][1],
        mbinfo[k].MV[1][0][0],
        mbinfo[k].MV[1][0][1],
        mbinfo[k].MV[1][1][0],
        mbinfo[k].MV[1][1][1],
        mbinfo[k].mv_field_sel[0][0],
        mbinfo[k].mv_field_sel[0][1],
        mbinfo[k].mv_field_sel[1][0],
        mbinfo[k].mv_field_sel[1][1]);

      k++;
    }
  }
#endif
}
