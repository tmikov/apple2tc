; Code, declared data, zero fill and unidentified bytes, so the coverage report
; has one of each to classify.

        org     $300
start:
        jsr     sub
        rts

        org     $310
sub:
        lda     #$01
        sta     $1000
        rts

        org     $320            ; declared in coverage-data.txt
blob:
        hex     0102030405060708

        org     $330            ; left undeclared, and nonzero: the report must
        hex     AABBCC          ; single this out as what is left to identify
