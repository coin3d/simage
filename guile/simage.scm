; simage.scm -- a guile module for interfacing with libsimage -*- scheme -*-
;
; Written by Lars J. Aas <larsa@coin3d.org>.
; This file is Public Domain.
;
; $Revision$

(define-module (coin simage))

(let ((libhandle (dynamic-link "libguilesimage.so")))
  (dynamic-call "guilesimage_init" libhandle))

(export
  simage-version-major     ;                                  => integer
  simage-version-minor     ;                                  => integer
  simage-version-micro     ;                                  => integer
  simage-get-last-error    ;                                  => string

  simage-load-supported?   ; filename                         => boolean
  simage-load              ; filename                         => simage-image
  simage-resize            ; simage-image width height        => simage-image
  simage-save-supported?   ; extension                        => boolean
  simage-save              ; simage-image filename extension  => boolean

  simage-image-width       ; simage-image                     => integer
  simage-image-height      ; simage-image                     => integer
  simage-image-components  ; simage-image                     => integer
  )

; EOF ----------------------------------------------------------------------
