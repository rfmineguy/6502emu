#include "tests.h"
#include "6502emu/cpu.h"

MunitResult eor_imm  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR #$4
  cpu.memory[0x0] = 0x49;
  cpu.memory[0x1] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR #$04");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR #$24
  cpu.memory[0x00] = 0x45;
  cpu.memory[0x01] = 0x24;
  cpu.memory[0x24] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR $24");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR $24, X
  cpu.memory[0x00] = 0x55;
  cpu.memory[0x01] = 0x24;
  cpu.memory[0x25] = 0x04;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR $24, X");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR $24, X
  cpu.memory[0x00] = 0x4D;
  cpu.memory[0x01] = 0x24;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x24] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR $0024");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR $24, X
  cpu.memory[0x00] = 0x5D;
  cpu.memory[0x01] = 0x24;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x25] = 0x04;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR $0024, X");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu;
  // EOR $24, X
  cpu.memory[0x00] = 0x59;
  cpu.memory[0x01] = 0x24;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x25] = 0x04;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR $0024, Y");
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);

  munit_assert_int(cpu.regA,                       ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, SF_NEGATIVE);

  return MUNIT_OK; 
}

MunitResult eor_indx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0x41; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR ($40, X)");

  cpu.regX = 1;
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                              ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE,        ==, SF_NEGATIVE   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );

  return MUNIT_OK;
}

MunitResult eor_indy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC ($40, X)
  cpu.memory[0x0 ] = 0x51; // adc (ind, X)
  cpu.memory[0x1 ] = 0x40; // $40     // use address value at $40 + X as memory to add
  cpu.memory[0x41] = 0x15; //
  cpu.memory[0x42] = 0x00; // $0015
  cpu.memory[0x15] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "EOR ($40), Y");

  cpu.regY = 1;
  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                              ==, 0xfb);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE,        ==, SF_NEGATIVE   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );

  return MUNIT_OK;
}
