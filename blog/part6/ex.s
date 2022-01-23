        org     $300

        tsx
        inx
        inx
        ldy     $101,x
        iny
        sty     $90

        lda     $91
        sta     $92

        ldy     #0
        sty     $1721
        dey
        sty     $93

        bne     $350
