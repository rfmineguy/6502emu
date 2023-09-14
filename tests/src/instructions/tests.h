#ifndef TESTS_H
#define TESTS_H
#include "../munit.h"


MunitResult bit_shift_tests(const MunitParameter[], void*);

MunitResult adc_imm  (const MunitParameter[], void*);
MunitResult adc_zp   (const MunitParameter[], void*);
MunitResult adc_zpx  (const MunitParameter[], void*);
MunitResult adc_abs  (const MunitParameter[], void*);
MunitResult adc_absx (const MunitParameter[], void*);
MunitResult adc_absy (const MunitParameter[], void*);
MunitResult adc_indx (const MunitParameter[], void*);
MunitResult adc_indy (const MunitParameter[], void*);

MunitResult and_imm  (const MunitParameter[], void*);
MunitResult and_zp   (const MunitParameter[], void*);
MunitResult and_zpx  (const MunitParameter[], void*);
MunitResult and_abs  (const MunitParameter[], void*);
MunitResult and_absx (const MunitParameter[], void*);
MunitResult and_absy (const MunitParameter[], void*);
MunitResult and_indx (const MunitParameter[], void*);
MunitResult and_indy (const MunitParameter[], void*);

MunitResult asl_acc  (const MunitParameter[], void*);
MunitResult asl_zp   (const MunitParameter[], void*);
MunitResult asl_zpx  (const MunitParameter[], void*);
MunitResult asl_abs  (const MunitParameter[], void*);
MunitResult asl_absx (const MunitParameter[], void*);

MunitResult bit_zp   (const MunitParameter[], void*);
MunitResult bit_abs  (const MunitParameter[], void*);

MunitResult bcc_rel  (const MunitParameter[], void*);
MunitResult bcs_rel  (const MunitParameter[], void*);
MunitResult beq_rel  (const MunitParameter[], void*);
MunitResult bmi_rel  (const MunitParameter[], void*);
MunitResult bne_rel  (const MunitParameter[], void*);
MunitResult bpl_rel  (const MunitParameter[], void*);
MunitResult bvc_rel  (const MunitParameter[], void*);
MunitResult bvs_rel  (const MunitParameter[], void*);

MunitResult brk      (const MunitParameter[], void*);

MunitResult clc_imp  (const MunitParameter[], void*);
MunitResult cld_imp  (const MunitParameter[], void*);
MunitResult cli_imp  (const MunitParameter[], void*);
MunitResult clv_imp  (const MunitParameter[], void*);

MunitResult cmp_imm  (const MunitParameter[], void*);
MunitResult cmp_zp   (const MunitParameter[], void*);
MunitResult cmp_zpx  (const MunitParameter[], void*);
MunitResult cmp_abs  (const MunitParameter[], void*);
MunitResult cmp_absx (const MunitParameter[], void*);
MunitResult cmp_absy (const MunitParameter[], void*);
MunitResult cmp_indx (const MunitParameter[], void*);
MunitResult cmp_indy (const MunitParameter[], void*);

MunitResult cpx_imm  (const MunitParameter[], void*);
MunitResult cpx_zp   (const MunitParameter[], void*);
MunitResult cpx_abs  (const MunitParameter[], void*);

MunitResult cpy_imm  (const MunitParameter[], void*);
MunitResult cpy_zp   (const MunitParameter[], void*);
MunitResult cpy_abs  (const MunitParameter[], void*);

MunitResult dec_zp   (const MunitParameter[], void*);
MunitResult dec_zpx  (const MunitParameter[], void*);
MunitResult dec_abs  (const MunitParameter[], void*);
MunitResult dec_absx (const MunitParameter[], void*);

MunitResult dex_imp  (const MunitParameter[], void*);
MunitResult dey_imp  (const MunitParameter[], void*);

MunitResult eor_imm  (const MunitParameter[], void*);
MunitResult eor_zp   (const MunitParameter[], void*);
MunitResult eor_zpx  (const MunitParameter[], void*);
MunitResult eor_abs  (const MunitParameter[], void*);
MunitResult eor_absx (const MunitParameter[], void*);
MunitResult eor_absy (const MunitParameter[], void*);
MunitResult eor_indx (const MunitParameter[], void*);
MunitResult eor_indy (const MunitParameter[], void*);

MunitResult inc_zp   (const MunitParameter[], void*);
MunitResult inc_zpx  (const MunitParameter[], void*);
MunitResult inc_abs  (const MunitParameter[], void*);
MunitResult inc_absx (const MunitParameter[], void*);

MunitResult inx_imp  (const MunitParameter[], void*);
MunitResult iny_imp  (const MunitParameter[], void*);

MunitResult jmp_abs  (const MunitParameter[], void*);
MunitResult jmp_imp  (const MunitParameter[], void*);

MunitResult jsr_abs  (const MunitParameter[], void*);

MunitResult lda_imm  (const MunitParameter[], void*);
MunitResult lda_zp   (const MunitParameter[], void*);
MunitResult lda_zpx  (const MunitParameter[], void*);
MunitResult lda_abs  (const MunitParameter[], void*);
MunitResult lda_absx (const MunitParameter[], void*);
MunitResult lda_absy (const MunitParameter[], void*);
MunitResult lda_indx (const MunitParameter[], void*);
MunitResult lda_indy (const MunitParameter[], void*);

