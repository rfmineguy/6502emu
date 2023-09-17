#include "tests.h"
#include "6502emu/cpu.h"

MunitResult cmp_imm  (const MunitParameter params[], void* fixture) { 
  cpu_t cpu;
  // cmp #$40
  cpu.memory[0x0]  = 0xC9;
  cpu.memory[0x1]  = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CMP #$40");
  cpu.regA = 0x80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult cmp_zp   (const MunitParameter params[], void* fixture) { 
  cpu_t cpu;
  cpu.memory[0x0]  = 0xC5;
  cpu.memory[0x1]  = 0x20;
  cpu.memory[0x20] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  munit_assert_string_equal(ins.str, "CMP $20");
  cpu.regA = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cmp_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]  = 0xD5;
  cpu.memory[0x1]  = 0x20;
  cpu.memory[0x21] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  munit_assert_string_equal(ins.str, "CMP $20, X");
  cpu.regA = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cmp_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]    = 0xCD;
  cpu.memory[0x1]    = 0x20;
  cpu.memory[0x2]    = 0x00;
  cpu.memory[0x0020] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  munit_assert_string_equal(ins.str, "CMP $0020");
  cpu.regA = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cmp_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]    = 0xDD;
  cpu.memory[0x1]    = 0x20;
  cpu.memory[0x2]    = 0x00;
  cpu.memory[0x0021] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  munit_assert_string_equal(ins.str, "CMP $0020, X");
  cpu.regA = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cmp_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x0]    = 0xD9;
  cpu.memory[0x1]    = 0x20;
  cpu.memory[0x2]    = 0x00;
  cpu.memory[0x0021] = 0x40;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  munit_assert_string_equal(ins.str, "CMP $0020, Y");
  cpu.regA = 80;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult cmp_indx (const MunitParameter params[], void* fixture) {
  return MUNIT_ERROR; 
}

MunitResult cmp_indy (const MunitParameter params[], void* fixture) {
  return MUNIT_ERROR; 
}
