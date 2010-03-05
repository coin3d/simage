;;;
; Copyright (c) Kongsberg Oil & Gas Technologies
;
; Permission to use, copy, modify, and distribute this software for any
; purpose with or without fee is hereby granted, provided that the above
; copyright notice and this permission notice appear in all copies.
;
; THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
; WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
; ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
; WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
; ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
; OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
;

; simage.scm -- a guile module for interfacing with libsimage -*- scheme -*-
;
; Written by Lars J. Aas <larsa@coin3d.org>.
;
; $Revision$
;
; TODO:
; - add some high-level scheme-level drawing primitives

(define-module (coin simage))

(let ((libhandle (dynamic-link "libguilesimage.so")))
  (dynamic-call "guilesimage_init" libhandle))

(export
  simage-version-major     ;                                  => integer
  simage-version-minor     ;                                  => integer
  simage-version-micro     ;                                  => integer
  simage-get-last-error    ;                                  => string

  ; #<simage-image>
  make-simage-image        ; width height [components = 4]    => simage-image
  simage-image-width       ; simage-image                     => integer
  simage-image-height      ; simage-image                     => integer
  simage-image-components  ; simage-image                     => integer

  simage-load-supported?   ; filename                         => boolean
  simage-load              ; filename                         => simage-image
  simage-save-supported?   ; extension                        => boolean
  simage-save              ; simage-image filename extension  => boolean
  simage-resize            ; simage-image width height        => simage-image

  ; #<simage-pixel>
  make-simage-pixel        ; red green blue [alpha = 255]     => simage-pixel
  simage-pixel-get-red     ; simage-pixel                     => integer
  simage-pixel-get-green   ; simage-pixel                     => integer
  simage-pixel-get-blue    ; simage-pixel                     => integer
  simage-pixel-get-alpha   ; simage-pixel                     => integer

  ; #<simage-image> #<simage-pixel>
  simage-image-get-pixel   ; simage-image x y                 => simage-pixel
  simage-image-set-pixel!  ; simage-image x y simage-pixel    => 
  )

; EOF ----------------------------------------------------------------------
