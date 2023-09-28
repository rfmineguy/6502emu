#include "tests.h"
#include "6502emu/cpu.h"

MunitResult sty_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x84;
  cpu.memory[0x1] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STY $40");

  cpu.regY = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x40], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sty_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x94;
  cpu.memory[0x1] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STY $40, X");

  cpu.regY = 0x45;
  cpu.regX = 1;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x41], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sty_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x8C;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;
  cpu.memory[0x4000] = 0x10;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STY $4000");

  cpu.regY = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x4000], ==, 0x45);

  return MUNIT_OK; 
}
