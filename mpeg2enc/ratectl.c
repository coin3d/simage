/* ratectl.c, bitrate control routines (linear quantization only currently) */

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
static void calc_actj(simpeg_encode_context * context, unsigned char *frame);
static double var_sblk(unsigned char *p, int lx);

void 
simpeg_encode_rc_init_seq(simpeg_encode_context * context)
{
  /* reaction parameter (constant) */
  if (context->r==0)  context->r = (int)floor(2.0*context->bit_rate/context->frame_rate + 0.5);

  /* average activity */
  if (context->avg_act==0.0) context->avg_act = 400.0;

  /* remaining # of bits in GOP */
  context->R = 0;

  /* global complexity measure */
  if (context->Xi==0) context->Xi = (int)floor(160.0*context->bit_rate/115.0 + 0.5);
  if (context->Xp==0) context->Xp = (int)floor( 60.0*context->bit_rate/115.0 + 0.5);
  if (context->Xb==0) context->Xb = (int)floor( 42.0*context->bit_rate/115.0 + 0.5);

  /* virtual buffer fullness */
  if (context->d0i==0) context->d0i = (int)floor(10.0*context->r/31.0 + 0.5);
  if (context->d0p==0) context->d0p = (int)floor(10.0*context->r/31.0 + 0.5);
  if (context->d0b==0) context->d0b = (int)floor(1.4*10.0*context->r/31.0 + 0.5);
/*
  if (d0i==0) d0i = (int)floor(10.0*r/(qscale_tab[0] ? 56.0 : 31.0) + 0.5);
  if (d0p==0) d0p = (int)floor(10.0*r/(qscale_tab[1] ? 56.0 : 31.0) + 0.5);
  if (d0b==0) d0b = (int)floor(1.4*10.0*r/(qscale_tab[2] ? 56.0 : 31.0) + 0.5);
*/

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\nrate control: sequence initialization\n");
    fprintf(context->statfile,
      " initial global complexity measures (I,P,B): Xi=%d, Xp=%d, Xb=%d\n",
      context->Xi, context->Xp, context->Xb);
    fprintf(context->statfile," reaction parameter: r=%d\n", context->r);
    fprintf(context->statfile,
      " initial virtual buffer fullness (I,P,B): d0i=%d, d0p=%d, d0b=%d\n",
      context->d0i, context->d0p, context->d0b);
    fprintf(context->statfile," initial average activity: avg_act=%.1f\n", context->avg_act);
  };
}

void 
simpeg_encode_rc_init_GOP(simpeg_encode_context * context, int np, int nb)
{
  context->R += (int) floor((1 + np + nb) * context->bit_rate / context->frame_rate + 0.5);
  context->Np = context->fieldpic ? 2*np+1 : np;
  context->Nb = context->fieldpic ? 2*nb : nb;

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\nrate control: new group of pictures (GOP)\n");
    fprintf(context->statfile," target number of bits for GOP: R=%d\n",context->R);
    fprintf(context->statfile," number of P pictures in GOP: Np=%d\n",context->Np);
    fprintf(context->statfile," number of B pictures in GOP: Nb=%d\n",context->Nb);
  };
}

/* Note: we need to substitute K for the 1.4 and 1.0 constants -- this can
   be modified to fit image content */

/* Step 1: compute target bits for current picture being coded */
void 
simpeg_encode_rc_init_pict(simpeg_encode_context * context,
                           unsigned char *frame)
{
  double Tmin;

  switch (context->pict_type)
  {
  case I_TYPE:
    context->T = (int) floor(context->R/(1.0+context->Np*context->Xp/(context->Xi*1.0)+context->Nb*context->Xb/(context->Xi*1.4)) + 0.5);
    context->d = context->d0i;
    break;
  case P_TYPE:
    context->T = (int) floor(context->R/(context->Np+context->Nb*1.0*context->Xb/(1.4*context->Xp)) + 0.5);
    context->d = context->d0p;
    break;
  case B_TYPE:
    context->T = (int) floor(context->R/(context->Nb+context->Np*1.4*context->Xp/(1.0*context->Xb)) + 0.5);
    context->d = context->d0b;
    break;
  }

  Tmin = (int) floor(context->bit_rate/(8.0*context->frame_rate) + 0.5);

  if (context->T<Tmin)
    context->T = Tmin;

  context->S = simpeg_encode_bitcount(context);
  context->Q = 0;

  calc_actj(context,frame);
  context->actsum = 0.0;

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\nrate control: start of picture\n");
    fprintf(context->statfile," target number of bits: T=%d\n",context->T);
  };
}

