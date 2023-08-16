#include <stdio.h>
#include "cpu.h"

int main(int argc, const char** argv) {
  cpu_t cpu;        // NOTE: Intentionally uninitialized
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

  cpu_reset(&cpu);

  do {
    cpu_dump(&cpu);
    printf("Press any button to step forward\n");
    fflush(stdin);
    getchar();
  } while (cpu_step(&cpu));
}
