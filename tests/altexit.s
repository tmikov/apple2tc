; A routine with alternate exits, for --alt-exit.
;
; `sub` returns three different ways. Two of them discard the return address
; and jump straight into the caller -- the 6502 way of saying "return, but not
; to where you came from". Without the option the whole routine is rejected
; ("stack level underflow") and stays inlined in the caller's switch; with it,
; sub becomes a real C function returning which exit it took, and the caller
; branches on that.
;
; Two exits rather than one, because the point of the transform is that the
; number of exits is not fixed. A one-exit routine could be a bool.
;
; Two baselines, as with codeat.s and inlinestr.s: the diff between them is
; what the option does.

        org     $300
start:
        lda     $1000
        jsr     sub
; The ordinary return lands here.
        lda     #$01
        sta     $1002
        jmp     end
unwound1:
        lda     #$02
        sta     $1002
        jmp     end
unwound2:
        lda     #$03
        sta     $1002
end:
        jmp     end

        org     $330
sub:
        lda     $1001
        beq     alt1
        cmp     #$05
        beq     alt2
        lda     #$AA
        sta     $1003
        rts
alt1:
        pla
        pla
        jmp     unwound1
alt2:
        pla
        pla
        jmp     unwound2
