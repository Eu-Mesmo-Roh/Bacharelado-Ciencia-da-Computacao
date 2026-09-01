    .section .rodata
printFormat:
    .string "Uma string a ser impressa.\n"

    .text
    .globl main
    .type main, @function

main:
    pushq %rbp
    movq %rsp, %rbp

    movq $printFormat, %rbx
    movq $4, (%rbx)

    movq $printFormat, %rdi
    movq $0, %rax
    call printf

    movl $0, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
