        org     $300

        sec
; const & const
        lda     #$FF
        and     #$55
        sta     $1000
; x & 0
        lda     $1001
        and     #$00
        sta     $1001
; x & 0xff
        lda     $1002
        and     #$FF
        sta     $1003
; ((x & const1) &const2)
        lda     $1004
        and     #$55
        and     #$0F
        sta     $1004
; ((x & 0xff) << 1) & 0xff
        lda     $1005
        asl     A
        asl     A
        asl     A
        sta     $1005

