#include "tests.h"
#include "6502emu/cpu.h"

MunitResult inc_zp   (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45
  cpu.memory[0x00] = 0xE6;
  cpu.memory[0x01] = 0x76;
  cpu.memory[0x76] = 3;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INC $76");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x76], ==, 4);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}

MunitResult inc_zpx  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45, X
  cpu.memory[0x00] = 0xF6;
  cpu.memory[0x01] = 0x76;
  cpu.memory[0x77] = 3;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INC $76, X");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x77], ==, 4);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}

MunitResult inc_abs  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45, X
  cpu.memory[0x00] = 0xEE;
  cpu.memory[0x01] = 0x76;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x76] = 3;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INC $0076");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x76], ==, 4);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}

MunitResult inc_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45, X
  cpu.memory[0x00] = 0xFE;
  cpu.memory[0x01] = 0x76;
  cpu.memory[0x02] = 0x00;
  cpu.memory[0x77] = 3;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INC $0076, X");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.memory[0x77], ==, 4);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}

MunitResult inx_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45, X
  cpu.memory[0x00] = 0xE8;
  cpu.regX = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INX");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regX, ==, 2);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}

MunitResult iny_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // inc $45, X
  cpu.memory[0x00] = 0xC8;
  cpu.regY = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "INY");

  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regY, ==, 2);
  munit_assert_int(cpu.status_flags & SF_ZERO    , ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );

  return MUNIT_OK; 
}
