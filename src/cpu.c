#include "6502emu/cpu.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

char* disassembled_instruction = NULL;

uint8_t* cpu_util_get_reg_ptr(register_e reg, cpu_t* cpu) {
  switch (reg) {
    case REG_A: return &cpu->regA;
    case REG_X: return &cpu->regX;
    case REG_Y: return &cpu->regY;
  }
}

void cpu_reset(cpu_t* cpu) {
  // read from the reset vector to set the PC
  cpu->pc = cpu->memory[0xFFFC];
  cpu->pc |= (cpu->memory[0xFFFD] << 8);
}

void cpu_dump(cpu_t* cpu) {
  printf("Dump:\n");
  printf("  A = %X\n", cpu->regA);
  printf("  X = %X\n", cpu->regX);
  printf("  Y = %X\n", cpu->regY);
  printf(" SP = %X\n", cpu->sp);
  printf(" PC = $%X\n", cpu->pc);
}

uint8_t cpu_fetch(cpu_t* cpu) {
  return cpu->memory[cpu->pc];
}

void cpu_reg_load(register_e reg, addressing_mode_e addr_mode, cpu_t* cpu) {
  uint8_t* reg_ptr = cpu_util_get_reg_ptr(reg, cpu);
  switch (addr_mode) {
    case AM_IMMEDIATE: 
      *reg_ptr = cpu->memory[cpu->pc + 1]; cpu->pc += 2; 
      break;
    case AM_ZP: { 
      uint8_t zp_addr = cpu->memory[cpu->pc + 1];
      *reg_ptr = cpu->memory[(uint16_t)zp_addr]; 
      cpu->pc += 2;
    } break;
    case AM_ZP_X:
      *reg_ptr = cpu->memory[cpu->pc + 1];
      cpu->pc += 2;
      break;
    case AM_ABS:
      *reg_ptr = (uint16_t) cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      break;
    default: assert(0 && "Unimplemented load addressing mode");
  }
}

void cpu_reg_store(register_e reg, addressing_mode_e addr_mode, cpu_t* cpu) {
  uint8_t* reg_ptr = cpu_util_get_reg_ptr(reg, cpu);
  switch (addr_mode) {
    case AM_ZP: {
      cpu->memory[cpu->pc + 1] = *reg_ptr; cpu->pc += 2;
    } break;
    default: assert(0 && "Unimplemented store addressing mode");
  }
}

