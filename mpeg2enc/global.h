/* global.h, global variables, function prototypes                          */

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
#ifndef SIMPEG_ENCODE_GLOBAL_H
#define SIMPEG_ENCODE_GLOBAL_H

#include "mpeg2enc.h"

/* choose between declaration (GLOBAL undefined)
 * and definition (GLOBAL defined)
 * GLOBAL is defined in exactly one file (mpeg2enc.c)
 */

/* SimpegWrite spesifics */

#include "api.h"
#include <stdio.h>
#include <setjmp.h>

/* global variables moved into a structure which is passed around */
typedef struct {
  char version[64];
  char author[64];
  unsigned char zig_zag_scan[64];
  unsigned char alternate_scan[64];
  unsigned char default_intra_quantizer_matrix[64];
  unsigned char non_linear_mquant_table[32];
  unsigned char map_non_linear_mquant[113];
  unsigned char *newrefframe[3], *oldrefframe[3], *auxframe[3];
  unsigned char *neworgframe[3], *oldorgframe[3], *auxorgframe[3];
  unsigned char *predframe[3];
  short (*blocks)[64];
  unsigned char intra_q[64], inter_q[64];
  unsigned char chrom_intra_q[64],chrom_inter_q[64];
  int dc_dct_pred[3];
  struct mbinfo * mbinfo;
  struct motion_data * motion_data;
  /* clipping (=saturation) table */
  unsigned char *clp, *clp_org;
  
  /* name strings */
  char id_string[256], tplorg[256], tplref[256];
  char iqname[256], niqname[256];
  char statname[256];
  char errortext[256];

  FILE *outfile, *statfile; /* file descriptors */
  int inputtype; /* format of input frames */

  int quiet; /* suppress warnings */


  /* coding model parameters */

  int N; /* number of frames in Group of Pictures */
  int M; /* distance between I/P frames */
  int P; /* intra slice refresh interval */
  int nframes; /* total number of frames to encode */
  int frame0, tc0; /* number and timecode of first frame */
  int mpeg1; /* ISO/IEC IS 11172-2 sequence */
  int fieldpic; /* use field pictures */

  /* sequence specific data (sequence header) */

  int horizontal_size, vertical_size; /* frame size (pels) */
  int width, height; /* encoded frame size (pels) multiples of 16 or 32 */
  int chrom_width,chrom_height,block_count;
  int mb_width, mb_height; /* frame size (macroblocks) */
  int width2, height2, mb_height2, chrom_width2; /* picture size */
  int aspectratio; /* aspect ratio information (pel or display) */
  int frame_rate_code; /* coded value of frame rate */
  double frame_rate; /* frames per second */
  double bit_rate; /* bits per second */
  int vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
  int constrparms; /* constrained parameters flag (MPEG-1 only) */
  int load_iquant, load_niquant; /* use non-default quant. matrices */
  int load_ciquant,load_cniquant;


  /* sequence specific data (sequence extension) */

  int profile, level; /* syntax / parameter constraints */
  int prog_seq; /* progressive sequence */
  int chroma_format;
  int low_delay; /* no B pictures, skipped pictures */

  /* sequence specific data (sequence display extension) */
  
  int video_format; /* component, PAL, NTSC, SECAM or MAC */
  int color_primaries; /* source primary chromaticity coordinates */
  int transfer_characteristics; /* opto-electronic transfer char. (gamma) */
  int matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
  int display_horizontal_size, display_vertical_size; /* display size */

/* picture specific data (picture header) */

  int temp_ref; /* temporal reference */
  int pict_type; /* picture coding type (I, P or B) */
  int vbv_delay; /* video buffering verifier delay (1/90000 seconds) */

  /* picture specific data (picture coding extension) */

  int forw_hor_f_code, forw_vert_f_code;
  int back_hor_f_code, back_vert_f_code; /* motion vector ranges */
  int dc_prec; /* DC coefficient precision for intra coded blocks */
  int pict_struct; /* picture structure (frame, top / bottom field) */
  int topfirst; /* display top field first */
  /* use only frame prediction and frame DCT (I,P,B,current) */
  int frame_pred_dct_tab[3], frame_pred_dct;
  int conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
  int qscale_tab[3], q_scale_type; /* linear/non-linear quantizaton table */
  int intravlc_tab[3], intravlc; /* intra vlc format (I,P,B,current) */
  int altscan_tab[3], altscan; /* alternate scan (I,P,B,current) */
  int repeatfirst; /* repeat first field after second field */
  int prog_frame; /* progressive frame */

  /* putbits */
  unsigned char outbfr;
  int outcnt;
  int bytecnt;
  
  /* putseq */
  unsigned char *putseq_encode_neworg[3], *putseq_encode_newref[3];
  int putseq_encode_sxf, putseq_encode_syf, putseq_encode_sxb, putseq_encode_syb;
  int putseq_encode_f;

  int SimpegWrite_current_frame;

  SimpegWrite_error_cb SimpegWrite_error_cb_user;
  SimpegWrite_warning_cb SimpegWrite_warning_cb_user;
  SimpegWrite_progress_cb SimpegWrite_progress_cb_user;
  void * cbuserdata;

  /* rate control variables */
  int Xi, Xp, Xb, r, d0i, d0p, d0b;
  double avg_act;
  int R, T, d;
  double actsum;
  int Np, Nb, S, Q;
  int prev_mquant;

  /* ratectl */
  int bitcnt_EOP;
  double next_ip_delay; /* due to frame reordering delay */
  double decoding_time;

  // from readpic.c read_ppm()
  unsigned char *read_ppm_u444, *read_ppm_v444, *read_ppm_u422, *read_ppm_v422;

  // buffer M images
  unsigned char ** bufbuf;
  int SimpegWrite_current_input_frame;

  jmp_buf jmpbuf;

} simpeg_encode_context;


