#include "6502emu/cpu.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

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

void cpu_get_str_rep(int index, cpu_t* cpu) {
  uint8_t byte  = cpu_fetch(cpu);
  uint8_t aaa   = byte >> 5;
  uint8_t bbb   = ((byte & 0b00011100) >> 2);
  uint8_t cc    = byte & 0b00000011;

  printf("aaa    : %" PRIu8 "\n", aaa);
  printf("bbb    : %" PRIu8 "\n", bbb);
  printf("cc     : %" PRIu8 "\n", cc);

  if (cc == 0b0000000) {
    switch (aaa) {
      case 0b00000001: printf("BIT\n"); break;
      case 0b00000010: printf("JMP\n"); break;
      case 0b00000011: printf("JMP (abs)\n"); break;
      case 0b00000100: printf("STY\n"); break;
      case 0b00000101: printf("LDY\n"); break;
      case 0b00000110: printf("CPY\n"); break;
      case 0b00000111: printf("CPX\n"); break;
    }
    switch (bbb) {
      case 0b00000001: printf("#immediate\n"); break;
      case 0b00000010: printf("zeropage\n"); break;
      case 0b00000011: printf("absolute\n"); break;
      case 0b00000101: printf("zeropage,X\n"); break;
      case 0b00000111: printf("absolute,X\n"); break;
    }
  }
  else if (cc == 0b00000001) {
    switch (aaa) {
      case 0b00000000: printf("ORA\n"); break;;
      case 0b00000001: printf("AND\n"); break;
      case 0b00000010: printf("EOR\n"); break;
      case 0b00000011: printf("ADC\n"); break;
      case 0b00000100: printf("STA\n"); break;
      case 0b00000101: printf("LDA\n"); break;
      case 0b00000110: printf("CMP\n"); break;
      case 0b00000111: printf("SBC\n"); break;
    }
    switch (bbb) {
      case 0b00000000: printf("(zeropage,X)\n"); break;
      case 0b00000001: printf("zeropage    \n"); break;
      case 0b00000010: printf("#immediate  \n"); break;
      case 0b00000011: printf("absolute    \n"); break;
      case 0b00000100: printf("(zeropage),Y\n"); break;
      case 0b00000101: printf("zeropage, X \n"); break;
      case 0b00000110: printf("absolute, Y \n"); break;
      case 0b00000111: printf("absolute, X \n"); break;
    }
  }
  else if (cc == 0b00000010) {
    switch (aaa) {
      case 0b00000000: printf("ASL\n");
      case 0b00000001: printf("ROL\n"); break;
      case 0b00000010: printf("LSR\n"); break;
      case 0b00000011: printf("ROR\n"); break;
      case 0b00000100: printf("STX\n"); break;
      case 0b00000101: printf("LDX\n"); break;
      case 0b00000110: printf("DEC\n"); break;
      case 0b00000111: printf("INC\n"); break;
    }
    switch (bbb) {
      case 0b00000000: printf("#immediate  \n"); break;
      case 0b00000001: printf("zeropage    \n"); break;
      case 0b00000010: printf("accumulator \n"); break;
      case 0b00000011: printf("absolute    \n"); break;
      case 0b00000101: printf("zeropage, X \n"); break;
      case 0b00000111: printf("absolute, X \n"); break;
    }
  }

  // https://llx.com/Neil/a2/opcodes.html
}
