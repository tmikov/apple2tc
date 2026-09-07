; Draw a constant solid white line at HGR row 8 ($2080-$20A7), then flicker a
; second solid white line at HGR row 32 ($2200-$2227) on and off -- present
; for one delay window, blanked for the next, forever. This is deliberately
; the single-buffered-game failure `blend` exists to fix in miniature: any one
; captured frame can show the first line and not the second, but the second
; is drawn at every other beat, so OR-ing a couple of consecutive frames'
; pages together always shows both.
;
; The delay is the same ~24000-cycle loop flip.s uses (24 outer passes of a
; 199-count inner loop -- see beep.s for the exact NMOS timing an inner pass
; like this costs), chosen there for the same reason it works here: longer
; than one host frame (17050 cycles) but shorter than two, so a captured
; frame can land on either phase and consecutive phases fall in different
; frames.
        org $0300
        lda $C050       ; graphics (not text)
        lda $C052       ; full screen (not mixed)
        lda $C057       ; hires on
        lda $C054       ; page 1

        ; The constant line: row 8, drawn once and never touched again.
        ldx #0
fill1:  lda #$7F
        sta $2080,x
        inx
        cpx #40
        bne fill1

loop:   ; Second line on: row 32.
        ldx #0
on1:    lda #$7F
        sta $2200,x
        inx
        cpx #40
        bne on1
        jsr delay
        ; Second line off.
        ldx #0
off1:   lda #$00
        sta $2200,x
        inx
        cpx #40
        bne off1
        jsr delay
        jmp loop

delay:  ldy #24
oloop:  ldx #199
iloop:  dex
        bne iloop
        dey
        bne oloop
        rts
