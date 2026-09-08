; Hand-known flag behaviour, for --dump-reg-liveness.
;
; Three routines, each chosen so that one column of the dump is decidable by
; reading this file rather than by trusting the analysis:
;
;   setcarry ($330)   returns its answer in the carry flag and in nothing else.
;                     The caller branches on C immediately after the JSR and
;                     both arms then overwrite A, N and Z, so C -- alone --
;                     must be its live-out, while A, N and Z are in `modified`.
;                     This is the "return with carry set on error" idiom the
;                     option exists to count.
;   clobbernz ($340)  writes A, X, N and Z on every path and nobody ever
;                     branches on the flags, so N and Z must be in `modified`
;                     and absent from its live-out.
;   usesy ($350)      indexes with Y without loading it, so Y must be in its
;                     live-in.
;
; Two baselines, at -O2 and -O3, because the two say different things. At -O2
; the sets above are visible as written. At -O3 the second dce() pass is itself
; driven by this liveness, so it deletes the stores nothing reads: clobbernz's
; `modified` collapses to empty and setcarry's to carry alone. That is the
; result being correct rather than the fixture being weak -- and it is worth
; pinning, because a liveness bug that made a dead flag look live would show up
; here as a store that failed to disappear.
;
; The program ends in a self-loop rather than a BRK, because BRK lowers to
; EncodeFlags and would read all six flags -- making clobbernz's dead N and Z
; live again, and hiding exactly what this fixture is here to show.

        org     $300

        jsr     setcarry
        bcs     bad
        lda     #$01
        sta     $1000
        jmp     next
bad:
        lda     #$02
        sta     $1000
next:
        jsr     clobbernz
        ldy     #4
        jsr     usesy
loop:
        jmp     loop

        org     $330
; Returns with C set when $1002 >= 5. A, N and Z are written on the way there
; and are all dead at the RTS.
setcarry:
        lda     $1002
        cmp     #5
        rts

        org     $340
; Writes A, X, N and Z. Reads no flag at all.
clobbernz:
        lda     $1003
        sta     $1004
        ldx     $1005
        stx     $1006
        rts

        org     $350
; Reads Y on entry and never writes it.
usesy:
        lda     $1100,y
        sta     $1101
        rts
