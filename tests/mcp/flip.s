; Flip PAGE2 four times with a delay between flips comfortably longer than one
; host frame (17050 cycles at 1.023 MHz -- see keys.jsonl's per-key spacing in
; docs/a2mcp.md for that number), then spin. Exercises `run`'s "screen_update"
; flip detector: each LOWSCR/HISCR access that actually changes A2_VC_PAGE2 is
; one page flip, i.e. one "update".
;
; Stays in text mode throughout: PAGE2 selects between $0400 and $0800. The
; two pages are given different byte 0 up front, once, and never written
; again -- every later flip therefore also changes a2host_visible_hash(),
; which is what makes this fixture able to notice a bug that counts a flip
; and the hash change it causes as two separate updates instead of one: with
; identical pages a flip would not touch the hash at all, and that bug would
; go unnoticed.
;
; The delay loop is ~24000 cycles (24 outer passes of a 199-count inner
; loop -- see beep.s for the exact NMOS timing an inner pass like this
; costs). That is deliberately more than one frame (17050 cycles) but less
; than two: enough that consecutive flips always land in distinct frames, but
; short enough that the settle detector's K=2-consecutive-stable-frames
; window never has time to close between one flip and the next -- otherwise
; the idle gap during the delay loop would itself register as a spurious
; settle, double-counting one flip as two updates.
        org $0300
        lda $C051       ; TEXT on
        lda $C054       ; page 1 (page2 off), a known starting state
        lda #1
        sta $0400       ; page 1's byte 0
        lda #2
        sta $0800       ; page 2's byte 0 -- different, so a flip changes the hash
        jsr delay
        lda $C055       ; page 2 on: flip #1
        jsr delay
        lda $C054       ; page 1 on: flip #2
        jsr delay
        lda $C055       ; page 2 on: flip #3
        jsr delay
        lda $C054       ; page 1 on: flip #4
spin:   jmp spin

delay:  ldy #24
oloop:  ldx #199
iloop:  dex
        bne iloop
        dey
        bne oloop
        rts
