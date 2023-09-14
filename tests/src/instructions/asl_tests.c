#include "tests.h"
#include "6502emu/cpu.h"
#include <stdio.h>
#include <stdint.h>

MunitResult asl_acc(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0] = 0x0A; // ASL A

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  munit_assert_string_equal(ins.str, "ASL A");

  cpu.regA = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0xfe);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, !=, 1   );

  cpu.regA = 0x0f;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x1e);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   ); // negative flag should be off
  return MUNIT_OK;
}

MunitResult asl_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // asl $45
  cpu.memory[0x0]  = 0x06;
  cpu.memory[0x1]  = 0x45;
  cpu.memory[0x45] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  // *(uint16_t*)(ins.raw + 1)
  munit_assert_string_equal(ins.str, "ASL $45");
  munit_assert_int(cpu.memory[0x01],           ==, 0x45);
  munit_assert_int(cpu.memory[*(ins.raw + 1)], ==, 0x04);

  // testing memory shifting
  cpu.memory[0x45] <<= 1;
  munit_assert_int(cpu.memory[0x45],               ==, 0x08);
  cpu.memory[0x45] >>= 1;
  munit_assert_int(cpu.memory[0x45],               ==, 0x04);
  
  // execute instruction
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], !=, 0x04);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], ==, 0x08);
  munit_assert_int(cpu.memory[0x45],                     ==, 0x08);

  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  // execute instruction again
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], !=, 0x08);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], ==, 0x10);

  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   ); // negative flag should be off

  // invoke carry flag
  cpu.memory[0x45] = 0xff;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], !=, 0xff);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1)], ==, 0xfe);

  // test the affected flags
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, 1   ); // carry flag should be on
  munit_assert_int((cpu.status_flags & SF_NEGATIVE) >> 7, ==, 1   ); // negative flag should be on
  return MUNIT_OK;
}

MunitResult asl_zpx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // asl $45, X
  cpu.memory[0x0]  = 0x16;
  cpu.memory[0x1]  = 0x45;
  cpu.memory[0x46] = 0x04;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 2);

  // *(uint16_t*)(ins.raw + 1)
  munit_assert_string_equal(ins.str, "ASL $45, X");
  munit_assert_int(cpu.memory[0x01],                      ==, 0x45);
  munit_assert_int(cpu.memory[*(ins.raw + 1) + cpu.regX], ==, 0x04);

  // execute instruction
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1) + cpu.regX], !=, 0x04);
  munit_assert_int(cpu.memory[*(uint8_t*)(ins.raw + 1) + cpu.regX], ==, 0x08);
  munit_assert_int(cpu.memory[0x45 + cpu.regX],                     ==, 0x08);

  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}

MunitResult asl_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // asl $45, X
  cpu.memory[0x0]  = 0x0E;
  cpu.memory[0x1]  = 0x45;
  cpu.memory[0x2]  = 0x24;
  cpu.memory[0x2445] = 0x04;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  // *(uint16_t*)(ins.raw + 1)
  munit_assert_string_equal(ins.str, "ASL $2445");
  munit_assert_int(cpu.memory[0x01],                                 ==, 0x45);
  munit_assert_int(cpu.memory[0x02],                                 ==, 0x24);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1)],            ==, 0x04);

  // execute instruction
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1)], !=, 0x04);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1)], ==, 0x08);
  munit_assert_int(cpu.memory[0x2445],                    ==, 0x08);

  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}

MunitResult asl_absx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};

  // asl $45, X
  cpu.memory[0x0]  = 0x1E;
  cpu.memory[0x1]  = 0x45;
  cpu.memory[0x2]  = 0x24;
  cpu.memory[0x2446] = 0x04;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);
  munit_assert_int(ins.bytes, ==, 3);

  // *(uint16_t*)(ins.raw + 1)
  munit_assert_string_equal(ins.str, "ASL $2445, X");
  munit_assert_int(cpu.memory[0x01],                                 ==, 0x45);
  munit_assert_int(cpu.memory[0x02],                                 ==, 0x24);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1) + cpu.regX], ==, 0x04);

  // execute instruction
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1) + cpu.regX], !=, 0x04);
  munit_assert_int(cpu.memory[*(uint16_t*)(ins.raw + 1) + cpu.regX], ==, 0x08);
  munit_assert_int(cpu.memory[0x2445 + cpu.regX],                    ==, 0x08);

  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   ); // zero flag should be off
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  return MUNIT_OK;
}
