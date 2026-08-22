; Return edges that nothing can produce any more, for --prune-returns.
;
; `sub` calls `helper` and resumes at `inner`. Externalizing `helper` erases
; that JSR -- and with it the only instruction that ever pushed `inner`'s
; address. But the recording saw `other`'s RTS return to `inner` as well, so
; `inner` is still a dynamic successor of a routine that is *not* external, and
; that one edge keeps the whole of `sub` alive through externalization.
;
; That is not a contrived shape. Snake Byte's $7230 survives for exactly this
; reason: $7239 is where its own `JSR $FC68` came back to, $FC68 is external,
; and a ROM RTS still lists $7239 among its returns.

        org     $300
start:
        jsr     sub
        jsr     other
end:
        jmp     end

        org     $320
sub:
        lda     #$01
        jsr     helper
inner:
        sta     $1000
        rts

        org     $340
helper:
        rts

        org     $350
other:
        nop
        rts
