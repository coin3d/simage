/* putvlc.c, generation of variable length codes                            */

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
#include "vlc.h"

/* private prototypes */
static void putDC(simpeg_encode_context * context, sVLCtable *tab, int val);

/* generate variable length code for luminance DC coefficient */
void 
simpeg_encode_putDClum(simpeg_encode_context * context, int val)
{
  putDC(context, DClumtab,val);
}

/* generate variable length code for chrominance DC coefficient */
void 
simpeg_encode_putDCchrom(simpeg_encode_context * context,int val)
{
  putDC(context, DCchromtab,val);
}

/* generate variable length code for DC coefficient (7.2.1) */
static void 
putDC(simpeg_encode_context * context,sVLCtable *tab, int val)
{
  int absval, size;

  absval = (val<0) ? -val : val; /* abs(val) */

  if (absval>2047 || (context->mpeg1 && absval>255))
  {
    /* should never happen */
    sprintf(context->errortext,"DC value out of range (%d)\n",val);
    simpeg_encode_error(context, context->errortext);
  }

  /* compute dct_dc_size */
  size = 0;

  while (absval)
  {
    absval >>= 1;
    size++;
  }

  /* generate VLC for dct_dc_size (Table B-12 or B-13) */
  simpeg_encode_putbits(context, tab[size].code,tab[size].len);

  /* append fixed length code (dc_dct_differential) */
  if (size!=0)
  {
    if (val>=0)
      absval = val;
    else
      absval = val + (1<<size) - 1; /* val + (2 ^ size) - 1 */
    simpeg_encode_putbits(context,absval,size);
  }
}

/* generate variable length code for first coefficient
 * of a non-intra block (7.2.2.2) */
void 
simpeg_encode_putACfirst(simpeg_encode_context * context,int run, int val)
{
  if (run==0 && (val==1 || val==-1)) /* these are treated differently */
    simpeg_encode_putbits(context,2|(val<0),2); /* generate '1s' (s=sign), (Table B-14, line 2) */
  else
    simpeg_encode_putAC(context,run,val,0); /* no difference for all others */
}

/* generate variable length code for other DCT coefficients (7.2.2) */
void 
simpeg_encode_putAC(simpeg_encode_context * context,int run, int signed_level, int vlcformat)
{
  int level, len;
  VLCtable *ptab;

  level = (signed_level<0) ? -signed_level : signed_level; /* abs(signed_level) */

  /* make sure run and level are valid */
  if (run<0 || run>63 || level==0 || level>2047 || (context->mpeg1 && level>255))
  {
    sprintf(context->errortext,"AC value out of range (run=%d, signed_level=%d)\n",
            run,signed_level);
    simpeg_encode_error(context, context->errortext);
  }

  len = 0;

  if (run<2 && level<41)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab1a[run][level-1];
    else
      ptab = &dct_code_tab1[run][level-1];

    len = ptab->len;
  }
  else if (run<32 && level<6)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab2a[run-2][level-1];
    else
      ptab = &dct_code_tab2[run-2][level-1];

    len = ptab->len;
  }

  if (len!=0) /* a VLC code exists */
  {
    simpeg_encode_putbits(context,ptab->code,len);
    simpeg_encode_putbits(context,signed_level<0,1); /* sign */
  }
  else
  {
    /* no VLC for this (run, level) combination: use escape coding (7.2.2.3) */
    simpeg_encode_putbits(context,1l,6); /* Escape */
    simpeg_encode_putbits(context,run,6); /* 6 bit code for run */
    if (context->mpeg1)
    {
      /* ISO/IEC 11172-2 uses a 8 or 16 bit code */
      if (signed_level>127)
        simpeg_encode_putbits(context,0,8);
      if (signed_level<-127)
        simpeg_encode_putbits(context,128,8);
      simpeg_encode_putbits(context,signed_level,8);
    }
    else
    {
      /* ISO/IEC 13818-2 uses a 12 bit code, Table B-16 */
      simpeg_encode_putbits(context,signed_level,12);
    }
  }
}

/* generate variable length code for macroblock_address_increment (6.3.16) */
void 
simpeg_encode_putaddrinc(simpeg_encode_context * context,int addrinc)
{
  while (addrinc>33)
  {
    simpeg_encode_putbits(context,0x08,11); /* macroblock_escape */
    addrinc-= 33;
  }

  simpeg_encode_putbits(context,addrinctab[addrinc-1].code,addrinctab[addrinc-1].len);
}

/* generate variable length code for macroblock_type (6.3.16.1) */
void 
simpeg_encode_putmbtype(simpeg_encode_context * context,int pict_type, int mb_type)
{
  simpeg_encode_putbits(context,mbtypetab[pict_type-1][mb_type].code,
          mbtypetab[pict_type-1][mb_type].len);
}

/* generate variable length code for motion_code (6.3.16.3) */
void 
simpeg_encode_putmotioncode(simpeg_encode_context * context,int motion_code)
{
  int abscode;

  abscode = (motion_code>=0) ? motion_code : -motion_code; /* abs(motion_code) */
  simpeg_encode_putbits(context,motionvectab[abscode].code,motionvectab[abscode].len);
  if (motion_code!=0)
    simpeg_encode_putbits(context,motion_code<0,1); /* sign, 0=positive, 1=negative */
}

/* generate variable length code for dmvector[t] (6.3.16.3), Table B-11 */
void 
simpeg_encode_putdmv(simpeg_encode_context * context,int dmv)
{
  if (dmv==0)
    simpeg_encode_putbits(context,0,1);
  else if (dmv>0)
    simpeg_encode_putbits(context,2,2);
  else
    simpeg_encode_putbits(context,3,2);
}

/* generate variable length code for coded_block_pattern (6.3.16.4)
 *
 * 4:2:2, 4:4:4 not implemented
 */
void 
simpeg_encode_putcbp(simpeg_encode_context * context,int cbp)
{
  simpeg_encode_putbits(context,cbptable[cbp].code,cbptable[cbp].len);
}
