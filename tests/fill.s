; Minimal binary for checking what init_emulated() emits.
        org     $300
        lda     #$01
        sta     $1000
        rts
