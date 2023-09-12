#include "tests.h"
#include "6502emu/cpu.h"
#include <stdlib.h>

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
}

MunitResult and_zp(const MunitParameter params[], void* fixture) {

}

MunitResult and_zpx(const MunitParameter params[], void* fixture) {

}

MunitResult and_abs(const MunitParameter params[], void* fixture) {

}

MunitResult and_absx(const MunitParameter params[], void* fixture) {

}

MunitResult and_absy(const MunitParameter params[], void* fixture) {

}

MunitResult and_indx(const MunitParameter params[], void* fixture) {

}

MunitResult and_indy(const MunitParameter params[], void* fixture) {

}
