#include "tests.h"

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

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
  {"/imm", and_imm, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite intermediate[] = {
  {"/adc", adc_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/and", and_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
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
