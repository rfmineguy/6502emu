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
  cpu->loaded = 1;
  return 1;
}

int cpu_step(cpu_t* cpu) {
  instruction_t ins = cpu_get_instruction(cpu->pc, cpu);
  cpu->pc += ins.bytes;
}


// bit pattern aaabbbcc
//  aaa / cc -> opcode
//  bbb      -> addr mode

instruction_t cpu_get_instruction(int index, const cpu_t* cpu) {
#define STR_APPEND(str, fmt, ...) str += snprintf(str, 50, fmt, ##__VA_ARGS__)

  instruction_t ins = {0};
  ins.address = index;

  uint8_t byte  = cpu->memory[index];
  uint8_t aaa   = byte >> 5;
  uint8_t bbb   = ((byte & 0b00011100) >> 2);
  uint8_t cc    = byte & 0b00000011;

  ins.str[0]    = 0;
  char* str_rep = ins.str;
  switch (byte) {
    case 0x20: STR_APPEND(str_rep, "%s", "JSR"); ins.bytes = 3; break;

    case 0x10: STR_APPEND(str_rep, "%s", "BPL"); ins.bytes = 1; break;
    case 0x30: STR_APPEND(str_rep, "%s", "BMI"); ins.bytes = 1; break;
    case 0x50: STR_APPEND(str_rep, "%s", "BVC"); ins.bytes = 1; break;
    case 0x70: STR_APPEND(str_rep, "%s", "BVS"); ins.bytes = 1; break;
    case 0x90: STR_APPEND(str_rep, "%s", "BCC"); ins.bytes = 1; break;
    case 0xB0: STR_APPEND(str_rep, "%s", "BCS"); ins.bytes = 1; break;
    case 0xD0: STR_APPEND(str_rep, "%s", "BNE"); ins.bytes = 1; break;
    case 0xF0: STR_APPEND(str_rep, "%s", "BEQ"); ins.bytes = 1; break;

    case 0x00: STR_APPEND(str_rep, "%s", "BRK"); ins.bytes = 1; break;
    case 0x40: STR_APPEND(str_rep, "%s", "RTI"); ins.bytes = 1; break;
    case 0x60: STR_APPEND(str_rep, "%s", "RTS"); ins.bytes = 1; break;

    case 0x08: STR_APPEND(str_rep, "%s", "PHP"); ins.bytes = 1; break;
    case 0x28: STR_APPEND(str_rep, "%s", "PLP"); ins.bytes = 1; break;
    case 0x48: STR_APPEND(str_rep, "%s", "PHA"); ins.bytes = 1; break;
    case 0x68: STR_APPEND(str_rep, "%s", "PLA"); ins.bytes = 1; break;
    case 0x88: STR_APPEND(str_rep, "%s", "DEY"); ins.bytes = 1; break;
    case 0xA8: STR_APPEND(str_rep, "%s", "TAY"); ins.bytes = 1; break;
    case 0xC8: STR_APPEND(str_rep, "%s", "INY"); ins.bytes = 1; break;
    case 0xE8: STR_APPEND(str_rep, "%s", "INX"); ins.bytes = 1; break;

    case 0x18: STR_APPEND(str_rep, "%s", "CLC"); ins.bytes = 1; break;
    case 0x38: STR_APPEND(str_rep, "%s", "SEC"); ins.bytes = 1; break;
    case 0x58: STR_APPEND(str_rep, "%s", "CLI"); ins.bytes = 1; break;
    case 0x78: STR_APPEND(str_rep, "%s", "SEI"); ins.bytes = 1; break;
    case 0x98: STR_APPEND(str_rep, "%s", "TYA"); ins.bytes = 1; break;
    case 0xB8: STR_APPEND(str_rep, "%s", "CLV"); ins.bytes = 1; break;
    case 0xD8: STR_APPEND(str_rep, "%s", "CLD"); ins.bytes = 1; break;
    case 0xF8: STR_APPEND(str_rep, "%s", "SED"); ins.bytes = 1; break;

    case 0x8A: STR_APPEND(str_rep, "%s", "TXA"); ins.bytes = 1; break;
    case 0x9A: STR_APPEND(str_rep, "%s", "TXS"); ins.bytes = 1; break;
    case 0xAA: STR_APPEND(str_rep, "%s", "TAX"); ins.bytes = 1; break;
    case 0xBA: STR_APPEND(str_rep, "%s", "TSX"); ins.bytes = 1; break;
    case 0xCA: STR_APPEND(str_rep, "%s", "DEX"); ins.bytes = 1; break;
    case 0xEA: STR_APPEND(str_rep, "%s", "NOP"); ins.bytes = 1; break;
  }

  // multibyte instructions (only if we didn't find a single byte instruction first)
  if (ins.bytes == 0 && cc == 0b00000000) {
    switch (aaa) {
      case 0b00000001: STR_APPEND(str_rep, "%s", "BIT "); ins.bytes += 1; break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "JMP "); ins.bytes += 1; break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "JMP "); ins.bytes += 1; break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STY "); ins.bytes += 1; break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDY "); ins.bytes += 1; break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "CPY "); ins.bytes += 1; break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "CPX "); ins.bytes += 1; break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                         ins.bytes += 1; break; // #immediate
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                         ins.bytes += 1; break; // zeropage
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; break; // absolute
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                         ins.bytes += 1; break; // zeropage, X
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; break; // absolute, X
      
      default: break;
    }
  }
  else if (ins.bytes == 0 && cc == 0b00000001) {
    switch (aaa) {
      case 0b00000000: STR_APPEND(str_rep, "%s", "ORA "); ins.bytes += 1; break;
      case 0b00000001: STR_APPEND(str_rep, "%s", "AND "); ins.bytes += 1; break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "EOR "); ins.bytes += 1; break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "ADC "); ins.bytes += 1; break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STA "); ins.bytes += 1; break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDA "); ins.bytes += 1; break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "CMP "); ins.bytes += 1; break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "SBC "); ins.bytes += 1; break;

      default: break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "($%02X, X)"  , cpu->memory[index + 1]);                           ins.bytes += 1; break; // (zeropage, X)
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                           ins.bytes += 1; break; // zeropage
      case 0b00000010: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                           ins.bytes += 1; break; // #immediate
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; break; // absolute
      case 0b00000100: STR_APPEND(str_rep, "($%02X), Y"  , cpu->memory[index + 1]);                           ins.bytes += 2; break; // (zeropage, Y)
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                           ins.bytes += 1; break; // zeropage, X
      case 0b00000110: STR_APPEND(str_rep, "$%02X%02X, Y", cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; break; // absolute, Y
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; break; // absolute, X
      
      default: break;
    }
  }
  // Group 2
  else if (ins.bytes == 0 && cc == 0b00000010) {
    switch (aaa) {
      case 0b00000000: STR_APPEND(str_rep, "%s", "ASL "); break;
      case 0b00000001: STR_APPEND(str_rep, "%s", "ROL "); break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "LSR "); break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "ROR "); break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STX "); break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDX "); break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "DEC "); break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "INC "); break;

      default: break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                         ins.bytes += 1; break; // #immediate
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                         ins.bytes += 1; break; // zeropage
      case 0b00000010: STR_APPEND(str_rep, "");                                                             ins.bytes += 0; break; // accumulator
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; break; // absolute
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                         ins.bytes += 1; break; // zeropage, X
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; break; // absolute, X
      
      default: break;
    };
    // we found the translation
  }
  ins.valid = ins.bytes > 0;
  return ins;
}

