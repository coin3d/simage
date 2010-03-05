/*
 * Copyright (c) Kongsberg Oil & Gas Technologies
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <simage.h>

int
main(int argc, char ** argv)
{
  int i;

  if (argc < 2) {
    (void)fprintf(stderr, "\n\tUsage: %s IMGFILE [IMGFILES]\n\n", argv[0]);
    exit(1);
  }

  for (i=1; i < argc; i++) {
    const char * filename = argv[i];
    if (!simage_check_supported(filename)) {
      (void)fprintf(stdout, "``%s'' is not in a supported format\n", filename);
    }
    else {
      int w, h, comp;
      unsigned char * buffer = simage_read_image(filename, &w, &h, &comp);

      (void)fprintf(stdout, "``%s'':\n", filename);

      if (!buffer) {
        (void)fprintf(stdout, "\t** couldn't load file: \"%s\"\n",
                      simage_get_last_error());
      }
      else {
        (void)fprintf(stdout, "\twidth: %d, height: %d, components: %d\n",
                      w, h, comp);
        simage_free_image(buffer);
      }
    }
    (void)fprintf(stdout, "\n");
  }

  /* FIXME: should write testcode for the plugin API functions
     aswell. 20001018 mortene. */

  return 0;
}
