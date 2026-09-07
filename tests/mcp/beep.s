; Toggle the speaker on a fixed period, then go quiet, for a predictable tone.
;
; Every trip around `tone` reads $C030 once (one toggle) and burns cycles in
; a dex/bne delay loop first. Counting NMOS 6502 timings exactly: ldx #199 is
; 2 cycles; the delay loop decrements X from 199 to 0, so it runs 199 times,
; 198 of them taking the backward branch (2 + 3 cycles) and the last falling
; through (2 cycles) -- 198*5 + 2 = 992, plus the two dex/bne pairs' shared
; cost already counted, i.e. 199*2 + 198*3 + 1*2 = 994 cycles total; dey is 2
; more; the closing bne tone is taken on every iteration but the last (3
; cycles); and the next lda $C030 itself is 4 (absolute addressing). That is
; 2 + 994 + 2 + 3 + 4 = 1005 cycles from one toggle to the next.
;
; 200 toggles make 199 such gaps, a span of 199*1005 = 199995 cycles, so
; (200-1) * 1023000 / (2 * 199995) is about 509 Hz -- and 199995 cycles is
; close to 200 ms at the Apple II's 1.023 MHz clock.
        org $0300
        ldy #200        ; 200 toggles = 100 full square-wave cycles
tone:   lda $C030       ; speaker: any access toggles it
        ldx #199        ; the delay loop below is 994 cycles
delay:  dex
        bne delay
        dey
        bne tone
spin:   jmp spin
