#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define MAX_TOKENS 10
#define MAX_LABELS 100

typedef struct {
    char name[20];
    int address;
} Label;

uint32_t assemble_instruction(char *instruction);
int parse_instruction(char *line, char *tokens[], int *token_count);
uint32_t encode_r_type(uint8_t opcode, uint8_t funct3, uint8_t funct7, uint8_t rd, uint8_t rs1, uint8_t rs2);
uint32_t encode_i_type(uint8_t opcode, uint8_t funct3, uint8_t rd, uint8_t rs1, int16_t imm);
uint32_t encode_s_type(uint8_t opcode, uint8_t funct3, uint8_t rs1, uint8_t rs2, int16_t imm);
uint32_t encode_b_type(uint8_t opcode, uint8_t funct3, uint8_t rs1, uint8_t rs2, int16_t imm);
int register_number(char *reg);
int is_valid_register(int reg);
int is_valid_immediate(char *imm_str, int *value);

#endif