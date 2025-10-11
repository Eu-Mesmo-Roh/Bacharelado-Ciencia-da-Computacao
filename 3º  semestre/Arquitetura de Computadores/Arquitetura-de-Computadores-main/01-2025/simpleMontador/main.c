#include "assembler.h"

int main() {
    char line[MAX_LINE_LENGTH];
    
    printf("RISC-V Assembler (type 'exit' to quit)\n");
    printf("Supported instructions: add, sub, and, or, sll, addi, andi, ori, lw, sw, beq\n");
    
    while (1) {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "exit") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        uint32_t instruction = assemble_instruction(line);
        if (instruction != 0) {
            printf("0x%06x\n", instruction);
        }
    }
    
    return 0;
}