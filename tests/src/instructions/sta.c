#include "tests.h"
#include "6502emu/cpu.h"

MunitResult sta_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x85;
  cpu.memory[0x1] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA $40");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x40], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x95;
  cpu.memory[0x1] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA $40, X");

  cpu.regA = 0x45;
  cpu.regX = 1;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x41], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x8D;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;
  cpu.memory[0x4000] = 0x10;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA $4000");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x4000], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x9D;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;
  cpu.memory[0x4001] = 0x10;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA $4000, X");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x4001], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x99;
  cpu.memory[0x1] = 0x00;
  cpu.memory[0x2] = 0x40;
  cpu.memory[0x4001] = 0x10;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA $4000, Y");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x4001], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_indx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x81;
  cpu.memory[0x1] = 0x40;
  cpu.memory[0x41] = 0x90;
  cpu.memory[0x42] = 0x00;
  cpu.memory[0x0090] = 0x10;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA ($40, X)");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x0090], ==, 0x45);

  return MUNIT_OK; 
}

MunitResult sta_indy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x91;
  cpu.memory[0x1] = 0x40;
  cpu.memory[0x41] = 0x90;
  cpu.memory[0x42] = 0x00;
  cpu.memory[0x0090] = 0x10;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "STA ($40), Y");

  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x0090], ==, 0x45);

  return MUNIT_OK; 
}
