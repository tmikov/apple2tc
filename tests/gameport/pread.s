; Call the ROM's PREAD once per paddle and store the four counts.
;
; PREAD takes the paddle index in X, returns the count in Y, and leaves X
; alone -- it reads $C064,X -- so X doubles as the store index.
;
; One `org`, and the capture address is counted rather than forced with a
; second one: a forward org leaves a gap in the emitted image, and the
; acceptance script would then depend on how a6502 fills it. The bytes are
;
;   $300 ldx #0      2      $306 sta $1F00,x  3      $30C bne loop  2
;   $302 jsr $FB1E   3      $309 inx          1      $30E again     3
;   $305 tya         1      $30A cpx #4       2
;
; so `again` is at $30E, which is what --snapshot-at= names. The snapshot
; fires on the FIRST time the address is reached (probe.c's s_snap_fired), so
; the capture point must be one the program reaches only after all four
; stores -- which is why it is the halt and not the entry.

        org     $300

        ldx     #0
loop:   jsr     $FB1E
        tya
        sta     $1F00,x
        inx
        cpx     #4
        bne     loop
again:  jmp     again
