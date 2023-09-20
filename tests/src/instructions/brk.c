#include "tests.h"
#include "6502emu/cpu.h"

MunitResult brk(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // BRK
  cpu.memory[0x0] = 0x0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BRK");

  // stack is from 0x100 - 0x1ff
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_BRK_CMD, ==, SF_BRK_CMD); // (regA & M) & 0b01000000

  return MUNIT_ERROR;
}