static void calc_actj(simpeg_encode_context * context,
                      unsigned char *frame)
{
  int i,j,k;
  unsigned char *p;
  double actj,var;
  int width;
  int width2, height2;

  width = context->width;
  width2 = context->width2;
  height2 = context->height2;

  k = 0;

  for (j=0; j<height2; j+=16)
    for (i=0; i<width; i+=16)
    {
      p = frame + ((context->pict_struct==BOTTOM_FIELD)?width:0) + i + width2*j;

      /* take minimum spatial activity measure of luminance blocks */

      actj = var_sblk(p,width2);
      var = var_sblk(p+8,width2);
      if (var<actj) actj = var;
      var = var_sblk(p+8*width2,width2);
      if (var<actj) actj = var;
      var = var_sblk(p+8*width2+8,width2);
      if (var<actj) actj = var;

      if (!context->fieldpic && !context->prog_seq)
      {
        /* field */
        var = var_sblk(p,width<<1);
        if (var<actj) actj = var;
        var = var_sblk(p+8,width<<1);
        if (var<actj) actj = var;
        var = var_sblk(p+width,width<<1);
        if (var<actj) actj = var;
        var = var_sblk(p+width+8,width<<1);
        if (var<actj) actj = var;
      }

      actj+= 1.0;

      context->mbinfo[k++].act = actj;
    }
}

void 
simpeg_encode_rc_update_pict(simpeg_encode_context * context)
{
  double X;

  context->S = simpeg_encode_bitcount(context) - context->S; /* total # of bits in picture */
  context->R-= context->S; /* remaining # of bits in GOP */
  X = (int) floor(context->S*((0.5*(double)context->Q)/(context->mb_width*context->mb_height2)) + 0.5);
  context->d+= context->S - context->T;
  context->avg_act = context->actsum/(context->mb_width*context->mb_height2);

  switch (context->pict_type)
  {
  case I_TYPE:
    context->Xi = X;
    context->d0i = context->d;
    break;
  case P_TYPE:
    context->Xp = X;
    context->d0p = context->d;
    context->Np--;
    break;
  case B_TYPE:
    context->Xb = X;
    context->d0b = context->d;
    context->Nb--;
    break;
  }

  if (context->statfile != NULL)
  {
    fprintf(context->statfile,"\nrate control: end of picture\n");
    fprintf(context->statfile," actual number of bits: S=%d\n",context->S);
    fprintf(context->statfile," average quantization parameter Q=%.1f\n",
      (double)context->Q/(context->mb_width*context->mb_height2));
    fprintf(context->statfile," remaining number of bits in GOP: R=%d\n",context->R);
    fprintf(context->statfile,
      " global complexity measures (I,P,B): Xi=%d, Xp=%d, Xb=%d\n",
      context->Xi, context->Xp, context->Xb);
    fprintf(context->statfile,
      " virtual buffer fullness (I,P,B): d0i=%d, d0p=%d, d0b=%d\n",
      context->d0i, context->d0p, context->d0b);
    fprintf(context->statfile," remaining number of P pictures in GOP: Np=%d\n",context->Np);
    fprintf(context->statfile," remaining number of B pictures in GOP: Nb=%d\n",context->Nb);
    fprintf(context->statfile," average activity: avg_act=%.1f\n", context->avg_act);
  };
}

/* compute initial quantization stepsize (at the beginning of picture) */
int 
simpeg_encode_rc_start_mb(simpeg_encode_context * context)
{
  int mquant;

  if (context->q_scale_type)
  {
    mquant = (int) floor(2.0*context->d*31.0/context->r + 0.5);

    /* clip mquant to legal (linear) range */
    if (mquant<1)
      mquant = 1;
    if (mquant>112)
      mquant = 112;

    /* map to legal quantization level */
    mquant = context->non_linear_mquant_table[context->map_non_linear_mquant[mquant]];
  }
  else
  {
    mquant = (int) floor(context->d*31.0/context->r + 0.5);
    mquant <<= 1;

    /* clip mquant to legal (linear) range */
    if (mquant<2)
      mquant = 2;
    if (mquant>62)
      mquant = 62;

    context->prev_mquant = mquant;
  }

/*
  fprintf(statfile,"rc_start_mb:\n");
  fprintf(statfile,"mquant=%d\n",mquant);
*/

  return mquant;
}