void SimpegWrite_error(simpeg_encode_context * context, const char *text, ...);
void SimpegWrite_warning(simpeg_encode_context * context, const char *text, ...);
int  SimpegWrite_progress(simpeg_encode_context * context, float sub, int current_frame, int num_frames);

/* End of SimpegWrite Specifics */


/* prototypes of global functions */

/* conform.c */
void simpeg_encode_range_checks(simpeg_encode_context * context);
void simpeg_encode_profile_and_level_checks(simpeg_encode_context * context);

/* fdctref.c */
void simpeg_encode_init_fdct(simpeg_encode_context * context);
void simpeg_encode_fdct(simpeg_encode_context * context, short *block);

/* idct.c */
void simpeg_encode_idct(simpeg_encode_context * context, short *block);
void simpeg_encode_init_idct(simpeg_encode_context * context);

/* motion.c */
void simpeg_encode_motion_estimation(simpeg_encode_context * context,
                                     unsigned char *oldorg, unsigned char *neworg,
                                     unsigned char *oldref, unsigned char *newref, unsigned char *cur,
                                     unsigned char *curref, int sxf, int syf, int sxb, int syb,
                                     struct mbinfo *mbi, int secondfield, int ipflag);

/* mpeg2enc.c */
void simpeg_encode_error(simpeg_encode_context * context, char *text);

/* predict.c */
void simpeg_encode_predict(simpeg_encode_context * context,
                           unsigned char *reff[], unsigned char *refb[],
                           unsigned char *cur[3], int secondfield, struct mbinfo *mbi);

/* putbits.c */
void simpeg_encode_initbits(simpeg_encode_context * context);
void simpeg_encode_putbits(simpeg_encode_context * context, int val, int n);
void simpeg_encode_alignbits(simpeg_encode_context * context);
int simpeg_encode_bitcount(simpeg_encode_context * context);

/* puthdr.c */
void simpeg_encode_putseqhdr(simpeg_encode_context * context);
void simpeg_encode_putseqext(simpeg_encode_context * context);
void simpeg_encode_putseqdispext(simpeg_encode_context * context);
void simpeg_encode_putuserdata(simpeg_encode_context * context, char *userdata);
void simpeg_encode_putgophdr(simpeg_encode_context * context, int frame, int closed_gop);
void simpeg_encode_putpicthdr(simpeg_encode_context * context);
void simpeg_encode_putpictcodext(simpeg_encode_context * context);
void simpeg_encode_putseqend(simpeg_encode_context * context);

