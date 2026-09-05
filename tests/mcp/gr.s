; Lo-res: fill row 0 with colour 1 and column 0 with colour 12, then spin.
; The GR page is the text page, two 4-bit cells per byte: low nibble is the
; even (upper) row of the pair, high nibble the odd (lower) one.
        org $0300
        lda $C050       ; graphics
        lda $C053       ; mixed
        lda $C054       ; page 1
        lda $C056       ; lo-res
        ldx #0
top:    lda #$01        ; row 0 only: the low nibble of the first line's bytes
        sta $0400,x
        inx
        cpx #40
        bne top
        lda #$CC        ; both cells of column 0, rows 2 and 3
        sta $0480
spin:   jmp spin
