#include "tests.h"
#include "6502emu/cpu.h"

MunitResult pha_imp  (const MunitParameter params[], void* fixture) { 
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x48;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "PHA");

  cpu.sp = CPU_STACK_SIZE;
  cpu.regA = 0x45;
  uint8_t p_sp = cpu.sp;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[CPU_STACK_BASE + cpu.sp + 1], ==, 0x45);
  munit_assert_int(cpu.sp,                                  ==, p_sp - 1);

  return MUNIT_OK; 
}

MunitResult php_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x08;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "PHP");

  cpu.sp = CPU_STACK_SIZE;
  cpu.status_flags = 0x45;
  uint8_t p_sp = cpu.sp;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[CPU_STACK_BASE + cpu.sp + 1], ==, 0x45);
  munit_assert_int(cpu.sp,                                  ==, p_sp - 1);

  return MUNIT_OK;
}

MunitResult pla_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x68;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "PLA");

  cpu.sp = CPU_STACK_SIZE;
  // PHP
  cpu.regA = 0x45;
  cpu.memory[CPU_STACK_BASE + cpu.sp--] = cpu.regA;

  uint8_t p_sp = cpu.sp;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                                ==, 0x45);
  munit_assert_int(cpu.sp,                                  ==, p_sp + 1);

  return MUNIT_OK;
}

MunitResult plp_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x28;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 1);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "PLP");

  cpu.sp = CPU_STACK_SIZE;
  // PHP
  cpu.status_flags = 0x45;
  cpu.memory[CPU_STACK_BASE + cpu.sp--] = cpu.status_flags;

  uint8_t p_sp = cpu.sp;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.status_flags,                        ==, 0x45);
  munit_assert_int(cpu.sp,                                  ==, p_sp + 1);

  return MUNIT_OK;
}
