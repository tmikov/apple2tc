        org     $300

main:
        ldx     #10
        jsr     func2
        jsr     func1
        jmp     main

func1:
        inx
func2:
        inx
        rts
