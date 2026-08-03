        org     $300

        ldx     #10
        jsr     func1
        brk     #1

        org     $320
func1:
        lda     $1001
        beq     skip
        pha
skip:
        lda     $1002
        rts
