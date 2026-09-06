/***************************************************************************

getarg.h - Support routines for the giflib utilities

SPDX-License-Identifier: MIT

**************************************************************************/

#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <gif_lib.h>

// giflib 5.2.0 moved GifQuantizeBuffer() out of libgif, and the const
// GifByteType * signature appears in 5.2.2.  A discussion from the
// giflib/Cinelerra ecosystem confirms that 5.1.6 and 5.2.1 still used
// the non-const form, while 5.2.2 changed it.
//
// gif_lib.h exposes GIFLIB_MAJOR, GIFLIB_MINOR, and GIFLIB_RELEASE specifically
// for conditional compilation.

#if (GIFLIB_MAJOR > 5) || \
    (GIFLIB_MAJOR == 5 && GIFLIB_MINOR > 2) || \
    (GIFLIB_MAJOR == 5 && GIFLIB_MINOR == 2 && GIFLIB_RELEASE >= 2)
#define SIMAGE_GIF_QUANTIZE_CONST const
#else
#define SIMAGE_GIF_QUANTIZE_CONST
#endif

/******************************************************************************
 Color table quantization
******************************************************************************/
int GifQuantizeBuffer(unsigned int Width,
                   unsigned int Height,
                   int *ColorMapSize,
                   SIMAGE_GIF_QUANTIZE_CONST GifByteType * RedInput,
                   SIMAGE_GIF_QUANTIZE_CONST GifByteType * GreenInput,
                   SIMAGE_GIF_QUANTIZE_CONST GifByteType * BlueInput,
                   GifByteType * OutputBuffer,
                   GifColorType * OutputColorMap);

#endif /*QUANTIZE_H */

/* end */