int cpu_load_program(const char* filename, cpu_t* cpu) {
  FILE* f = fopen(filename, "rb");
  if (!f) {
    fprintf(stderr, "Failed to open file '%s'\n", filename);
    return 0;
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  fread(cpu->memory, sizeof(uint8_t), size, f);
  fclose(f);
  return 1;
}

int cpu_step(cpu_t* cpu) {
  opcode_e opcode = cpu_fetch(cpu);
  printf("opcode: 0x%X\n", opcode);
  switch (opcode) {
    case OP_EMU_EXIT:   return 0;

    case OP_STA_ZP:     cpu_reg_store(REG_A, AM_ZP       , cpu); return 1;
    case OP_STA_ZP_X:   cpu_reg_store(REG_A, AM_ZP_X     , cpu); return 1;
    case OP_STA_ABS:    cpu_reg_store(REG_A, AM_ABS      , cpu); return 1;
    case OP_STA_ABS_X:  cpu_reg_store(REG_A, AM_ABS_X    , cpu); return 1;
    case OP_STA_ABS_Y:  cpu_reg_store(REG_A, AM_ABS_Y    , cpu); return 1;
    case OP_STA_IND_X:  cpu_reg_store(REG_A, AM_IND_X    , cpu); return 1;
    case OP_STA_IND_Y:  cpu_reg_store(REG_A, AM_IND_Y    , cpu); return 1;

    case OP_LDA_IMM:    cpu_reg_load(REG_A, AM_IMMEDIATE, cpu); return 1;
    case OP_LDA_ZP:     cpu_reg_load(REG_A, AM_ZP       , cpu); return 1;
    case OP_LDA_ZP_X:   cpu_reg_load(REG_A, AM_ZP_X     , cpu); return 1;
    case OP_LDA_ABS:    cpu_reg_load(REG_A, AM_ABS      , cpu); return 1;
    case OP_LDA_ABS_X:  cpu_reg_load(REG_A, AM_ABS_X    , cpu); return 1;
    case OP_LDA_ABS_Y:  cpu_reg_load(REG_A, AM_ABS_Y    , cpu); return 1;
    case OP_LDA_IND_X:  cpu_reg_load(REG_A, AM_IND_X    , cpu); return 1;
    case OP_LDA_IND_Y:  cpu_reg_load(REG_A, AM_IND_Y    , cpu); return 1;
    default:
      printf("Unimplemented opcode, %d", opcode);
      return 0;
  }
}


// bit pattern aaabbbcc
//  aaa / cc -> opcode
//  bbb      -> addr mode


void cpu_get_str_rep(int index, cpu_t* cpu, char* out_str_rep, int out_str_size, int* byte_size) {
#define OUT_STR_CAT(str) strncat(out_str_rep, str, out_str_size)
  uint8_t byte  = cpu_fetch(cpu);
  uint8_t aaa   = byte >> 5;
  uint8_t bbb   = ((byte & 0b00011100) >> 2);
  uint8_t cc    = byte & 0b00000011;

  printf("byte   : 0x%02X\n", byte);
  printf("aaa    : %" PRIu8 "\n", aaa);
  printf("bbb    : %" PRIu8 "\n", bbb);
  printf("cc     : %" PRIu8 "\n", cc);

  out_str_rep[0] = 0;

  if (cc == 0b00000000) {
    switch (aaa) {
      case 0b00000001: OUT_STR_CAT("BIT "); break;
      case 0b00000010: OUT_STR_CAT("JMP "); break;
      case 0b00000011: OUT_STR_CAT("JMP "); break;
      case 0b00000100: OUT_STR_CAT("STY "); break;
      case 0b00000101: OUT_STR_CAT("LDY "); break;
      case 0b00000110: OUT_STR_CAT("CPY "); break;
      case 0b00000111: OUT_STR_CAT("CPX "); break;
    }
    *byte_size = 1; // each opcode is one byte
    switch (bbb) {
      case 0b00000000: OUT_STR_CAT("#immediate"); break;
      case 0b00000001: OUT_STR_CAT("zeropage"); break;
      case 0b00000011: OUT_STR_CAT("absolute"); break;
      case 0b00000101: OUT_STR_CAT("zeropage,X"); break;
      case 0b00000111: OUT_STR_CAT("absolute,X"); break;
    }
    return;         // we found the translation
  }
  else if (cc == 0b00000001) {
    switch (aaa) {
      case 0b00000000: OUT_STR_CAT("ORA "); break;
      case 0b00000001: OUT_STR_CAT("AND "); break;
      case 0b00000010: OUT_STR_CAT("EOR "); break;
      case 0b00000011: OUT_STR_CAT("ADC "); break;
      case 0b00000100: OUT_STR_CAT("STA "); break;
      case 0b00000101: OUT_STR_CAT("LDA "); break;
      case 0b00000110: OUT_STR_CAT("CMP "); break;
      case 0b00000111: OUT_STR_CAT("SBC "); break;
    }
    switch (bbb) {
      case 0b00000000: OUT_STR_CAT("(zeropage,X)\n"); break;
      case 0b00000001: OUT_STR_CAT("zeropage    \n"); break;
      case 0b00000010: OUT_STR_CAT("#immediate  \n"); break;
      case 0b00000011: OUT_STR_CAT("absolute    \n"); break;
      case 0b00000100: OUT_STR_CAT("(zeropage),Y\n"); break;
      case 0b00000101: OUT_STR_CAT("zeropage, X \n"); break;
      case 0b00000110: OUT_STR_CAT("absolute, Y \n"); break;
      case 0b00000111: OUT_STR_CAT("absolute, X \n"); break;
    }
    return;  // we found the translation
  }
  else if (cc == 0b00000010) {
    printf("Opcode:   ");
    switch (aaa) {
      case 0b00000000: OUT_STR_CAT("ASL "); break;
      case 0b00000001: OUT_STR_CAT("ROL "); break;
      case 0b00000010: OUT_STR_CAT("LSR "); break;
      case 0b00000011: OUT_STR_CAT("ROR "); break;
      case 0b00000100: OUT_STR_CAT("STX "); break;
      case 0b00000101: OUT_STR_CAT("LDX "); break;
      case 0b00000110: OUT_STR_CAT("DEC "); break;
      case 0b00000111: OUT_STR_CAT("INC "); break;
    }
    printf("AddrMode: ");
    switch (bbb) {
      case 0b00000000: OUT_STR_CAT("#immediate  \n"); break;
      case 0b00000001: OUT_STR_CAT("zeropage    \n"); break;
      case 0b00000010: OUT_STR_CAT("accumulator \n"); break;
      case 0b00000011: OUT_STR_CAT("absolute    \n"); break;
      case 0b00000101: OUT_STR_CAT("zeropage, X \n"); break;
      case 0b00000111: OUT_STR_CAT("absolute, X \n"); break;
    };
    // we found the translation
  }

  // https://llx.com/Neil/a2/opcodes.html
}
