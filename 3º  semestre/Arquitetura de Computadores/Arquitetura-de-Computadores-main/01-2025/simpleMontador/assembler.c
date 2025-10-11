#include "assembler.h"

int register_number(char *reg)
{
    if (reg[0] == 'x')
    {
        return atoi(reg + 1);
    }
    return -1;
}

int is_valid_register(int reg)
{
    return reg >= 0 && reg <= 31;
}

int is_valid_immediate(char *imm_str, int *value)
{
    char *endptr;
    *value = strtol(imm_str, &endptr, 0);
    if (*endptr != '\0')
    {
        return 0;
    }
    return 1;
}

int parse_instruction(char *line, char *tokens[], int *token_count)
{
    char *token;
    int count = 0;

    // Remove newline character if present
    line[strcspn(line, "\n")] = '\0';

    // First token (instruction name)
    token = strtok(line, " ,\t");
    if (token == NULL)
        return 0;
    tokens[count++] = token;

    // Subsequent tokens
    while ((token = strtok(NULL, " ,\t")) != NULL && count < MAX_TOKENS)
    {
        tokens[count++] = token;
    }

    *token_count = count;
    return 1;
}

uint32_t encode_r_type(uint8_t opcode, uint8_t funct3, uint8_t funct7, uint8_t rd, uint8_t rs1, uint8_t rs2)
{
    return (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t encode_i_type(uint8_t opcode, uint8_t funct3, uint8_t rd, uint8_t rs1, int16_t imm)
{
    return ((imm & 0xFFF) << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t encode_s_type(uint8_t opcode, uint8_t funct3, uint8_t rs1, uint8_t rs2, int16_t imm)
{
    uint32_t imm_11_5 = (imm >> 5) & 0x7F;
    uint32_t imm_4_0 = imm & 0x1F;
    return (imm_11_5 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm_4_0 << 7) | opcode;
}

uint32_t encode_b_type(uint8_t opcode, uint8_t funct3, uint8_t rs1, uint8_t rs2, int16_t imm)
{
    uint32_t imm_12 = (imm >> 12) & 0x1;
    uint32_t imm_10_5 = (imm >> 5) & 0x3F;
    uint32_t imm_4_1 = (imm >> 1) & 0xF;
    uint32_t imm_11 = (imm >> 11) & 0x1;

    return (imm_12 << 31) | (imm_10_5 << 25) | (rs2 << 20) | (rs1 << 15) |
           (funct3 << 12) | (imm_4_1 << 8) | (imm_11 << 7) | opcode;
}

uint32_t assemble_instruction(char *instruction)
{
    char *tokens[MAX_TOKENS];
    int token_count;

    if (!parse_instruction(instruction, tokens, &token_count))
    {
        fprintf(stderr, "Error parsing instruction\n");
        return 0;
    }

    char *op = tokens[0];

    // R-type instructions
    if (strcmp(op, "add") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: add rd, rs1, rs2\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs1 = register_number(tokens[2]);
        int rs2 = register_number(tokens[3]);

        if (!is_valid_register(rd) || !is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_r_type(0x33, 0x0, 0x00, rd, rs1, rs2);
    }
    else if (strcmp(op, "sub") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: sub rd, rs1, rs2\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs1 = register_number(tokens[2]);
        int rs2 = register_number(tokens[3]);

        if (!is_valid_register(rd) || !is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_r_type(0x33, 0x0, 0x20, rd, rs1, rs2);
    }
    else if (strcmp(op, "and") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: and rd, rs1, rs2\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs1 = register_number(tokens[2]);
        int rs2 = register_number(tokens[3]);

        if (!is_valid_register(rd) || !is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_r_type(0x33, 0x7, 0x00, rd, rs1, rs2);
    }
    else if (strcmp(op, "or") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: or rd, rs1, rs2\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs1 = register_number(tokens[2]);
        int rs2 = register_number(tokens[3]);

        if (!is_valid_register(rd) || !is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_r_type(0x33, 0x6, 0x00, rd, rs1, rs2);
    }
    else if (strcmp(op, "sll") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: sll rd, rs1, rs2\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs1 = register_number(tokens[2]);
        int rs2 = register_number(tokens[3]);

        if (!is_valid_register(rd) || !is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_r_type(0x33, 0x1, 0x00, rd, rs1, rs2);
    }
    // I-type instructions
    else if (strcmp(op, "addi") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: addi rd, rs, imm\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs = register_number(tokens[2]);
        int imm;

        if (!is_valid_register(rd) || !is_valid_register(rs))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        if (!is_valid_immediate(tokens[3], &imm))
        {
            fprintf(stderr, "Invalid immediate value\n");
            return 0;
        }

        return encode_i_type(0x13, 0x0, rd, rs, imm);
    }
    else if (strcmp(op, "andi") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: andi rd, rs, imm\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs = register_number(tokens[2]);
        int imm;

        if (!is_valid_register(rd) || !is_valid_register(rs))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        if (!is_valid_immediate(tokens[3], &imm))
        {
            fprintf(stderr, "Invalid immediate value\n");
            return 0;
        }

        return encode_i_type(0x13, 0x7, rd, rs, imm);
    }
    else if (strcmp(op, "ori") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: ori rd, rs, imm\n");
            return 0;
        }

        int rd = register_number(tokens[1]);
        int rs = register_number(tokens[2]);
        int imm;

        if (!is_valid_register(rd) || !is_valid_register(rs))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        if (!is_valid_immediate(tokens[3], &imm))
        {
            fprintf(stderr, "Invalid immediate value\n");
            return 0;
        }

        return encode_i_type(0x13, 0x6, rd, rs, imm);
    }
    else if (strcmp(op, "lw") == 0)
    {
        if (token_count != 3)
        {
            fprintf(stderr, "Syntax error: lw rd, offset(rs)\n");
            return 0;
        }

        int rd = register_number(tokens[1]);

        // Parse offset(rs)
        char *ptr = strchr(tokens[2], '(');
        if (ptr == NULL)
        {
            fprintf(stderr, "Syntax error: expected offset(rs)\n");
            return 0;
        }

        int offset = atoi(tokens[2]);
        char *rs_str = ptr + 1;
        rs_str[strcspn(rs_str, ")")] = '\0';
        int rs = register_number(rs_str);

        if (!is_valid_register(rd) || !is_valid_register(rs))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_i_type(0x03, 0x2, rd, rs, offset);
    }
    // S-type instructions
    else if (strcmp(op, "sw") == 0)
    {
        if (token_count != 3)
        {
            fprintf(stderr, "Syntax error: sw rs2, offset(rs1)\n");
            return 0;
        }

        int rs2 = register_number(tokens[1]);

        // Parse offset(rs1)
        char *ptr = strchr(tokens[2], '(');
        if (ptr == NULL)
        {
            fprintf(stderr, "Syntax error: expected offset(rs1)\n");
            return 0;
        }

        int offset = atoi(tokens[2]);
        char *rs1_str = ptr + 1;
        rs1_str[strcspn(rs1_str, ")")] = '\0';
        int rs1 = register_number(rs1_str);

        if (!is_valid_register(rs2) || !is_valid_register(rs1))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        return encode_s_type(0x23, 0x2, rs1, rs2, offset);
    }
    // B-type instructions
    else if (strcmp(op, "beq") == 0)
    {
        if (token_count != 4)
        {
            fprintf(stderr, "Syntax error: beq rs1, rs2, offset\n");
            return 0;
        }

        int rs1 = register_number(tokens[1]);
        int rs2 = register_number(tokens[2]);
        int offset;

        if (!is_valid_register(rs1) || !is_valid_register(rs2))
        {
            fprintf(stderr, "Invalid register number\n");
            return 0;
        }

        if (!is_valid_immediate(tokens[3], &offset))
        {
            fprintf(stderr, "Invalid offset value\n");
            return 0;
        }

        return encode_b_type(0x63, 0x0, rs1, rs2, offset);
    }
    else
    {
        fprintf(stderr, "Unknown instruction: %s\n", op);
        return 0;
    }
}