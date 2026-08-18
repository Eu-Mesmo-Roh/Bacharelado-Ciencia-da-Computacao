    .text
    .global main
    .type main, @function
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
