#! /bin/sh
# simage-convert.scm
# Written by Lars J. Aas <larsa@sim.no>.
# This file is in the Public Domain.
exec guile -e main -s $0 $@
!#

(use-modules
  (ice-9 format)
  (coin simage))

(define *scale-x* 1)
(define *scale-y* 1)
(define *resize-x* 0)
(define *resize-y* 0)
(define *input* '())
(define *output* '())

(define *task* '()) ; 'copy | 'resize | 'scale | 'exit | 'fail

(define (simage-convert-usage)
  (display "Usage: simage-convert [options] <infile> <outfile>\n")
  (display "Options:\n")
  (display "  --help\n")
  (display "  --version\n")
  (display "  --scale <factor> <factor>\n")
  (display "  --resize [<size> | -1] [<size> | -1]\n"))

(define (simage-convert-version)
  (display "simage-convert v1.0\n")
  (display (format "using libsimage v~A.~A.~A\n"
                   (simage-version-major)
                   (simage-version-minor)
                   (simage-version-micro))))

(define (extension-of filename)
  (define (head-until character-list character)
    (cond ((eq? character-list '()) '())
          ((eq? (car character-list) '()) '())
          ((eq? (car character-list) character) '())
          (else (cons (car character-list)
                      (head-until (cdr character-list) character)))))
  (list->string (reverse! (head-until (reverse! (string->list filename)) #\.))))

(define (simage-resize-task)
  (cond ((eq? *input* '())
          (display "error: no input\n"))
        ((eq? *output* '())
          (display "error: no output\n"))
        ((not (simage-load-supported? *input*))
          (display "error: not supported input format\n"))
        ((not (simage-save-supported? (extension-of *output*)))
          (display "error: not supported output format\n"))
        (else
          (let* ((image       (simage-load *input*))
                 (width       (simage-image-width image))
                 (height      (simage-image-height image))
                 (new-width   *resize-x*)
                 (new-height  *resize-y*)
                 (scaled '()))
            (cond ((and (eq? new-width -1) (eq? new-height -1))
                    (display "error: both width and height can't be -1\n"))
                  ((eq? new-width -1)
                    (set! new-width (round (* width (/ new-height height)))))
                  ((eq? new-height -1)
                    (set! new-height (round (* height (/ new-width width))))))
            (set! scaled (simage-resize image new-width new-height))
            ; (display (format "old: ~A\nnew: ~A\n" image scaled))
            (if (not (simage-save scaled *output* (extension-of *output*)))
                (display (format "error: \"~A\"\n"
                                 (simage-get-last-error))))))))

(define (simage-scale-task)
  (cond ((eq? *input* '())
          (display "error: no input\n"))
        ((eq? *output* '())
          (display "error: no output\n"))
        ((not (simage-load-supported? *input*))
          (display "error: not supported input format\n"))
        ((not (simage-save-supported? (extension-of *output*)))
          (display "error: not supported output format\n"))
        (else
          (let* ((image      (simage-load *input*))
                 (width      (simage-image-width image))
                 (height     (simage-image-height image))
                 (new-width  (* *scale-x* width))
                 (new-height (* *scale-y* height))
                 (scaled     '()))
            (set! scaled (simage-resize image new-width new-height))
            ; (display (format "old: ~A\nnew: ~A\n" image scaled))
            (if (not (simage-save scaled *output* (extension-of *output*)))
                (display (format "error: \"~A\"\n"
                                 (simage-get-last-error))))))))

(define (simage-copy-task)
  (cond ((eq? *input* '())
          (display "error: no input\n"))
        ((eq? *output* '())
          (display "error: no output\n"))
        ((not (simage-load-supported? *input*))
          (display "error: not supported input format\n"))
        ((not (simage-save-supported? (extension-of *output*)))
          (display "error: not supported output format\n"))
        (else
          (let ((image (simage-load *input*)))
            (if (not (simage-save image *output* (extension-of *output*)))
                (display (format "error: \"~A\"\n"
                                 (simage-get-last-error))))))))

(define (simage-no-task)
  (display "error: invalid command line: see --help option\n")
  -1)

(define (parse-args args)
  ; (display (format "arg: \"~A\"\n" (car args)))
  (cond ((eq? args '()))
        ((eq? (car args) '()))
        ((equal? (car args) "--help")
          (simage-convert-usage)
          (set! *task* 'exit))
        ((equal? (car args) "--version")
          (simage-convert-version)
          (set! *task* 'exit))
        ((equal? (car args) "--scale")
          (if (< (length args) 3)
              (begin
                (set! *task* 'exit)
                (display "error: \"--scale\": wrong number of arguments\n"))
              (begin
                (set! *task* 'scale)
                (set! *scale-x* (string->number (cadr args)))
                (set! *scale-y* (string->number (caddr args)))
                (parse-args (cdddr args)))))
        ((equal? (car args) "--resize")
          (if (< (length args) 3)
              (begin
                (set! *task* 'exit)
                (display "error: \"--resize\": wrong number of arguments\n"))
              (begin
                (set! *task* 'resize)
                (set! *resize-x* (string->number (cadr args)))
                (set! *resize-y* (string->number (caddr args)))
                (parse-args (cdddr args)))))
        (else
          (if (= (length args) 2)
              (begin
                (set! *input* (car args))
                (set! *output* (cadr args))
                (if (eq? *task* '()) (set! *task* 'copy)))
              (begin
                (set! *task* 'exit)
                (display "error: wrong number of arguments\n"))))))

(define (main args)
  (parse-args (cdr args))
  (cond ((eq? *task* 'exit)   0)
        ((eq? *task* 'fail)   -1)
        ((eq? *task* 'copy)   (simage-copy-task))
        ((eq? *task* 'resize) (simage-resize-task))
        ((eq? *task* 'scale)  (simage-scale-task))
        (else
          (simage-no-task))))

; EOF ----------------------------------------------------------------------
