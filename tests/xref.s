; References to a target range, of both kinds `id`'s xref command looks for.
;
; $310..$31F is the target. Reaching it takes a direct call, an absolute read of
; a byte inside it, an indirect jump through a vector, and the vector itself --
; a little-endian word that no instruction operand mentions. That last one is
; the case the data scan exists for: it is how an otherwise unreferenced region
; usually turns out to be reached.
;
; The scan decodes at every offset rather than following instruction
; boundaries, so it also reports hits inside longer instructions. That is
; expected, and the baseline pins it down rather than pretending otherwise.

        org     $300
start:
        jsr     target          ; code: a call
        lda     target+3        ; code: an absolute read into the range
        jmp     (vector)        ; code: an indirect jump
        brk     #0

        org     $310
target:
        lda     #$01
        sta     $1000
        rts

        org     $320
vector:
        dw      target          ; data: a word pointing into the range
