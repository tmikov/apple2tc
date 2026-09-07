; Stay in text mode and scribble on hi-res page 1, which is not displayed.
; a2host_visible_hash() must not see it -- see the comment on hash_video_state()
; in lib/a2host/a2host.c for why the frame-hash oracle deliberately does.
;
; One half of a deliberate pair with visible-page.s: same mode, same loop,
; different target page. That page is the only thing that may differ between
; the two files -- it is what isolates mode-awareness as the thing under test.
;
; The two stores are 0x80 apart, not 0x100: the resulting overlap between them
; means the byte at a given address depends on which store last landed on it,
; which shifts by one iteration's phase every frame. Address and stored value
; never lock into an identity mapping, so the page keeps changing forever
; instead of settling into a fixed pattern after the first full pass -- a
; hidden page that stops changing would let a mode-blind hash pass this test
; by accident, since it would then have nothing left to notice either.
        org $0300
        lda $C051       ; TEXT on
        lda $C054       ; page 1
        ldx #0
loop:   inx
        txa
        sta $2000,x     ; hi-res page 1, undisplayed in text mode
        sta $2080,x
        jmp loop
