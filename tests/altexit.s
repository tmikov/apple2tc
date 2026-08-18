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
; The caller is itself a routine rather than the top-level code, and that is
; load-bearing twice over. `outer` only becomes a candidate at all if its block
; set picks up unwound1 and unwound2 -- they are continuations of the call, and
; nothing else reaches them -- which is the rescan in run(). And extracting
; `outer` means cloning its CallAlt, which is the first instruction in this IR
; whose operand list is longer than its declared arity. A release build used to
; drop those operands silently; the alternate exits simply vanished.
;
; Two baselines, as with codeat.s and inlinestr.s: the diff between them is
; what the option does.

        org     $300
start:
        jsr     outer
end:
        jmp     end

        org     $310
outer:
        lda     $1000
        jsr     sub
; The ordinary return lands here.
        lda     #$01
        sta     $1002
        rts
unwound1:
        lda     #$02
        sta     $1002
        rts
unwound2:
        lda     #$03
        sta     $1002
        rts

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
