#include "tests.h"
#include "6502emu/cpu.h"

MunitResult jsr_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  cpu.memory[0x00] = 0x20;
  cpu.memory[0x01] = 0x00;
  cpu.memory[0x02] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "JSR $2000");

  cpu.pc = 4;
  cpu.sp = CPU_STACK_SIZE;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, 0x2000);
  munit_assert_int(cpu.memory[CPU_STACK_BASE + cpu.sp], ==, 3);

  return MUNIT_OK;
}
