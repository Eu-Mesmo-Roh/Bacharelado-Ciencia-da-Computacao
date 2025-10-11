#Código da oper	Registrador a 	Registrador b 	Registrador c 	Constante

#assembly code Fibonacci
# N = r1
# first = r2
# next = r3
# second = r4
# i = r5
# r6 = branch

# r0 = 0000
# r1 = 0001
# r2 = 0010
# r3 = 0011
# r4 = 0100
# r5 = 0101
# r6 = 0110
# r7 = 0111
# r8 = 1000
# r9 = 1001

# r1 = N
# r2 = first
# r3 = next 
# r4 = second
# r5 = i
# r6 = i_aux
# r7 = const 1
    
addi r1 r0 10 #Defina N como o número desejado de termos da sequência Fibonacci
addi r2 r0 0  #Inicializa first
addi r3 r0 0  #Inicializa next
addi r4 r0 1  #Inicializa second
addi r6 r0 0  #inicializa i
addi r7 r0 1  #inicializa i_aux
addi r8 r0 1  #inicializa const 1

branch r1 r5   #se r1 for igual a r5 pula
add r3 r2 r4   # next = first + second;
add r2 r0 r4   # first = second;
add r4 r0 r3   #second = next;
add r7 r5 r7   #i = i + 1;
add r5 r7 r5   #i_aux = i + 1;
jump r0 r0 -6  #linhas


