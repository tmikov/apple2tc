; Stay in text mode and scribble on the text page, which text mode displays.
; a2host_visible_hash() must see this immediately.
;
; One half of a deliberate pair with hidden-page.s: same mode, same loop,
; different target page. That page is the only thing that may differ between
; the two files -- it is what isolates mode-awareness as the thing under test.
;
; The two stores are 0x80 apart, not 0x100: the overlap keeps the page
; changing every frame instead of settling into a fixed pattern after the
; first full pass (see hidden-page.s's comment for why that matters), which
; is also what makes screen_change fire promptly and reliably here.
        org $0300
        lda $C051       ; TEXT on
        lda $C054       ; page 1
        ldx #0
loop:   inx
        txa
        sta $0400,x     ; text page 1, displayed in text mode
        sta $0480,x
        jmp loop
