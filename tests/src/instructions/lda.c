#include "tests.h"
#include "6502emu/cpu.h"

MunitResult lda_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA9; //adc imm
  cpu.memory[1] = 0x40; //#$40
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA #$40");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x40);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA5; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA $40");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xB5; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x41] = 0x20;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA $40, X");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xAD; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[2] = 0x00;
  cpu.memory[0x0040] = 0x20;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA $0040");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xBD; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[2] = 0x00;
  cpu.memory[0x0041] = 0x20;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA $0040, X");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xB9; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[2] = 0x00;
  cpu.memory[0x0041] = 0x20;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA $0040, Y");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_indx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xA1; //adc imm
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0x20;
  cpu.memory[0x20] = 0x20;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA ($40, X)");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult lda_indy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xB1;
  cpu.memory[1] = 0x40; //#$40
  cpu.memory[0x40] = 0x20;
  cpu.memory[0x41] = 0x00;
  cpu.memory[0x0020] = 0x20;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "LDA ($40), Y");

  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x20);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}
