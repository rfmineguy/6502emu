#include "tests.h"
#include "6502emu/cpu.h"

MunitResult nop  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0xEA;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "NOP");

  cpu_execute(&cpu, ins);
  return MUNIT_OK; 
}
