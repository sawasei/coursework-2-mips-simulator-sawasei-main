#include "interpreter.h"
#include "global.h"
#include "memory.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> /* for isdigit */

/* read and return a non-negative integer from stdin */
int readInt(void)
{
    int x = 0; /* return value accumulator */
    int c;

    while ((c = getchar()) != EOF)
    {
        if (isdigit(c))
        {
            /* assumes '0'..'9' are contiguous */
            x = x * 10 + (c - '0');
        }
        else // if not a digit
        {
            /* put it back */
            ungetc(c, stdin);

            /* escapes from innermost loop */
            break;
        }
    }

    /* Flush stdin */
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    return x;
}

/* Convenience function to fetch instruction from address in PC */
uint32_t fetch(uint32_t pc)
{
    return read_word(pc);
}

/* This is the main interpreter function, which decodes and executes MIPS instructions */
void run(uint32_t pc)
{
    /* Fetch - decode - execute loop */
    bool terminate = false;
    do
    {
        /* Fetch instruction */
        uint32_t instr = fetch(pc);

#ifdef DEBUG
        fprintf(stderr, "[0x%08X]        0x%08X    ", pc, instr);
#endif

        /* Decode and execute instruction */
        if (instr == 0)
        {
#ifdef DEBUG
            fprintf(stderr, "nop\n");
#endif
        }
        else
        {
            uint32_t opcode = instr >> 26;

            switch (opcode)
            {
            case 0: /* R - format: RS, RT, RD, SHAMT, FUNC */
            {
                uint32_t rs = (instr & 0x03E00000) >> 21;
                uint32_t rt = (instr & 0x001F0000) >> 16;
                uint32_t rd = (instr & 0x0000F800) >> 11;
                uint32_t shamt = (instr & 0x000007C0) >> 6;
                uint32_t func = instr & 0x0000003F;

#ifdef DEBUG
                fprintf(stderr, "Opcode: %u Rs: %u Rt: %u Rd: %u Shift: %u Function: %u\n", opcode, rs, rt, rd, shamt, func);
#endif

                switch (func)
                {
                case 32: // ADD
                    registers[rd] = registers[rs] + registers[rt];
#ifdef DEBUG
                    fprintf(stderr, "add $%u, $%u, $%u\n", rd, rs, rt);
#endif
                    break;
                case 34: // SUB
                    registers[rd] = registers[rs] - registers[rt];
#ifdef DEBUG
                    fprintf(stderr, "sub $%u, $%u, $%u\n", rd, rs, rt);
#endif
                    break;
                case 12: // SYSCALL
                {
#ifdef DEBUG
                    fprintf(stderr, "syscall\n");
#endif
                    switch (registers[2]) // v0
                    {
                    case 1: // print_int
                        printf("%d", registers[4]); // a0
                        break;
                    case 4: // print_string
                    {
                        const char *s = (const char *)&(memory[registers[4]]); // a0
                        printf("%s", s);
                        break;
                    }
                    case 5: // read_int
                    {
                        int32_t input = readInt();
                        registers[2] = input; // v0
                        break;
                    }
                    case 10: // exit
                        terminate = true;
                        break;
                    case 11: // print_character
                        printf("%c", (char)registers[4]); // a0
                        break;
                    default:
                        fprintf(stderr, "Unimplemented syscall. Terminate.\n");
                        exit(1);
                    }
                    break;
                }
                default:
                    fprintf(stderr, "Unimplemented R-type function. Terminate.\n");
                    exit(1);
                }
                break;
            }

            case 35: // LW (I-type)
            {
                uint32_t base = (instr & 0x03E00000) >> 21;
                uint32_t rt = (instr & 0x001F0000) >> 16;
                int16_t offset = instr & 0x0000FFFF;
                registers[rt] = read_word(registers[base] + offset);
#ifdef DEBUG
                fprintf(stderr, "lw $%u, %d($%u)\n", rt, offset, base);
#endif
                break;
            }
            case 43: // SW (I-type)
            {
                uint32_t base = (instr & 0x03E00000) >> 21;
                uint32_t rt = (instr & 0x001F0000) >> 16;
                int16_t offset = instr & 0x0000FFFF;
                write_word(registers[base] + offset, registers[rt]);
#ifdef DEBUG
                fprintf(stderr, "sw $%u, %d($%u)\n", rt, offset, base);
#endif
                break;
            }
            default:
                // Bail out if unrecognized instruction is encountered
                fprintf(stderr, "Unimplemented instruction. Terminate.\n");
                exit(1);
            }
        }

        /* Increment pc */
        pc += 4;

    } while (!terminate);

    /* Dump register file */
    for (int i = 0; i < 32; i++)
    {
        printf("$%d : 0x%08X\n", i, registers[i]);
    }
}
