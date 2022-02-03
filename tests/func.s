        org     $300

        ldx     #10
        jsr     func1
        lda     #>lab1
        pha
        lda     $FFE3,Y
        pha
        ldx     #5
        jmp     func1

lab1:
        sta     $1000
        brk     #1

        org     $320
func1:
        inc     $1001
        dex
        bne     func1
        rts
