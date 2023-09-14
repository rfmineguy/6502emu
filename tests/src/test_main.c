#include "instructions/tests.h"

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitTest random_tests[] = {
  {"/bit_shift", bit_shift_tests, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest adc_tests[] = {
  {"/imm",  adc_imm,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",   adc_zp,   setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",  adc_zpx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",  adc_abs,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx", adc_absx, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy", adc_absy, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx", adc_indx, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy", adc_indy, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest and_tests[] = {
  {"/imm",  and_imm,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",   and_zp,   setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",  and_zpx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",  and_abs,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx", and_absx, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy", and_absy, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx", and_indx, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy", and_indy, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest asl_tests[] = {
  {"/acc",  asl_acc,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",   asl_zp,   setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",  asl_zpx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",  asl_abs,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx", asl_absx, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest bit_tests[] = {
  {"/zp",   bit_zp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",  bit_abs,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest branch_tests[] = {
  {"/bcc",  bcc_rel,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/bcs",  bcs_rel,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/beq",  beq_rel,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest brk_tests[] = {
  {"/brk",  brk    ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest clear_status_tests[] = {
  {"/clc_imp",  clc_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cld_imp",  cld_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cli_imp",  cli_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/clv_imp",  clv_imp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},  
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest set_status_tests[] = {
  {"/sec_imp",  sec_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/sed_imp",  sed_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/sei_imp",  sei_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest cmp_tests[] = {
  {"/imm",   cmp_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    cmp_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   cmp_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   cmp_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  cmp_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  cmp_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  cmp_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  cmp_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest cpx_tests[] = {
  {"/imm",   cpx_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    cpx_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   cpx_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest cpy_tests[] = {
  {"/imm",   cpy_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    cpy_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   cpy_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest dec_tests[] = {
  {"/zp",   dec_zp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   dec_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   dec_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  dec_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest dex_tests[] = {
  {"/imp",   dex_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest dey_tests[] = {
  {"/imp",   dey_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest eor_tests[] = {
  {"/imm",   eor_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    eor_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   eor_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   eor_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  eor_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  eor_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  eor_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  eor_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest inc_tests[] = {
  {"/zp",    inc_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   inc_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   inc_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  inc_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest inx_tests[] = {
  {"/imp",   inx_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest iny_tests[] = {
  {"/imp",   iny_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest jmp_tests[] = {
  {"/abs",   jmp_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/imp",   jmp_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest jsr_tests[] = {
  {"/abs",   jsr_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest lda_tests[] = {
  {"/imm",   lda_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    lda_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   lda_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   lda_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  lda_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  lda_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  lda_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  lda_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest ldx_tests[] = {
  {"/imm",   ldx_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    ldx_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   ldx_zpy ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   ldx_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  ldx_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest ldy_tests[] = {
  {"/imm",   ldy_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    ldy_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   ldy_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   ldy_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  ldy_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest lsr_tests[] = {
  {"/acc",   lsr_acc ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    lsr_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   lsr_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   lsr_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  lsr_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest nop_tests[] = {
  {"/nop",  nop,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest ora_tests[] = {
  {"/imm",   ora_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    ora_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   ora_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   ora_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  ora_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  ora_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  ora_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  ora_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest pha_tests[] = {
  {"/imp",  pha_imp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest php_tests[] = {
  {"/imp",  php_imp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest pla_tests[] = {
  {"/imp",  pla_imp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest plp_tests[] = {
  {"/imp",  plp_imp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest rol_tests[] = {
  {"/acc",   rol_acc ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    rol_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   rol_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   rol_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  rol_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest ror_tests[] = {
  {"/acc",   ror_acc,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    ror_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   ror_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   ror_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  ror_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest rti_tests[] = {
  {"/imp",   rti_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest rts_tests[] = {
  {"/imp",   rts_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest sbc_tests[] = {
  {"/imm",   sbc_imm ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",    sbc_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   sbc_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   sbc_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  sbc_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  sbc_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  sbc_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  sbc_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest sec_tests[] = {
  { "/sec/imp", sec_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/sed/imp", sed_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/sei/imp", sei_imp, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest sta_tests[] = {
  {"/zp",    sta_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   sta_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   sta_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absx",  sta_absx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/absy",  sta_absy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indx",  sta_indx,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indy",  sta_indy,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest stx_tests[] = {
  {"/zp",    stx_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpy",   stx_zpy ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   stx_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest sty_tests[] = {
  {"/zp",    sty_zp  ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zpx",   sty_zpx ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/abs",   sty_abs ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
MunitTest transfer_tests[] = {
  {"/tax_imp", tax_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/tay_imp", tay_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/tsx_imp", tsx_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/txa_imp", txa_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/txs_imp", txs_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/tya_imp", tya_imp ,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite intermediate[] = {
  {"/random", random_tests,          NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/adc",    adc_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/and",    and_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/asl",    asl_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/branch", branch_tests,          NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/bit",    bit_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/brk",    brk_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/clear",  clear_status_tests,    NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/set",    set_status_tests,      NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/cmp",    cmp_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/cpx",    cpx_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/cpy",    cpy_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/dec",    dec_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/dex",    dex_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/dey",    dey_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/eor",    eor_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/inc",    inc_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/inx",    inx_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/iny",    iny_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/jmp",    jmp_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/jsr",    jsr_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/lda",    lda_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/ldx",    ldx_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/ldy",    ldy_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/lsr",    lsr_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/nop",    nop_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/ora",    ora_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/pha",    pha_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/php",    php_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/pla",    pla_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/plp",    plp_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/rol",    rol_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/ror",    ror_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/rti",    rti_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/rts",    rts_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/sbc",    sbc_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/sec",    sec_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/sta",    sta_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/stx",    stx_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/sty",    sty_tests,             NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/transf", transfer_tests,        NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

static const MunitSuite main_suite = {
  "6502",
  NULL, //tests,
  intermediate,
  1,
  MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, const char** argv) {
  return munit_suite_main(&main_suite, NULL, argc, (char *const *)argv);
}

