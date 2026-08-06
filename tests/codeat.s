; Targets of dynamic control transfers the tracer cannot follow.
;
; Neither continuation below is reachable statically, and there is no runtime
; data, so without --code-at the disassembler stops at $306 and everything from
; $310 on is invisible. codeat.txt asserts the two edges that connect them.
;
; Baselines: codeat.ir (with the edges) and codeat-noedges.ir (without), so the
; test pins down what the option actually changes rather than just that it runs.

        org     $300

; A computed RTS. This is the core of the Apple II "inline string after JSR"
; idiom, where the printer pushes the address of the string terminator and
; returns past it -- the return address is data, so it cannot be resolved
; statically.
        lda     #>via_rts-1
        pha
        lda     #<via_rts-1
        pha
rts_site:
        rts

; An indirect jump through a pointer the tracer never evaluates.
        org     $310
        lda     #<via_jmp
        sta     $10
        lda     #>via_jmp
        sta     $11
jmp_site:
        jmp     ($0010)

        org     $320
via_rts:
        lda     #$11
        sta     $1000
        jmp     $310

        org     $330
via_jmp:
        lda     #$22
        sta     $1001
        brk     #0
