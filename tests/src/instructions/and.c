#include "tests.h"
#include "6502emu/cpu.h"
#include <stdlib.h>
#include <stdio.h>

MunitResult and_imm(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x29; // and imm
  cpu.memory[1] = 0x80; // #$80

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "AND #$80");

  cpu.regA = 0xff;                          // lda #ff
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x80);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 0   );
  return MUNIT_OK;
}

MunitResult and_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x25; // and zp
  cpu.memory[1] = 0x80; // $80
  cpu.memory[0x80] = 0x84;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "AND $80");

  cpu.regA = 0xff;                          // lda #ff
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x84);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );
  return MUNIT_OK;
}

MunitResult and_zpx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x35; // and zp
  cpu.memory[1] = 0x80; // $80
  cpu.memory[0x81] = 0x84;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "AND $80, X");

  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x84);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );
  return MUNIT_OK;
}

MunitResult and_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x2D; // and abs
  cpu.memory[1] = 0x00; // $00
  cpu.memory[2] = 0x20; // $20
  cpu.memory[0x2000] = 0x84;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  munit_assert_string_equal(ins.str, "AND $2000");

  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x84);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regA = 0x7f;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x04);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}

MunitResult and_absx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x3D; // and abs, x
  cpu.memory[1] = 0x00; // $00
  cpu.memory[2] = 0x20; // $20
  cpu.memory[0x2004] = 0x84;
  cpu.regX      = 4;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  munit_assert_string_equal(ins.str, "AND $2000, X");

  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x84);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regA = 0x7f;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x04);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}

MunitResult and_absy(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x39; // and abs, y
  cpu.memory[1] = 0x00; // $00
  cpu.memory[2] = 0x20; // $20
  cpu.memory[0x2004] = 0x84;
  cpu.regY      = 4;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_string_equal(ins.str, "AND $2000, Y");

  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x84);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regA = 0x7f;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0x04);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}

MunitResult and_indx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // AND ($40, X)
  cpu.memory[0x0 ] = 0x21; // and (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x15;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "AND ($40, X)");

  cpu.regX = 1;
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x15);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regX = 1;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK;
}

MunitResult and_indy(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // AND ($40, Y)
  cpu.memory[0x0 ] = 0x31; // and (ind, Y)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x15;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "AND ($40), Y");

  cpu.regY = 1;
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x15);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regY = 1;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK;
}
