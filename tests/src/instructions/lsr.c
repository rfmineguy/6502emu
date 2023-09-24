#include "tests.h"
#include "6502emu/cpu.h"

MunitResult lsr_acc  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x4A;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LSR A");

  cpu.regA = 0xf9;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA, ==, 0x7C);
  munit_assert_int(cpu.status_flags & SF_CARRY,     ==, SF_CARRY);
  munit_assert_int(cpu.status_flags & SF_ZERO,      ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE,  ==, 0);

  return MUNIT_OK;
}

MunitResult lsr_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x46; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0xf9;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LSR $40");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x40],               ==, 0x7C);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, SF_CARRY);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lsr_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x56; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x41] = 0xf9;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LSR $40, X");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x41],               ==, 0x7C);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, SF_CARRY);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lsr_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x4E; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[2] = 0x00;
  cpu.memory[0x0040] = 0xf9;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LSR $0040");

  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x0040],             ==, 0x7C);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, SF_CARRY);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lsr_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x5E; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[2] = 0x00;
  cpu.memory[0x0041] = 0xf9;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LSR $0040, X");

  cpu.regX = 1;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.memory[0x0041],             ==, 0x7C);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, SF_CARRY);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}
