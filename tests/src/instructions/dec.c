#include "tests.h"
#include "6502emu/cpu.h"

MunitResult dec_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // dec $0x40
  cpu.memory[0x00] = 0xC6;
  cpu.memory[0x01] = 0x40;
  cpu.memory[0x40] = 0x03;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEC $40");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  // dec $0x40
  cpu.memory[0x0]  = 0xC6;
  cpu.memory[0x1]  = 0x40;
  cpu.memory[0x40] = 0x00;

  ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEC $40");

  cpu_execute(&cpu, ins);
  // cpu_dump_regs(&cpu);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK;
}

MunitResult dec_zpx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  //dec $40, X
  cpu.memory[0x00] = 0xD6;
  cpu.memory[0x01] = 0x40;
  cpu.memory[0x41] = 0x01;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEC $40, X");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult dec_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  //dec $0040
  cpu.memory[0x00] = 0xCE;
  cpu.memory[0x01] = 0x40;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x40] = 0x01;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEC $0040");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult dec_absx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  //dec $0040
  cpu.memory[0x00] = 0xDE;
  cpu.memory[0x01] = 0x40;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x41] = 0x01;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEC $0040, X");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK; 
}

MunitResult dex_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  //dex
  cpu.memory[0x00] = 0xCA;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEX");

  cpu.regX = 1;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  cpu.regX = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult dey_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  //dex
  cpu.memory[0x00] = 0x88;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "DEY");

  cpu.regY = 1;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, SF_ZERO);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  cpu.regY = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO, ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}
