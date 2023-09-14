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

#endif
