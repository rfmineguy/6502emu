#include "tests.h"
#include "6502emu/cpu.h"

MunitResult ldx_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA2; //adc imm
  cpu.memory[1] = 0x40; //#$40
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDX #$40");

  cpu.regX = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX,                       ==, 0x40);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldx_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA6; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDX $40");

  cpu.regX = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldx_zpy  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xB6; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x41] = 0x20;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDX $40, Y");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldx_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xAE;
  cpu.memory[1] = 0x40;
  cpu.memory[2] = 0x00;
  cpu.memory[0x0040] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDX $0040");

  cpu.regX = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult ldx_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xBE;
  cpu.memory[1] = 0x40;
  cpu.memory[2] = 0x00;
  cpu.memory[0x0041] = 0x20;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDX $0040, Y");

  cpu.regX = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}
