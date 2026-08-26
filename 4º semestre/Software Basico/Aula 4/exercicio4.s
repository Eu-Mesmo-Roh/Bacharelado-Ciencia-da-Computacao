    .text
    .globl main
    .type main, @function

main:
    # Sem usar o rbp não precisamos de prólogo
    subq $16, %rsp      # subtrai 16 bytes da memoria para rsp
    movl $1, 12(%rsp)   # adiciona o valor 1 no fim dos primeiros 4 bits
    movl $0, 8(%rsp)    # adiciona o valor 0 no fim dos da proxima variavel (proximos 4 bits)
    movl $27, %eax       # retorna 0
    addq $16, %rsp      # "Esvazia a pilha"
    ret                 # retorna a função que o chamou.
