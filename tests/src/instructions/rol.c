#include "tests.h"
#include "6502emu/cpu.h"

MunitResult rol_acc  (const MunitParameter params[], void* fixture) { 
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x2A;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROL A");

  cpu.regA = 0x41;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA, ==, 0x82);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}

MunitResult rol_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x26;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x60] = 0x41;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROL $60");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x60], ==, 0x82);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}

MunitResult rol_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x36;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x61] = 0x41;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROL $60, X");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x61], ==, 0x82);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}

MunitResult rol_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x2E;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x60] = 0x41;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROL $0060");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x60], ==, 0x82);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}

MunitResult rol_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x3E;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x61] = 0x41;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROL $0060, X");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x61], ==, 0x82);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}
