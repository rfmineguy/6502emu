#include "tests.h"
#include "6502emu/cpu.h"
#include <stdio.h>

MunitResult bcc_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x90;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BCC");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_CARRY); // clear carry flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x90;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BCC");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_CARRY); // clear carry flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bcs_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xB0;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BCS");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_CARRY; // set carry flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xB0;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BCS");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_CARRY; // set carry flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult beq_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xF0;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BEQ");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_ZERO; // set zero flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xF0;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BEQ");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_ZERO; // set zero flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bmi_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x30;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BMI");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_NEGATIVE; // set negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x30;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BMI");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_NEGATIVE; // set negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bne_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xD0;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BNE");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_ZERO); // clear zero flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0xD0;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BNE");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_ZERO); // clear zero flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bpl_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x10;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BPL");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_NEGATIVE); // clear negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x10;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BPL");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_NEGATIVE); // clear negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bvc_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x50;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BVC");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_OVERFLOW); // clear overflow flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x50;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BVC");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags &= ~(SF_OVERFLOW); // clear negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}

MunitResult bvs_rel(const MunitParameter params[], void* fixture) {
  cpu_t cpu;

  /* TEST #1 jump one space forward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x70;
  cpu.memory[0x1] = 0x01; // branch 1 byte forward relative to the current pc
  cpu.pc = 0;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BVS");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_OVERFLOW; // clear overflow flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc + 1);
  }

  /* TEST #2 jump one space backward */
  {
  // this instruction uses signed numbers
  cpu.memory[0x0] = 0x70;
  cpu.memory[0x1] = 0xFF; // branch 1 byte backward relative to the current pc
  cpu.pc = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "BVS");

  uint16_t prev_pc = cpu.pc;
  cpu.status_flags |= SF_OVERFLOW; // clear negative flag
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc, ==, prev_pc - 1);
  }

  return MUNIT_OK;
}
