#include <stdio.h>
#include "cpu.h"

int main() {
  cpu_t cpu;        // NOTE: Intentionally uninitialized
  cpu.memory[0xFFFC] = 0x00;
  cpu.memory[0xFFFD] = 0x10;
  cpu_reset(&cpu);
  cpu_dump(&cpu);

  while (1) {
    cpu_opcode_e opcode = cpu_fetch(&cpu);
    switch (opcode) {
      case OP_LDA:
    }
  }
  
  printf("Hello World\n");
}
