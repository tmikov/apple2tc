; A subroutine entry that is also the return point of a call.
;
; `draw` calls `masks` and then falls straight into `plot`, so `plot` is both a
; subroutine in its own right -- `line` calls it directly -- and the block that
; `masks` returns to. That gives `plot` two predecessor edges for one control
; transfer: the JSR at $310, whose *fall* block is `plot`, and the RTS at the
; end of `masks`, which returns there.
;
; Both legs describe the same transfer, so if the JSR leg is acceptable the RTS
; leg carries no additional flow. `plot` must be recovered as a routine.
;
; This is Snake Byte's $60E4 / $6127 / $60E7 / $615A shape, which used to be
; rejected with "invalid predecessor inst RTS at $6147" and took five further
; routines down with it through the invalid-JSR cascade.

        org     $300
start:
        jsr     draw
        jsr     line
        brk     #0

        org     $310
draw:
        jsr     masks
; Falls through. `plot` is the return point of the call above *and* a callee of
; `line` below.
plot:
        lda     #$01
        sta     $1000
        rts

        org     $320
masks:
        lda     #$02
        sta     $1001
        rts

        org     $330
line:
        jsr     masks
        jsr     plot
        rts
