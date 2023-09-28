#include "tests.h"
#include "6502emu/cpu.h"

MunitResult tax_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0xAA;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TAX");

  cpu.regA = 0x0;
  cpu.regX = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX, ==, cpu.regA);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult tay_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0xA8;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TAY");

  cpu.regA = 0x0;
  cpu.regY = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX, ==, cpu.regY);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}
  
MunitResult tsx_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0xBA;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TSX");

  cpu.regX = 0x00;
  cpu.sp = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX, ==, cpu.sp);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult txa_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x8A;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TXA");

  cpu.regX = 0x00;
  cpu.regA = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, cpu.regX);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult txs_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x9A;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TXS");

  cpu.sp   = 0x00;
  cpu.regX = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.sp, ==, cpu.regX);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult tya_imp (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x98;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "TYA");

  cpu.regA = 0x00;
  cpu.regY = 0x45;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, cpu.regY);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}
