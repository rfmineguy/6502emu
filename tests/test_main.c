#include "tests.h"

MunitTest adc_tests[] = {
  {"/imm", adc_imm, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zp",  adc_zp,  setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite intermediate[] = {
  {"/adc", adc_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

static const MunitSuite main_suite = {
  "testing",
  NULL, //tests,
  intermediate,
  1,
  MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, const char** argv) {
  return munit_suite_main(&main_suite, NULL, argc, (char *const *)argv);
}
