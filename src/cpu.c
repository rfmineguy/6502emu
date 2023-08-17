#include "cpu.h"
#include <stdio.h>
#include <assert.h>

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

opcode_e cpu_fetch(cpu_t* cpu) {
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
