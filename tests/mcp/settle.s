; Write a changing byte to the displayed page five times, about 1.4 frames
; apart (the same ~24000-cycle delay as flip.s, and for the same reason: long
; enough that each write lands in its own host frame), then stop writing and
; spin forever. Exercises `run`'s "screen_update" settle detector: the
; visible hash changes at each write, then holds identical once the writes
; stop, and after K=2 consecutive stable frame boundaries that quiescence is
; itself an "update" -- the single-buffered game's stand-in for a page flip.
;
; A single fixed address ($0400, the first byte of the displayed text page)
; is enough: the settle detector only cares whether the *hash* changed, not
; how much of the page did, so five stores of five different values in place
; are exactly as much "activity" as five stores fanned out across the page.
        org $0300
        lda $C051       ; TEXT on
        lda $C054       ; page 1
        lda #0
        sta $20         ; write counter, in zero page (X is delay's own)
write:  lda $20
        sta $0400       ; scribble on the displayed page
        jsr delay
        inc $20
        lda $20
        cmp #5
        bne write
spin:   jmp spin

delay:  ldy #24
oloop:  ldx #199
iloop:  dex
        bne iloop
        dey
        bne oloop
        rts
