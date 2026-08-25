#Posições na STACK
    .equ ptrOla, -15 #Local do ponteiro para STR_OLA na pilha
    .equ localSize, -16

    .section .rodata

printFormat:
    .string "Um Inteiro %i\n"

    .text
    .globl main
    .type main, @function

scanFormat:
    .string "Um Inteiro %i\n"

    .text
    .globl main
    .type main, @function

main:







    movl $0, %eax   #return 0
    movq %rbp, %rsp
    popq %rbp   #fim do epílogo
    ret     #retorna ao chamador