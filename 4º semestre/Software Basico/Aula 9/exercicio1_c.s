    .section .rodata
printFormat:
    .string "uma string a ser impressa.\n"

    .section .bss
    .lcomm VETOR 27

    .text
    .globl main
    .type main, @function
main:
    pushq %rbp  
    movq %rsp, %rbp     # fim do prologo

    movq $0, %rcx       # passa 0 para rcx

loop_transcreve:
    movb printFormat(%rcx), %al    # lê o caractere atual
    cmpb $0, %al    # compara para saber se chegou ao fim
    je fim_loop # pula para o fim do loop

    cmpb $'a', %al # verifica se o caractere esta entre a e z
    jb salva_letra
    cmpb $'z', %al
    ja salva_letra

    subb $32, %al

salva_letra:
    movb %al,VETOR(%rcx)    # passa o caractere lido para o vetor em .bss
    incq %rcx   # soma 1 em rcx
    jmp loop_transcreve # volta para o loop_transcreve

fim_loop:
    movb $0,VETOR(%rcx) # coloca o \0 no vetor

    movq $VETOR, %rdi   # passa o endereço do vetor para rdi
    movl $0, %eax   # retira o float
    call printf # chama o printf

    movl $0, %eax   # epilogo
    movq %rbp, %rsp
    popq %rbp
    ret