int cpu_get_str_rep(int index, cpu_t* cpu, char* out_str_rep, int out_str_size, int* out_byte_size) {
#define OUT_STR_APPEND(fmt, ...) out_str_rep += snprintf(out_str_rep, out_str_size, fmt, ##__VA_ARGS__)
  uint8_t byte  = cpu->memory[index];
  uint8_t aaa   = byte >> 5;
  uint8_t bbb   = ((byte & 0b00011100) >> 2);
  uint8_t cc    = byte & 0b00000011;

  out_str_rep[0] = 0;

  // STY $00
  // ORA $3224, X
  const char* opcode_str = NULL;  // string rep of the opcode
  int bytes = 0;                  // number of bytes for the opcode

  // single-byte instructions
  switch (byte) {
    case 0x20: OUT_STR_APPEND("%s $%02X%02X", "JSR", cpu->memory[index + 2], cpu->memory[index + 1]); bytes += 3; break; // absolute
    
    case 0x10: OUT_STR_APPEND("%s", "BPL"); bytes += 1; break;
    case 0x30: OUT_STR_APPEND("%s", "BMI"); bytes += 1; break;
    case 0x50: OUT_STR_APPEND("%s", "BVC"); bytes += 1; break;
    case 0x70: OUT_STR_APPEND("%s", "BVS"); bytes += 1; break;
    case 0x90: OUT_STR_APPEND("%s", "BCC"); bytes += 1; break;
    case 0xB0: OUT_STR_APPEND("%s", "BCS"); bytes += 1; break;
    case 0xD0: OUT_STR_APPEND("%s", "BNE"); bytes += 1; break;
    case 0xF0: OUT_STR_APPEND("%s", "BEQ"); bytes += 1; break;

    case 0x00: OUT_STR_APPEND("%s", "BRK"); bytes += 1; break;
    case 0x40: OUT_STR_APPEND("%s", "RTI"); bytes += 1; break;
    case 0x60: OUT_STR_APPEND("%s", "RTS"); bytes += 1; break;

    case 0x08: OUT_STR_APPEND("%s", "PHP"); bytes += 1; break;
    case 0x28: OUT_STR_APPEND("%s", "PLP"); bytes += 1; break;
    case 0x48: OUT_STR_APPEND("%s", "PHA"); bytes += 1; break;
    case 0x68: OUT_STR_APPEND("%s", "PLA"); bytes += 1; break;
    case 0x88: OUT_STR_APPEND("%s", "DEY"); bytes += 1; break;
    case 0xA8: OUT_STR_APPEND("%s", "TAY"); bytes += 1; break;
    case 0xC8: OUT_STR_APPEND("%s", "INY"); bytes += 1; break;
    case 0xE8: OUT_STR_APPEND("%s", "INX"); bytes += 1; break;

    case 0x18: OUT_STR_APPEND("%s", "CLC"); bytes += 1; break;
    case 0x38: OUT_STR_APPEND("%s", "SEC"); bytes += 1; break;
    case 0x58: OUT_STR_APPEND("%s", "CLI"); bytes += 1; break;
    case 0x78: OUT_STR_APPEND("%s", "SEI"); bytes += 1; break;
    case 0x98: OUT_STR_APPEND("%s", "TYA"); bytes += 1; break;
    case 0xB8: OUT_STR_APPEND("%s", "CLV"); bytes += 1; break;
    case 0xD8: OUT_STR_APPEND("%s", "CLD"); bytes += 1; break;
    case 0xF8: OUT_STR_APPEND("%s", "SED"); bytes += 1; break;

    case 0x8A: OUT_STR_APPEND("%s", "TXA"); bytes += 1; break;
    case 0x9A: OUT_STR_APPEND("%s", "TXS"); bytes += 1; break;
    case 0xAA: OUT_STR_APPEND("%s", "TAX"); bytes += 1; break;
    case 0xBA: OUT_STR_APPEND("%s", "TSX"); bytes += 1; break;
    case 0xCA: OUT_STR_APPEND("%s", "DEX"); bytes += 1; break;
    case 0xEA: OUT_STR_APPEND("%s", "NOP"); bytes += 1; break;

    default: break;
  }

  // multibyte instructions (only if we didn't find a single byte instruction first)
  if (bytes == 0 && cc == 0b00000000) {
    switch (aaa) {
      case 0b00000001: OUT_STR_APPEND("%s", "BIT "); break;
      case 0b00000010: OUT_STR_APPEND("%s", "JMP "); break;
      case 0b00000011: OUT_STR_APPEND("%s", "JMP "); break;
      case 0b00000100: OUT_STR_APPEND("%s", "STY "); break;
      case 0b00000101: OUT_STR_APPEND("%s", "LDY "); break;
      case 0b00000110: OUT_STR_APPEND("%s", "CPY "); break;
      case 0b00000111: OUT_STR_APPEND("%s", "CPX "); break;

      default: break;
    }
    bytes = 1; // each opcode is one byte
    switch (bbb) {
      case 0b00000000: OUT_STR_APPEND("#$%02X"      , cpu->memory[index + 1]);                           bytes += 1; break; // #immediate
      case 0b00000001: OUT_STR_APPEND("$%02X"       , cpu->memory[index + 1]);                           bytes += 1; break; // zeropage
      case 0b00000011: OUT_STR_APPEND("$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]);   bytes += 2; break; // absolute
      case 0b00000101: OUT_STR_APPEND("$%02X, X"    , cpu->memory[index + 1]);                           bytes += 1; break; // zeropage, X
      case 0b00000111: OUT_STR_APPEND("$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]);   bytes += 2; break; // absolute, X
      
      default: break;
    }
  }
  else if (bytes == 0 && cc == 0b00000001) {
    switch (aaa) {
      case 0b00000000: OUT_STR_APPEND("%s", "ORA "); break;
      case 0b00000001: OUT_STR_APPEND("%s", "AND "); break;
      case 0b00000010: OUT_STR_APPEND("%s", "EOR "); break;
      case 0b00000011: OUT_STR_APPEND("%s", "ADC "); break;
      case 0b00000100: OUT_STR_APPEND("%s", "STA "); break;
      case 0b00000101: OUT_STR_APPEND("%s", "LDA "); break;
      case 0b00000110: OUT_STR_APPEND("%s", "CMP "); break;
      case 0b00000111: OUT_STR_APPEND("%s", "SBC "); break;

      default: break;
    }
    bytes = 1;
    switch (bbb) {
      case 0b00000000: OUT_STR_APPEND("($%02X, X)"  , cpu->memory[index + 1]);                           bytes += 1; break; // (zeropage, X)
      case 0b00000001: OUT_STR_APPEND("$%02X"       , cpu->memory[index + 1]);                           bytes += 1; break; // zeropage
      case 0b00000010: OUT_STR_APPEND("#$%02X"      , cpu->memory[index + 1]);                           bytes += 1; break; // #immediate
      case 0b00000011: OUT_STR_APPEND("$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]);   bytes += 2; break; // absolute
      case 0b00000100: OUT_STR_APPEND("($%02X), Y"  , cpu->memory[index + 1]);                           bytes += 2; break; // (zeropage, Y)
      case 0b00000101: OUT_STR_APPEND("$%02X, X"    , cpu->memory[index + 1]);                           bytes += 1; break; // zeropage, X
      case 0b00000110: OUT_STR_APPEND("$%02X%02X, Y", cpu->memory[index + 2], cpu->memory[index + 1]);   bytes += 2; break; // absolute, Y
      case 0b00000111: OUT_STR_APPEND("$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]);   bytes += 2; break; // absolute, X
      
      default: break;
    }
  }
  // Group 2
  else if (bytes == 0 && cc == 0b00000010) {
    switch (aaa) {
      case 0b00000000: OUT_STR_APPEND("%s", "ASL "); break;
      case 0b00000001: OUT_STR_APPEND("%s", "ROL "); break;
      case 0b00000010: OUT_STR_APPEND("%s", "LSR "); break;
      case 0b00000011: OUT_STR_APPEND("%s", "ROR "); break;
      case 0b00000100: OUT_STR_APPEND("%s", "STX "); break;
      case 0b00000101: OUT_STR_APPEND("%s", "LDX "); break;
      case 0b00000110: OUT_STR_APPEND("%s", "DEC "); break;
      case 0b00000111: OUT_STR_APPEND("%s", "INC "); break;

      default: break;
    }
    bytes = 1;
    switch (bbb) {
      case 0b00000000:  OUT_STR_APPEND("#$%02X"      , cpu->memory[index + 1]);                         bytes += 1; break; // #immediate
      case 0b00000001:  OUT_STR_APPEND("$%02X"       , cpu->memory[index + 1]);                         bytes += 1; break; // zeropage
      case 0b00000010:  OUT_STR_APPEND("");                                                             bytes += 0; break; // accumulator
      case 0b00000011:  OUT_STR_APPEND("$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); bytes += 2; break; // absolute
      case 0b00000101:  OUT_STR_APPEND("$%02X, X"    , cpu->memory[index + 1]);                         bytes += 1; break; // zeropage, X
      case 0b00000111:  OUT_STR_APPEND("$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); bytes += 2; break; // absolute, X
      
      default: break;
    };
    // we found the translation
  }

  *out_byte_size = bytes;  // https://llx.com/Neil/a2/opcodes.html
  return bytes != 0;
}

int cpu_is_instruction_in_range(instruction_t ins, int lower_addr, int upper_addr) {
  return ins.address > lower_addr && ins.address < upper_addr;
}
