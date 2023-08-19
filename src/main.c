#include <stdio.h>
#include "6502emu/cpu.h"
#include "6502emu/args.h"

int main(int argc, char** argv) {
  cpu_t cpu;        // NOTE: Intentionally uninitialized

  args_t args = args_parse(argc, argv);
  if (!args.input_file) {
    fprintf(stderr, "Must supply file\n");
    return 0;
  }
  if (!cpu_load_program(args.input_file, &cpu)) {
    fprintf(stderr, "File doesn't exist\n");
    return 0;
  }
  /*
  // Setup reset vector
  cpu.memory[0xFFFC] = 0x00;
  cpu.memory[0xFFFD] = 0x10;

  // lda #45
  cpu.memory[0x1000] = OP_LDA_IMM;
  cpu.memory[0x1001] = 0x49;

  // sta $45 to $00
  cpu.memory[0x1002] = OP_STA_ZP;
  cpu.memory[0x1003] = 0x00;

  // lda $00
  cpu.memory[0x1004] = OP_LDA_ZP;
  cpu.memory[0x1005] = 0x00;
  */

  cpu_reset(&cpu);

  do {
    cpu_get_str_rep(cpu.pc, &cpu);
    cpu_dump(&cpu);
    printf("Press any button to step forward\n");
    fflush(stdin);
    getchar();
  } while (cpu_step(&cpu));
}