MunitResult ldx_imm  (const MunitParameter[], void*);
MunitResult ldx_zp   (const MunitParameter[], void*);
MunitResult ldx_zpy  (const MunitParameter[], void*);
MunitResult ldx_abs  (const MunitParameter[], void*);
MunitResult ldx_absy (const MunitParameter[], void*);

MunitResult ldy_imm  (const MunitParameter[], void*);
MunitResult ldy_zp   (const MunitParameter[], void*);
MunitResult ldy_zpx  (const MunitParameter[], void*);
MunitResult ldy_abs  (const MunitParameter[], void*);
MunitResult ldy_absx (const MunitParameter[], void*);

MunitResult lsr_acc  (const MunitParameter[], void*);
MunitResult lsr_zp   (const MunitParameter[], void*);
MunitResult lsr_zpx  (const MunitParameter[], void*);
MunitResult lsr_abs  (const MunitParameter[], void*);
MunitResult lsr_absx (const MunitParameter[], void*);

MunitResult nop      (const MunitParameter[], void*);

MunitResult ora_imm  (const MunitParameter[], void*);
MunitResult ora_zp   (const MunitParameter[], void*);
MunitResult ora_zpx  (const MunitParameter[], void*);
MunitResult ora_abs  (const MunitParameter[], void*);
MunitResult ora_absx (const MunitParameter[], void*);
MunitResult ora_absy (const MunitParameter[], void*);
MunitResult ora_indx (const MunitParameter[], void*);
MunitResult ora_indy (const MunitParameter[], void*);

MunitResult pha_imp  (const MunitParameter[], void*);
MunitResult php_imp  (const MunitParameter[], void*);

MunitResult pla_imp  (const MunitParameter[], void*);
MunitResult plp_imp  (const MunitParameter[], void*);

MunitResult rol_acc  (const MunitParameter[], void*);
MunitResult rol_zp   (const MunitParameter[], void*);
MunitResult rol_zpx  (const MunitParameter[], void*);
MunitResult rol_abs  (const MunitParameter[], void*);
MunitResult rol_absx (const MunitParameter[], void*);

MunitResult ror_acc  (const MunitParameter[], void*);
MunitResult ror_zp   (const MunitParameter[], void*);
MunitResult ror_zpx  (const MunitParameter[], void*);
MunitResult ror_abs  (const MunitParameter[], void*);
MunitResult ror_absx (const MunitParameter[], void*);

MunitResult rti_imp  (const MunitParameter[], void*);
MunitResult rts_imp  (const MunitParameter[], void*);

MunitResult sbc_imm  (const MunitParameter[], void*);
MunitResult sbc_zp   (const MunitParameter[], void*);
MunitResult sbc_zpx  (const MunitParameter[], void*);
MunitResult sbc_abs  (const MunitParameter[], void*);
MunitResult sbc_absx (const MunitParameter[], void*);
MunitResult sbc_absy (const MunitParameter[], void*);
MunitResult sbc_indx (const MunitParameter[], void*);
MunitResult sbc_indy (const MunitParameter[], void*);

MunitResult sec_imp  (const MunitParameter[], void*);
MunitResult sed_imp  (const MunitParameter[], void*);
MunitResult sei_imp  (const MunitParameter[], void*);

MunitResult sta_zp   (const MunitParameter[], void*);
MunitResult sta_zpx  (const MunitParameter[], void*);
MunitResult sta_abs  (const MunitParameter[], void*);
MunitResult sta_absx (const MunitParameter[], void*);
MunitResult sta_absy (const MunitParameter[], void*);
MunitResult sta_indx (const MunitParameter[], void*);
MunitResult sta_indy (const MunitParameter[], void*);

MunitResult stx_zp   (const MunitParameter[], void*);
MunitResult stx_zpx  (const MunitParameter[], void*);
MunitResult stx_abs  (const MunitParameter[], void*);
MunitResult stx_absx (const MunitParameter[], void*);
MunitResult stx_absy (const MunitParameter[], void*);
MunitResult stx_indx (const MunitParameter[], void*);
MunitResult stx_indy (const MunitParameter[], void*);

MunitResult sty_zp   (const MunitParameter[], void*);
MunitResult sty_zpx  (const MunitParameter[], void*);
MunitResult sty_abs  (const MunitParameter[], void*);
MunitResult sty_absx (const MunitParameter[], void*);
MunitResult sty_absy (const MunitParameter[], void*);
MunitResult sty_indx (const MunitParameter[], void*);
MunitResult sty_indy (const MunitParameter[], void*);

MunitResult tax_imp  (const MunitParameter[], void*);
MunitResult tay_imp  (const MunitParameter[], void*);
MunitResult tsx_imp  (const MunitParameter[], void*);
MunitResult txa_imp  (const MunitParameter[], void*);
MunitResult txs_imp  (const MunitParameter[], void*);
MunitResult tya_imp  (const MunitParameter[], void*);

#endif
