; Mutually recursive subroutines, which make the call graph cyclic.
;
; identifySimpleRoutines extracts both funca and funcb into IR functions, and
; each one calls the other, so no topological order over the call graph exists.
; CPURegLiveness must not depend on one. Two distinct things break without that:
;
;   - Seeding FuncData in callee-first order aborts on the edge that closes the
;     cycle, because the callee has no entry yet when the Call is looked up.
;   - Accumulating a function's `modified` set during that same pass reads a
;     callee kill set that is still empty, under-approximating it. `modified`
;     seeds liveOut, which is only ever intersected afterwards, so too small a
;     value here lets dce() drop stores that are still live.
;
; funcb deliberately writes X and Y, which funca never touches, so the second
; failure is visible: with `modified` computed too early the X and Y stores in
; the entry function disappear from the output.

        org     $300

        ldx     #10
        jsr     funca
        brk     #1

        org     $320
funca:
        lda     $1001
        beq     skipa
        jsr     funcb
skipa:
        sta     $1000
        rts

        org     $340
funcb:
        ldx     $1002
        stx     $1003
        ldy     $1004
        sty     $1005
        beq     skipb
        jsr     funca
skipb:
        rts
