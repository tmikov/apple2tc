; Mixed HGR: an alternating-dot comb in the graphics area (rows 0-159) plus a
; filled text window (screen lines 20-23), then spin.
;
; The comb is what makes "color" (per-dot artifact approximation) and
; "color140"/"mono140" (true colour-clock resolution) disagree somewhere in
; the graphics area -- the same discriminator hgr-render.s's HCOLOR=1 bar
; relies on, reproduced here by hand since this fixture does not go through
; Applesoft.
;
; The text window is drawn by the same draw_glyph_cb() regardless of which
; a2_hgr_mode_t is selected, so rows 160-191 of a "color" render and of a
; "color140" render at scale:2 must be pixel-identical -- that identity is
; exactly what the widening bug in lib/a2mcp/mcp_screen.cpp broke (it used to
; stretch the already-full-width text to double width via `x / widen` applied
; to every row, not just the graphics ones).
        org $0300
        lda $C050       ; graphics
        lda $C053       ; mixed
        lda $C057       ; hi-res
        lda $C054       ; page 1

        ; Fill screen row 0 of HGR page 1 ($2000-$2027) with $55: dots
        ; 0,2,4,6 of every byte set, none adjacent -- an isolated-dot comb
        ; whose per-dot artifact colour (COLOR) and colour-clock cell colour
        ; (COLOR140/MONO140) are known to differ.
        ldx #0
gr_loop:
        lda #$55
        sta $2000,x
        inx
        cpx #40
        bne gr_loop

        ; Text window: screen lines 20-23 are (line % 8) * 128 + (line / 8) *
        ; 40 into the text page -- line 20 is offset 592 ($0400 + $250 =
        ; $0650), and each subsequent line is $80 further (128 = one region).
        ; $C1 is 'A' (0x41) with the high bit set, i.e. normal video.
        ldx #0
txt_loop:
        lda #$C1
        sta $0650,x     ; line 20
        sta $06D0,x     ; line 21
        sta $0750,x     ; line 22
        sta $07D0,x     ; line 23
        inx
        cpx #40
        bne txt_loop

spin:   jmp spin
