section .data
    fmt db "%d", 10, 0
section .bss
    x resq 1
    y resq 1
section .text
    global main
    extern printf
main:
    push rbp
    mov qword [x], 5
    mov rax, [x]
    cmp rax, 2
    jne .L_ELSE_0
    mov rdi, fmt
    mov rsi, [x]
    xor rax, rax
    call printf
    jmp .L_END_0
.L_ELSE_0:
    mov qword [y], 10
    mov rdi, fmt
    mov rsi, [y]
    xor rax, rax
    call printf
.L_END_0:
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall

section .note.GNU-stack noalloc noexec nowait progbits
