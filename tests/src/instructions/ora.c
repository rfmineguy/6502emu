#include "tests.h"
#include "6502emu/cpu.h"

MunitResult ora_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x09;
  cpu.memory[0x1] = 0x53;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA #$53");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x05;
  cpu.memory[0x1] = 0x43;
  cpu.memory[0x43] = 0x53;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA $43");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x15;
  cpu.memory[0x1] = 0x43;
  cpu.memory[0x44] = 0x53;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA $43, X");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x0D;
  cpu.memory[0x1] = 0x43;
  cpu.memory[0x2] = 0x00;
  cpu.memory[0x43] = 0x53;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA $0043");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x1D;
  cpu.memory[0x1] = 0x43;
  cpu.memory[0x2] = 0x00;
  cpu.memory[0x44] = 0x53;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA $0043, X");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x19;
  cpu.memory[0x1] = 0x43;
  cpu.memory[0x2] = 0x00;
  cpu.memory[0x44] = 0x53;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA $0043, Y");

  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);
  return MUNIT_OK;
}

MunitResult ora_indx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0x01; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x53;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA ($40, X)");

  cpu.status_flags |= SF_CARRY;
  cpu.regX = 1;
  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}

MunitResult ora_indy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0x11; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x53;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ORA ($40), Y");

  cpu.status_flags |= SF_CARRY;
  cpu.regY = 1;
  cpu.regA = 0x4;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x57);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0);

  return MUNIT_OK;
}
