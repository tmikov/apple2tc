        org     $1100 - $23
        lda     $C054
        lda     $C052
        lda     $C051
        lda     $C057
        ldy     #0
        sty     $1721
        sty     $1722
        dey
        sty     $90
        lda     #$5f
        sta     $91
        sty     $92
        lda     #$3f
        sta     $93
        bne     $114c
