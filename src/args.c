#include "6502emu/args.h"
#include <stdio.h>
#include <getopt.h>

args_t args_parse(int argc, char** argv) {
  args_t a = {0};
  char c;
  while ((c  = getopt(argc, argv, "f:d:")) != -1) {
    switch (c) {
      case 'f': {
        a.input_file = optarg;
      } break;
      case 'd': {
        a.dbg_file = optarg;
      } break;
      case '?': {
        fprintf(stderr, "Unknown option : %c\n", optopt);
      } break;
    }
  }

  return a;
}

void args_usage() {
  printf("Usage:\n");
  printf(" -f <filename>   : supply a valid ld65 binary file\n");
  printf(" -d <dbgfile>    : supply a valid ld65 dbgfile (--dbgfile in ld65)\n");
}
