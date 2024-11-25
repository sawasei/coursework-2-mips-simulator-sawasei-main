#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>

extern uint32_t pc;                    // The PC
extern uint32_t registers[32];         // MIPS register file (32 registers)
extern uint32_t hi, lo;                // HI and LO registers for multiplication/division

#endif // GLOBAL_H_
1