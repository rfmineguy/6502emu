#include "tests.h"
#include "6502emu/cpu.h"

MunitResult ldy_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA0; //adc imm
  cpu.memory[1] = 0x40; //#$40
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDY #$40");

  cpu.regY = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY,                       ==, 0x40);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldy_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA4; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDY $40");

  cpu.regY = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldy_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xB4; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x41] = 0x20;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDY $40, X");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldy_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xAC;
  cpu.memory[1] = 0x40;
  cpu.memory[2] = 0x00;
  cpu.memory[0x0040] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDY $0040");

  cpu.regY = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldy_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xBC;
  cpu.memory[1] = 0x40;
  cpu.memory[2] = 0x00;
  cpu.memory[0x0041] = 0x20;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDY $0040, X");

  cpu.regY = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}
