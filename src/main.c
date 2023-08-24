#include <stdio.h>
#include "6502emu/cpu.h"
#include "6502emu/dbginfo.h"
#include "6502emu/args.h"

void cc65_err_callback(const cc65_parseerror* err) {
  fprintf(stderr, "cc65_read_dbginfo error\n");
  fprintf(stderr, "  severity: %d\n", err->type);
  fprintf(stderr, "  file    : %s\n", err->name);
  fprintf(stderr, "  line    : %d\n", err->line);
  fprintf(stderr, "  msg     : %s\n", err->errormsg);
}

int main(int argc, char** argv) {
  cpu_t cpu;        // NOTE: Intentionally uninitialized

  args_t args = args_parse(argc, argv);
  if (!args.input_file) {
    fprintf(stderr, "Must supply file\n");
    return 0;
  }
  if (!args.dbg_file) {
    fprintf(stderr, "Must provide a debug file\n");
    return 0;
  }
  if (!cpu_load_program(args.input_file, &cpu)) {
    fprintf(stderr, "File doesn't exist\n");
    return 0;
  }

  printf("Parsing dbg file\n");
  cc65_dbginfo dbg = cc65_read_dbginfo(args.dbg_file, &cc65_err_callback);

  const cc65_segmentinfo* segments = cc65_get_segmentlist(dbg);
  for (int i = 0; i < segments->count; i++) {
    printf("segment #%d: { name: %s, start: %d, size: %d }\n", i, segments->data[i].segment_name, segments->data[i].segment_start, segments->data[i].segment_size);
  }

  cc65_free_dbginfo(dbg);
  return 0;

  /*
  // Setup reset vector
  cpu.memory[0xFFFC] = 0x00;
  cpu.memory[0xFFFD] = 0x10;

  // lda #45
  cpu.memory[0x1000] = OP_LDA_IMM;
  cpu.memory[0x1001] = 0x49;

  // sta $45 to $00
  cpu.memory[0x1002] = OP_STA_ZP;
  cpu.memory[0x1003] = 0x00;

  // lda $00
  cpu.memory[0x1004] = OP_LDA_ZP;
  cpu.memory[0x1005] = 0x00;
  */

  cpu_reset(&cpu);
  printf("CPU RESET\n");

  for (int i = 0x8000; i < 0x8010;) {
    instruction_t ins = cpu_get_instruction(i, &cpu);
    printf("str:     %s\n", ins.str);
    printf("raw:     %02X%02X%02X\n", ins.raw[0], ins.raw[1], ins.raw[2]);
    printf("bytes:   %d\n", ins.bytes);
    printf("address: %02X%02X\n", (ins.address >> 8) & 0xFF, ins.address & 0xFF);
    printf("valid:   %d\n", ins.valid);
    printf("is_bp:   %d\n", ins.is_bp);
    i += ins.bytes;
  }
  // char str_rep[255];
  // int byte_size;
  // for (int i = 0x8000; i < 0x8010;) {
  //   if (cpu_get_str_rep(i, &cpu, str_rep, 255, &byte_size)) {
  //     printf("%02X%02X   %s\n", (i >> 8) & 0xFF, i & 0xFF, str_rep);
  //     i += byte_size;
  //     continue;
  //   }
  //   else {
  //     printf("%02X%02X   invalid\n", (i >> 8) & 0xFF, i & 0xFF);
  //     i++;
  //   }
  // }
  // do {
  //   cpu_get_str_rep(cpu.pc, &cpu, str_rep, 255, &byte_size);
  //   printf("str_rep   : %s\n", str_rep);
  //   printf("byte_size : %d\n", byte_size);
  //   // cpu_dump(&cpu);
  //   printf("Press any button to step forward\n");
  //   fflush(stdin);
  //   getchar();
  // } while (cpu_step(&cpu));
}
