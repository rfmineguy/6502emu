#include "tests.h"
#include "6502emu/cpu.h"

MunitResult ror_acc  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x6A;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROR A");

  cpu.regA = 0x41;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA, ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult ror_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x66;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x60] = 0x41;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROR $60");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x60], ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult ror_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x76;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x61] = 0x41;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROR $60, X");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x61], ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult ror_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x6E;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x60] = 0x41;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROR $0060");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x60], ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult ror_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0x7E;
  cpu.memory[0x01] = 0x60;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x61] = 0x41;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  
  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ROR $0060, X");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x61], ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}
