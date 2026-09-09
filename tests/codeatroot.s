; Code that only exists once the program has written it.
;
; A --code-at *edge* needs a branch at the origin to hang itself on. Self-
; modifying code does not offer one: the transfer is a byte pattern stored into
; a slot, and in the image as it sits the slot holds something else entirely.
; Neptune is the case this was built for -- it picks its input device once at
; the title screen and then writes `JSR $36B0` into $1060, so the 50-byte
; paddle reader at $36B0 has no static reference anywhere and the tracer emits
; all of it as DFB.
;
; Here the same shape in miniature. The slot at $320 is filled in at runtime
; with `JSR handler / RTS`; statically it is two BRKs, and `handler` is named
; only by two immediate bytes, which are not references. So nothing reaches
; $330 and it stays data.
;
; Baselines: codeatroot.ir (with the root asserted) and codeatroot-noroot.ir
; (without), so the test pins what the root changes rather than that it runs.

        org     $300

        lda     #$20            ; JSR
        sta     $320
        lda     #<handler       ; an immediate, not a reference
        sta     $321
        lda     #>handler
        sta     $322
        lda     #$60            ; RTS
        sta     $323
        jsr     $320
        brk     #0

; The slot. The tracer follows the JSR above into it and finds BRK.
        org     $320
slot:
        brk     #0
        brk     #0

; Unreferenced by anything static.
        org     $330
handler:
        lda     #$42
        sta     $1000
        rts
