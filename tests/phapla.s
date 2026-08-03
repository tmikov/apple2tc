        org     $300

        ldx     #10
        jsr     func1
        brk     #1

        org     $320
func1:
        pha
        lda     $1001
        beq     skip
        sta     $1000
skip:
        pla
        rts