/* Step 2: measure virtual buffer - estimated buffer discrepancy */
int 
simpeg_encode_rc_calc_mquant(simpeg_encode_context * context, int j)
{
  int mquant;
  double dj, Qj, actj, N_actj;

  /* measure virtual buffer discrepancy from uniform distribution model */
  dj = context->d + (simpeg_encode_bitcount(context)-context->S) - j*(context->T/(context->mb_width*context->mb_height2));

  /* scale against dynamic range of mquant and the bits/picture count */
  Qj = dj*31.0/context->r;
/*Qj = dj*(q_scale_type ? 56.0 : 31.0)/r;  */

  actj = context->mbinfo[j].act;
  context->actsum+= actj;

  /* compute normalized activity */
  N_actj = (2.0*actj+context->avg_act)/(actj+2.0*context->avg_act);

  if (context->q_scale_type)
  {
    /* modulate mquant with combined buffer and local activity measures */
    mquant = (int) floor(2.0*Qj*N_actj + 0.5);

    /* clip mquant to legal (linear) range */
    if (mquant<1)
      mquant = 1;
    if (mquant>112)
      mquant = 112;

    /* map to legal quantization level */
    mquant = context->non_linear_mquant_table[context->map_non_linear_mquant[mquant]];
  }
  else
  {
    /* modulate mquant with combined buffer and local activity measures */
    mquant = (int) floor(Qj*N_actj + 0.5);
    mquant <<= 1;

    /* clip mquant to legal (linear) range */
    if (mquant<2)
      mquant = 2;
    if (mquant>62)
      mquant = 62;

    /* ignore small changes in mquant */
    if (mquant>=8 && (mquant-context->prev_mquant)>=-4 && (mquant-context->prev_mquant)<=4)
      mquant = context->prev_mquant;

    context->prev_mquant = mquant;
  }

  context->Q+= mquant; /* for calculation of average mquant */

/*
  fprintf(statfile,"rc_calc_mquant(%d): ",j);
  fprintf(statfile,"bitcount=%d, dj=%f, Qj=%f, actj=%f, N_actj=%f, mquant=%d\n",
    bitcount(),dj,Qj,actj,N_actj,mquant);
*/

  return mquant;
}

/* compute variance of 8x8 block */
static double var_sblk(unsigned char *p, int lx)
{
  int i, j;
  unsigned int v, s, s2;

  s = s2 = 0;

  for (j=0; j<8; j++)
  {
    for (i=0; i<8; i++)
    {
      v = *p++;
      s+= v;
      s2+= v*v;
    }
    p+= lx - 8;
  }

  return s2/64.0 - (s/64.0)*(s/64.0);
}

/* VBV calculations
 *
 * generates warnings if underflow or overflow occurs
 */

/* vbv_end_of_picture
 *
 * - has to be called directly after writing picture_data()
 * - needed for accurate VBV buffer overflow calculation
 * - assumes there is no byte stuffing prior to the next start code
 */


void 
simpeg_encode_vbv_end_of_picture(simpeg_encode_context * context)
{
  context->bitcnt_EOP = simpeg_encode_bitcount(context);
  context->bitcnt_EOP = (context->bitcnt_EOP + 7) & ~7; /* account for bit stuffing */
}

/* calc_vbv_delay
 *
 * has to be called directly after writing the picture start code, the
 * reference point for vbv_delay
 */

