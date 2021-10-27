/* writepic.c, write reconstructed pictures                                 */

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

void 
simpeg_encode_writeframe(simpeg_encode_context * context,
                         char *fname, unsigned char **frame)
{
  int chrom_hsize, chrom_vsize;
  char name[128];
  FILE *fd;

  chrom_hsize = (context->chroma_format==CHROMA444) ? context->horizontal_size
                                           : context->horizontal_size>>1;

  chrom_vsize = (context->chroma_format!=CHROMA420) ? context->vertical_size
                                           : context->vertical_size>>1;

  if (fname[0]=='-')
    return;
  
  /* Y */
  sprintf(name,"%s.Y",fname);
  if (!(fd = fopen(name,"wb")))
  {
    sprintf(context->errortext,"Couldn't create %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  fwrite(frame[0],1,(size_t)context->horizontal_size*context->vertical_size,fd);
  fclose(fd);

  /* Cb */
  sprintf(name,"%s.U",fname);
  if (!(fd = fopen(name,"wb")))
  {
    sprintf(context->errortext,"Couldn't create %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  fwrite(frame[1],1,(size_t)chrom_hsize*chrom_vsize,fd);
  fclose(fd);

  /* Cr */
  sprintf(name,"%s.V",fname);
  if (!(fd = fopen(name,"wb")))
  {
    sprintf(context->errortext,"Couldn't create %s\n",name);
    simpeg_encode_error(context,context->errortext);
  }
  fwrite(frame[2],1,(size_t)chrom_hsize*chrom_vsize,fd);
  fclose(fd);
}
