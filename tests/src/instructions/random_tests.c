#include "tests.h"
#include <stdio.h>

MunitResult bit_shift_tests(const MunitParameter params[], void* fixture) {
  uint8_t arr[10] = {0x4, 0x5, 0x9, 0x8, 0x2, 0x54, 0x32, 0x0, 0x0, 0x0};

  arr[0] <<= 1;
  munit_assert_int(arr[0], ==, 0x8);

  arr[1] <<= 1;
  munit_assert_int(arr[1], ==, 0x0a);

  arr[2] <<= 1;
  munit_assert_int(arr[2], ==, 0x12);

 *(arr + 3) <<= 1;
  munit_assert_int(arr[3], ==, 0x10);

  *(arr + *(arr + 4)) <<= 1;
  munit_assert_int(arr[2], ==, 0x24);

  uint16_t v16 = *(uint16_t*)(arr + 5);
  munit_assert_int(v16, ==, 0x3254);    //little endian flips the bits when read

  uint8_t v8 = *(uint8_t*)(arr + 5);
  munit_assert_int(v8, ==, 0x54);

  *(uint8_t*)(arr + 5) <<= 1;
  v8 = *(uint8_t*)(arr + 5);
  munit_assert_int(v8, ==, 0xA8);

  return MUNIT_OK;
}
