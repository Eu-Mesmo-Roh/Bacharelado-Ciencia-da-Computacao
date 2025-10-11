# Montador MIPS Simples

Um montador simples para um subconjunto de instruções MIPS implementado em C.

## Instruções suportadas

### R-type:
- `add rd, rs, rt`
- `sub rd, rs, rt`
- `and rd, rs, rt`
- `or rd, rs, rt`
- `sll rd, rt, shamt`

### I-type:
- `addi rt, rs, imm`
- `andi rt, rs, imm`
- `ori rt, rs, imm`
- `lw rt, offset(rs)`
- `sw rt, offset(rs)`
- `beq rs, rt, offset`

## Como compilar e executar

```bash
make       # Compila o programa
make run   # Executa o programa
make clean # Limpa os arquivos gerados