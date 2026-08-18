; The alternate exit that must be refused: one that lands back inside the
; routine it is leaving.
;
; `inner` is reached both by falling out of the compare and by the PLA/PLA/JMP,
; so the jump is not "return into the caller" at all. Accepting it would put
; the target -- and everything reachable from it -- inside the routine, which
; for a real game means the caller's main loop, and a routine that calls
; itself. That is not hypothetical: it is what an earlier attempt at this
; feature produced, and it passed every behavioural oracle we had because the
; unwind never executed in any recording.

        org     $300
start:
        lda     $1000
        jsr     sub
        jsr     sub2
        jsr     sub3
        jsr     outer4
        lda     $1009
        beq     jumpin
end:
        jmp     end
out2:
        sta     $1006
        jmp     end
out3:
        lda     #$09
        sta     $100A
        jmp     end
jumpin:
        jmp     sub3

        org     $330
sub:
        lda     $1001
        beq     alt
inner:
        lda     #$AA
        sta     $1003
        rts
alt:
        pla
        pla
        jmp     inner

; The other shape that must be refused: work between discarding the return
; address and leaving. There is nowhere to put it -- the extracted routine has
; already returned by then, and the caller has not yet resumed -- so the
; routine stays inlined rather than being cut in half.
        org     $350
sub2:
        lda     $1004
        beq     alt2
        rts
alt2:
        pla
        pla
        lda     #$07
        sta     $1005
        jmp     out2

; And the entry restriction: a routine with alternate exits that is also
; jumped into. Arriving any way other than by a JSR is converted by building a
; block that calls the routine and then dispatches on the return address, and
; that block has no way to take an alternate exit -- the exit would be silently
; lost. Until those paths can express it, such a routine stays inlined.
        org     $370
sub3:
        lda     $1008
        beq     alt3
        rts
alt3:
        pla
        pla
        jmp     out3

; The caller's side of the same problem. `outer4` reaches `unwound4` only by
; calling sub4 and taking its alternate exit, so unwound4 is part of outer4 --
; and it reloads the stack pointer, which no routine may do. Only the
; rescan in run() walks it: on the first pass sub4's exits are not known yet,
; so outer4 looks clean. Without the rescan outer4 is accepted here and then
; extractRoutine() pulls unwound4 in anyway, following the CallAlt -- a block
; that entered a routine without ever being checked.
        org     $390
outer4:
        lda     $100B
        jsr     sub4
        rts
unwound4:
        lda     #$00
        ldx     #$FF
        txs
        rts

        org     $3A0
sub4:
        lda     $100C
        beq     alt4
        rts
alt4:
        pla
        pla
        jmp     unwound4
