#ifndef SIMPEG_ENCODE_PUTSEQ_H
#define SIMPEG_ENCODE_PUTSEQ_H

#include "global.h"

void SimpegWrite_putseq_begin(simpeg_encode_context * context);
void SimpegWrite_putseq_encode_bitmap(simpeg_encode_context * context,
                                      const unsigned char *buffer);
void SimpegWrite_putseq_end(simpeg_encode_context * context);

#endif /* SIMPEG_ENCODE_PUTSEQ_H */
