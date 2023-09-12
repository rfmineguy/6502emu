#include "tests.h"
#include "6502emu/cpu.h"
#include <stdlib.h>

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitResult adc_imm(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x69; //adc imm
  cpu.memory[1] = 0x40; //#$40
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC #$40");

  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x40);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA,                       ==, 0x41);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}

MunitResult adc_zp(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0 ] = 0x65; // adc zp
  cpu.memory[0x1 ] = 0x40; // $40           , 0x40 < 0xff so its in the zeropage
  cpu.memory[0x40] = 0x10; // memory to be added with
    
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $40");
  
  /* Test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x10);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  /*  Test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x11);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  return MUNIT_OK;
}
