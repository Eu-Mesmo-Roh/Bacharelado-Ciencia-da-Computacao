#Constantes
    .equ STDOUT, 1

#Posições na STACK
    .equ ptrOla, -8 #Local do ponteiro para STR_OLA na pilha
    .equ localSize, -16

    .section .rodata
STR_OLA:
    .string "Ola\n"

    .text
    .globl main
    .type main,@function

main:
    pushq %rbp
    movq %rsp,%rbp
    addq $localSize,%rsp #final do prólogo
    movq $STR_OLA,%rsi #carregar o end. base de STR_OLA para rsi
    movq %rsi, ptrOla(%rbp) #armazenar o endereço de STR_OLA na pilha
while:
    movq ptrOla(%rbp),%rsi #carregar a posição atual para rsi
    cmpb $0,(%rsi) #o valor na posição de memória é \0?
    je fimLoop #salta se igual. Checa flag Z em rflags
    movl $1,%edx #imprimir 1 caractere
    movq $STDOUT, %rdi #id. arquivo STDOUT em rdi
    call write #chama função do C
    incl ptrOla(%rbp) #somar um no endereço armazenado na pilha
    jmp while #salto incondicional
fimLoop: #label de saída
    movl $0,%eax #return 0
    movq %rbp, %rsp
    popq %rbp #fim do epílogo
    ret #retornar ao chamador
