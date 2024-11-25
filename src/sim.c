#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "global.h"
#include "interpreter.h"
#include "loader.h"
#include "memory.h"

uint8_t *memory;                      // Pointer to flat memory
uint32_t pc;                          // The PC
uint32_t registers[32];               // The MIPS register file (32 registers)
uint32_t hi, lo;                      // HI and LO registers for multiplication/division

/* Print usage instructions */
void usage()
{
   printf("Usage: sim FILE\n");
}

int main(int argc, const char *argv[])
{
   FILE *fin;

   /* Allocate a lot of memory to implement the MIPS memory in a flat model */
   memory = (uint8_t *)malloc(MEMSIZE * sizeof(uint8_t));

   /* Check command line parameters */
   if (argc != 2) {
      usage();
      fprintf(stderr, "Error: no input file provided.\n");
      exit(EXIT_FAILURE);
   }

   /* Open input file */
   fin = fopen(argv[1], "rb");
   if (!fin) {
      usage();
      fprintf(stderr, "Error: \"%s\" file not found.\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   /* Read text and data sections from input file */
   if (!read_file(fin)) {
      fprintf(stderr, "Error: Cannot read input file.\n");
      exit(EXIT_FAILURE);
   }

   /* Close input file */
   fclose(fin);

   /* Set up stack pointer and program counter */
   pc = 0x00400000;
   registers[29] = 0x7FFFEFFC; // Initialize stack pointer (sp) to 0x7FFFEFFC

   /* Run program */
   run(pc);

   printf("\n");
   return (EXIT_SUCCESS);
}