/* putmpg.c */
void simpeg_encode_putintrablk(simpeg_encode_context * context, short *blk, int cc);
void simpeg_encode_putnonintrablk(simpeg_encode_context * context, short *blk);
void simpeg_encode_putmv(simpeg_encode_context * context, int dmv, int f_code);

/* putpic.c */
void simpeg_encode_putpict(simpeg_encode_context * context, unsigned char *frame);

/* putseq.c */
void simpeg_encode_putseq(simpeg_encode_context * context);

/* putvlc.c */
void simpeg_encode_putDClum(simpeg_encode_context * context, int val);
void simpeg_encode_putDCchrom(simpeg_encode_context * context, int val);
void simpeg_encode_putACfirst(simpeg_encode_context * context, int run, int val);
void simpeg_encode_putAC(simpeg_encode_context * context, int run, int signed_level, int vlcformat);
void simpeg_encode_putaddrinc(simpeg_encode_context * context, int addrinc);
void simpeg_encode_putmbtype(simpeg_encode_context * context, int pict_type, int mb_type);
void simpeg_encode_putmotioncode(simpeg_encode_context * context, int motion_code);
void simpeg_encode_putdmv(simpeg_encode_context * context, int dmv);
void simpeg_encode_putcbp(simpeg_encode_context * context, int cbp);

/* quantize.c */
int simpeg_encode_quant_intra(simpeg_encode_context * context,
                              short *src, short *dst, int dc_prec,
                              unsigned char *quant_mat, int mquant);
int simpeg_encode_quant_non_intra(simpeg_encode_context * context,
                                  short *src, short *dst,
                                  unsigned char *quant_mat, int mquant);
void simpeg_encode_iquant_intra(simpeg_encode_context * context,
                                short *src, short *dst, int dc_prec,
                                unsigned char *quant_mat, int mquant);
void simpeg_encode_iquant_non_intra(simpeg_encode_context * context,
                                    short *src, short *dst,
                                    unsigned char *quant_mat, int mquant);

/* ratectl.c */
void simpeg_encode_rc_init_seq(simpeg_encode_context * context);
void simpeg_encode_rc_init_GOP(simpeg_encode_context * context, int np, int nb);
void simpeg_encode_rc_init_pict(simpeg_encode_context * context, unsigned char *frame);
void simpeg_encode_rc_update_pict(simpeg_encode_context * context);
int simpeg_encode_rc_start_mb(simpeg_encode_context * context);
int simpeg_encode_rc_calc_mquant(simpeg_encode_context * context, int j);
void simpeg_encode_vbv_end_of_picture(simpeg_encode_context * context);
void simpeg_encode_calc_vbv_delay(simpeg_encode_context * context);

/* readpic.c */
void simpeg_encode_readframe(simpeg_encode_context * context,
                             char *fname, unsigned char *frame[]);

/* stats.c */
void simpeg_encode_calcSNR(simpeg_encode_context * context,
                           unsigned char *org[3], unsigned char *rec[3]);
void simpeg_encode_stats(simpeg_encode_context * context);

/* transfrm.c */
void simpeg_encode_transform(simpeg_encode_context * context,
                             unsigned char *pred[], unsigned char *cur[],
                             struct mbinfo *mbi, short blocks[][64]);
void simpeg_encode_itransform(simpeg_encode_context * context,
                              unsigned char *pred[], unsigned char *cur[],
                              struct mbinfo *mbi, short blocks[][64]);
void simpeg_encode_dct_type_estimation(simpeg_encode_context * context,
                                       unsigned char *pred, unsigned char *cur,
                                       struct mbinfo *mbi);

/* writepic.c */
void simpeg_encode_writeframe(simpeg_encode_context * context,
                              char *fname, unsigned char *frame[]);


#endif /* SIMPEG_ENCODE_GLOBAL_H */
