        org     $300
        lda     $1000
        tax
        stx     $1001
        lda     $1002
        inc     $1002
        tax
        stx     $1003
        beq     L1

        lda     $2000
        tax
        inx
        txa
        sta     $2005

        lda     $2006
        beq     L1

        lda     $3000
        tax
        inx
        lda     #1
        sta     $3000
        txa
        sta     $3005
L1:
