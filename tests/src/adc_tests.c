#include "tests.h"
#include "6502emu/cpu.h"
#include <stdlib.h>

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

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $40");
  
  /* test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x10);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, 0   );

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x11);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, 0   );

  return MUNIT_OK;
}

MunitResult adc_zpx(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0x0 ] = 0x75; // adc zp, X
  cpu.memory[0x1 ] = 0x40; // $40           , 0x40 < 0xff so its in the zeropage
  cpu.memory[0x41] = 0x10; // memory to be added with
  cpu.regX         = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $40, X");
  
  /* test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x10);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, 0   );

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x11);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 0   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, ==, 0   );

  /*  test overflow */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0xfe;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x0E);
  munit_assert_int(cpu.status_flags & SF_CARRY,    ==, 1   );
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW, !=, 0   );

  return MUNIT_OK;
}

MunitResult adc_abs(const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC $0040
  cpu.memory[0x0 ] = 0x6D; // adc abs
  cpu.memory[0x1 ] = 0x40; // $40
  cpu.memory[0x2 ] = 0x00; // $00   ; add value at $0040 to A
  cpu.memory[0x40] = 0x15;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $0040");
  
  /* test with carry flag clear */
  cpu.status_flags &= ~(SF_CARRY);
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x15);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x16);
  munit_assert_int(cpu.status_flags & SF_NEGATIVE, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,     ==, 0   );

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0x80;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x96);
  munit_assert_int((cpu.status_flags & SF_NEGATIVE) >> 7, ==, 1   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW,        !=, SF_OVERFLOW   );

  /*  test with carry flag set and trigger overflow */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0xfe;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, (uint8_t)0x114);
  munit_assert_int((cpu.status_flags & SF_NEGATIVE) >> 7, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW,        ==, SF_OVERFLOW   );
  return MUNIT_OK;
}

MunitResult adc_absx (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC $0040
  cpu.memory[0x0 ] = 0x7D; // adc abs, X
  cpu.memory[0x1 ] = 0x40; // $40
  cpu.memory[0x2 ] = 0x00; // $00   ; add value at $0040 to A
  cpu.memory[0x41] = 0x15;
  cpu.regX         = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $0040, X");

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x16);
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, 0   );
  munit_assert_int((cpu.status_flags & SF_NEGATIVE) >> 7, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW,        !=, SF_OVERFLOW   );
  return MUNIT_OK;
}

MunitResult adc_absy (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  // ADC $0040
  cpu.memory[0x0 ] = 0x79; // adc abs, Y
  cpu.memory[0x1 ] = 0x40; // $40
  cpu.memory[0x2 ] = 0x00; // $00   ; add value at $0040 to A
  cpu.memory[0x41] = 0x15;
  cpu.regY         = 1;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "ADC $0040, Y");

  /*  test with carry flag set */
  cpu.status_flags |= SF_CARRY;
  cpu.regA = 0;
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.regA, ==, 0x16);
  munit_assert_int(cpu.status_flags & SF_CARRY,           ==, 0   );
  munit_assert_int((cpu.status_flags & SF_NEGATIVE) >> 7, ==, 0   );
  munit_assert_int(cpu.status_flags & SF_ZERO,            ==, 0   );
  munit_assert_int(cpu.status_flags & SF_OVERFLOW,        !=, SF_OVERFLOW   );
  return MUNIT_OK;
}

MunitResult adc_indx(const MunitParameter params[], void* fixture) {
  return MUNIT_ERROR;
}

MunitResult adc_indy(const MunitParameter params[], void* fixture) {
  return MUNIT_ERROR;
}
