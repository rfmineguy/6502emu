#include "tests.h"
#include "6502emu/cpu.h"
#include <stdio.h>
#include <stdint.h>

MunitResult clc_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // CLC
  cpu.memory[0x0] = 0x18;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CLC");

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, 0   );
  return MUNIT_OK;
}

MunitResult cld_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  cpu.memory[0x0] = 0xD8;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CLD");

  /*  test with carry flag set */
  cpu.status_flags |= SF_DECIMAL;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_DECIMAL,           ==, 0   );
  return MUNIT_OK;
}

MunitResult cli_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  cpu.memory[0x0] = 0x58;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CLI");

  /*  test with carry flag set */
  cpu.status_flags |= SF_INTDISA;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_INTDISA,           ==, 0   );
  return MUNIT_OK;
}

MunitResult clv_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  cpu.memory[0x0] = 0xB8;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "CLV");

  /*  test with carry flag set */
  cpu.status_flags |= SF_OVERFLOW;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_OVERFLOW,           ==, 0   );
  return MUNIT_OK;
}

MunitResult sec_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // SEC
  cpu.memory[0x0] = 0x38;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SEC");

  /*  test with carry flag cleared */
  cpu.status_flags &= ~(SF_CARRY);
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, SF_CARRY   );
  return MUNIT_OK;
}

MunitResult sed_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // SED
  cpu.memory[0x0] = 0xF8;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SED");

  /*  test with carry flag cleared */
  cpu.status_flags &= ~(SF_DECIMAL);
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_DECIMAL,           ==, SF_DECIMAL   );

  return MUNIT_OK;
}

MunitResult sei_imp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // SEI
  cpu.memory[0x0] = 0x78;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SEI");

  /*  test with carry flag cleared */
  cpu.status_flags &= ~(SF_INTDISA);
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_INTDISA,           ==, SF_INTDISA   );

  return MUNIT_OK;
}