void 
simpeg_encode_calc_vbv_delay(simpeg_encode_context * context)
{
  double picture_delay;

  /* number of 1/90000 s ticks until next picture is to be decoded */
  if (context->pict_type == B_TYPE)
  {
    if (context->prog_seq)
    {
      if (!context->repeatfirst)
        picture_delay = 90000.0/context->frame_rate; /* 1 frame */
      else
      {
        if (!context->topfirst)
          picture_delay = 90000.0*2.0/context->frame_rate; /* 2 frames */
        else
          picture_delay = 90000.0*3.0/context->frame_rate; /* 3 frames */
      }
    }
    else
    {
      /* interlaced */
      if (context->fieldpic)
        picture_delay = 90000.0/(2.0*context->frame_rate); /* 1 field */
      else
      {
        if (!context->repeatfirst)
          picture_delay = 90000.0*2.0/(2.0*context->frame_rate); /* 2 flds */
        else
          picture_delay = 90000.0*3.0/(2.0*context->frame_rate); /* 3 flds */
      }
    }
  }
  else
  {
    /* I or P picture */
    if (context->fieldpic)
    {
      if(context->topfirst==(context->pict_struct==TOP_FIELD))
      {
        /* first field */
        picture_delay = 90000.0/(2.0*context->frame_rate);
      }
      else
      {
        /* second field */
        /* take frame reordering delay into account */
        picture_delay = context->next_ip_delay - 90000.0/(2.0*context->frame_rate);
      }
    }
    else
    {
      /* frame picture */
      /* take frame reordering delay into account*/
      picture_delay = context->next_ip_delay;
    }

    if (!context->fieldpic || context->topfirst!=(context->pict_struct==TOP_FIELD))
    {
      /* frame picture or second field */
      if (context->prog_seq)
      {
        if (!context->repeatfirst)
          context->next_ip_delay = 90000.0/context->frame_rate;
        else
        {
          if (!context->topfirst)
            context->next_ip_delay = 90000.0*2.0/context->frame_rate;
          else
            context->next_ip_delay = 90000.0*3.0/context->frame_rate;
        }
      }
      else
      {
        if (context->fieldpic)
          context->next_ip_delay = 90000.0/(2.0*context->frame_rate);
        else
        {
          if (!context->repeatfirst)
            context->next_ip_delay = 90000.0*2.0/(2.0*context->frame_rate);
          else
            context->next_ip_delay = 90000.0*3.0/(2.0*context->frame_rate);
        }
      }
    }
  }

  if (context->decoding_time==0.0)
  {
    /* first call of calc_vbv_delay */
    /* we start with a 7/8 filled VBV buffer (12.5% back-off) */
    picture_delay = ((context->vbv_buffer_size*16384*7)/8)*90000.0/context->bit_rate;
    if (context->fieldpic)
      context->next_ip_delay = (int)(90000.0/context->frame_rate+0.5);
  }

  /* VBV checks */

  /* check for underflow (previous picture) */
  if (!context->low_delay && (context->decoding_time < context->bitcnt_EOP*90000.0/context->bit_rate))
  {
    /* picture not completely in buffer at intended decoding time */
    if (!context->quiet)
      SimpegWrite_warning(context,"vbv_delay underflow! (decoding_time=%.1f, t_EOP=%.1f)",
                          context->decoding_time, context->bitcnt_EOP*90000.0/context->bit_rate);
  }

  /* when to decode current frame */
  context->decoding_time += picture_delay;

  /* warning: bitcount() may overflow (e.g. after 9 min. at 8 Mbit/s */
  context->vbv_delay = (int)(context->decoding_time - simpeg_encode_bitcount(context)*90000.0/context->bit_rate);

  /* check for overflow (current picture) */
  if ((context->decoding_time - context->bitcnt_EOP*90000.0/context->bit_rate)
      > (context->vbv_buffer_size*16384)*90000.0/context->bit_rate)
  {
    if (!context->quiet)
      SimpegWrite_warning(context,"vbv_delay overflow!");
  }

  if (context->statfile != NULL)
    fprintf(context->statfile,
            "\nvbv_delay=%d (bitcount=%d, decoding_time=%.2f, bitcnt_EOP=%d)\n",
            context->vbv_delay,simpeg_encode_bitcount(context),context->decoding_time,context->bitcnt_EOP);

  if (context->vbv_delay<0)
  {
    if (!context->quiet)
      SimpegWrite_warning(context,"vbv_delay underflow: %d",context->vbv_delay);
    context->vbv_delay = 0;
  }

  if (context->vbv_delay>65535)
  {
    if (!context->quiet)
      SimpegWrite_warning(context,"vbv_delay overflow: %d",context->vbv_delay);
    context->vbv_delay = 65535;
  }
}
