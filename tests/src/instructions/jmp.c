#include "tests.h"
#include "6502emu/cpu.h"
#include <stdio.h>

MunitResult jmp_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x4C;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "JMP $4000");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, 0x4000);

  return MUNIT_OK; 
}

MunitResult jmp_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x6C;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "JMP $4000");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, 0x4000);

  return MUNIT_OK;
}
