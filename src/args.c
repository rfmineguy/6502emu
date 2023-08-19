#include "6502emu/args.h"
#include <stdio.h>
#include <getopt.h>

args_t args_parse(int argc, char** argv) {
  args_t a = {0};
  char c;
  while ((c  = getopt(argc, argv, "f:")) != -1) {
    switch (c) {
      case 'f': {
        a.input_file = optarg;
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
  printf(" -f <filename>   : supply a valid 6502 binary file\n");
}
