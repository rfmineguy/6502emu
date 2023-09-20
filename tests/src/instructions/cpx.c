#include "tests.h"
#include "6502emu/cpu.h"

MunitResult cpx_imm(const MunitParameter params[], void* fixture) { 
  cpu_t cpu;
  // cmp #$40
  cpu.memory[0x0]  = 0xE0;
  cpu.memory[0x1]  = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CPX #$40");
  cpu.regX = 0x80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult cpx_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]  = 0xE4;
  cpu.memory[0x1]  = 0x20;
  cpu.memory[0x20] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  munit_assert_string_equal(ins.str, "CPX $20");
  cpu.regX = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cpx_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]    = 0xEC;
  cpu.memory[0x1]    = 0x20;
  cpu.memory[0x2]    = 0x00;
  cpu.memory[0x0020] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  munit_assert_string_equal(ins.str, "CPX $0020");
  cpu.regX = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}
