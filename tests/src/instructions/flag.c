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

  return MUNIT_ERROR;
}

MunitResult cli_imp(const MunitParameter params[], void* fixture) {

  return MUNIT_ERROR;
}

MunitResult clv_imp(const MunitParameter params[], void* fixture) {

  return MUNIT_ERROR;
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
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, 1   );
  return MUNIT_OK;
}

MunitResult sed_imp(const MunitParameter params[], void* fixture) {

  return MUNIT_ERROR;
}

MunitResult sei_imp(const MunitParameter params[], void* fixture) {

  return MUNIT_ERROR;
}
