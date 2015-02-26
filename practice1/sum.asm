global main

extern printf
extern scanf

section .text

; e^[esp]
; returns (eax:edx) result
exp:
    fld     qword [esp + 4]           ; push the argument (esp is instruction address, so it’s esp+4) to the FPU stack
    fldl2e                            ; push log_2(e) to the FPU stack
    fmulp   st1                       ; multiply them
    fld     st0                       ; duplicate the top item
    frndint                           ; round it
    fsub    st1, st0                  ; st1 <- fractional part of (st1)
    fxch                              ; swap them
                                      ; st1 := integer part; st0 := fractional part
    f2xm1                             ; st0 := 2 ^ (fractional part) - 1
    fld1                              ; push 1 to the FPU stack
    faddp   st1                       ; st0 := 2 ^ (fractional part)
    fscale                            ; st0 *= 2 ^ (integer part); success!
    sub     esp, 8
    fstp    qword [esp]
    pop     eax
    pop     edx
    ; return value is stored in eax:edx
    ret


; int main()
; Read two doubles and print their sum.
main:
    ; print prompt
    push    fmtPrompt
    call    printf
    add     esp, 4

    ; read X
     push    dblX
     push    fmtIn
     call    scanf
     add     esp, 8

    ; compute exp

    sub     esp, 8
    fld     qword [dblX]
    fstp    qword [esp]
    call    exp
    add     esp, 8

    ; print the result

    push    edx
    push    eax
    push    fmtOut
    call    printf
    add     esp, 12

    ; return 0
    xor     eax, eax
    ret

section .data
fmtPrompt:  db 'Enter X: ', 0
fmtIn:      db '%lf', 0
fmtOut:     db 'e^x is %f', 10, 0

section .bss
dblX:       resq 1
dblY:       resq 1
