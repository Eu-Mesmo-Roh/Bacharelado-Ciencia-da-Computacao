# Constantes
    .equ STDIN, 0
    .equ STDOUT, 1

# Posições na stack
    .equ local_size, 16 # como se fosse um #define em C
    .equ vetor, -16

    .text
    .global main
    .type main, @function
main:
    pushq %rbp # salva o frame pointer na pilha
    movq %rsp, %rbp # copiar o stack pointer para o frame pointer
    subq $local_size, %rsp # Selecionado o começo da pilha


    movl $2, %edx # Selecionando os dois bytes para a excrita
    leaq vetor(%rbp), %rsi  # carregando o endereço do vetor.
    movl $STDIN, %edi
    call read
    
    movl $2, %edx
    leaq vetor(%rbp), %rsi
    movl $STDOUT, %edi
    call write




    movl $0, %eax # o valor retornado deve estar em EAX (return 0)
    movq %rbp, %rsp # voltar a pilha para a posição original
    popq %rbp
    ret
