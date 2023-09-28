#include "tests.h"
#include "6502emu/cpu.h"

MunitResult sbc_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0xE9; //sbc imm
  cpu.memory[1] = 0x40; //#$40
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC #$40");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0xE5; //sbc imm
  cpu.memory[0x01] = 0xfe; //#$40
  cpu.memory[0xfe] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC $FE");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0xF5; //sbc imm
  cpu.memory[0x01] = 0xfe; //#$40
  cpu.memory[0xff] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC $FE, X");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0xED; //sbc imm
  cpu.memory[0x01] = 0x15; //#$40
  cpu.memory[0x02] = 0x00; //#$40
  cpu.memory[0x15] = 0x40;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC $0015");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0xFD; //sbc absx
  cpu.memory[0x01] = 0x15; //#$40
  cpu.memory[0x02] = 0x00; //#$40
  cpu.memory[0x16] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC $0015, X");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x00] = 0xF9; //sbc absx
  cpu.memory[0x01] = 0x15; //#$40
  cpu.memory[0x02] = 0x00; //#$40
  cpu.memory[0x16] = 0x40;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC $0015, Y");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_indx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0xE1; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x40;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC ($40, X)");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}

MunitResult sbc_indy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0xF1; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x40;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "SBC ($40), Y");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xff   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x40;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x00    );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0       );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, SF_ZERO );

  return MUNIT_OK;
}
