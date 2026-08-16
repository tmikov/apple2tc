; The Apple II "inline string after JSR" idiom, for --inline-str.
;
; print_str takes its argument as the NUL-terminated bytes placed after the
; JSR that calls it: it pops its own return address, walks them, and then
; pushes the address of the terminator so its RTS lands past them.
;
; Two baselines, so the test pins down what the option changes rather than
; merely that it runs. Without --inline-str the tracer follows the JSR's
; fall-through into the text -- $C8 $C9 $00 decodes as INY / CMP #$00, which
; is the failure this option exists to prevent and is *not* an error the
; disassembler can notice.

        org     $300
start:
        jsr     print_str
        dfb     $C8,$C9,$00
        lda     #$01
        sta     $1000
        jsr     print_str
        dfb     $C1,$00
end:
        jmp     end

; A stand-in for COUT, so the fixture pulls in no ROM.
        org     $318
cout:
        rts

        org     $320
print_str:
        pla
        sta     $0C
        pla
        sta     $0D
loop:
        inc     $0C
        bne     rd
        inc     $0D
rd:
        ldy     #$00
        lda     ($0C),y
        beq     done
        jsr     cout
        jmp     loop
done:
        lda     $0D
        pha
        lda     $0C
        pha
        rts
