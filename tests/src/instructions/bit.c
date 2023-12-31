#include "tests.h"
#include "6502emu/cpu.h"

MunitResult bit_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // SEC
  cpu.memory[0x0]  = 0x24;
  cpu.memory[0x1]  = 0x55;
  cpu.memory[0x55] = 0b11001010; //CA
  cpu.regA         = 0b11001010;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BIT $55");

  // regA = 0b11001010
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0); // if (regA & M) = 0
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, SF_OVERFLOW); // (regA & M) & 0b01000000
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE); // (regA & M) & 0b10000000

  return MUNIT_OK;
}

MunitResult bit_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // SEC
  cpu.memory[0x0]    = 0x2C;
  cpu.memory[0x1]    = 0x55;
  cpu.memory[0x2]    = 0x01;
  cpu.memory[0x0155] = 0b11001010; // 0xCA
  cpu.regA           = 0b11001010; // 0xCA

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BIT $0155");

  // regA = 0b11001010  // 0xCA
  cpu_execute(&cpu, ins);
  // cpu_dump_regs(&cpu);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0); // if (regA & M) = 0
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, SF_OVERFLOW); // (regA & M) & 0b01000000
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE); // (regA & M) & 0b10000000

  return MUNIT_OK;
}
