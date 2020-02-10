/***************************************************************************

getarg.h - Support routines for the giflib utilities

SPDX-License-Identifier: MIT

**************************************************************************/

#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <gif_lib.h>

/******************************************************************************
 Color table quantization
******************************************************************************/
int GifQuantizeBuffer(unsigned int Width, unsigned int Height,
                   int *ColorMapSize, GifByteType * RedInput,
                   GifByteType * GreenInput, GifByteType * BlueInput,
                   GifByteType * OutputBuffer,
                   GifColorType * OutputColorMap);

#endif /*QUANTIZE_H */

/* end */
