; Poll the keyboard the way a game does and *never* touch KBDSTRB, then show
; the latch. A key delivered as type-ahead only reaches the latch while the
; strobe is clear, so against this program every key after the first would stage
; behind the strobe and never arrive; a key delivered as a keystroke overwrites
; the latch and always does. That difference is the whole point of the test.
        org $0300
        lda $C051       ; TEXT
        lda $C054       ; page 1
loop:   lda $C000       ; KBD -- bit 7 set while a key is waiting
        bpl loop
        sta $0400       ; top-left of the text page; high bit set = normal video
        jmp loop        ; deliberately no LDA $C010
