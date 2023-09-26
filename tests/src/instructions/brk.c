#include "tests.h"
#include "6502emu/cpu.h"

MunitResult brk(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // BRK
  cpu.memory[0x00] = 0x00;

  cpu.memory[0xfffe] = 0x00;
  cpu.memory[0xffff] = 0x80;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BRK");

  // stack is from 0x100 - 0x1ff
  cpu.pc = 3;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags & SF_BRK_CMD,                        ==, SF_BRK_CMD); // (regA & M) & 0b01000000
  munit_assert_int(cpu.pc,                                               ==, 0x8000);
  munit_assert_int((uint16_t)cpu.memory[CPU_STACK_BASE + (cpu.sp + 1)],  ==, 0x0003 - 1);

  return MUNIT_OK;
}
