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

instruction_t cpu_step(cpu_t* cpu) {
  instruction_t ins = cpu_get_instruction(cpu->pc, cpu);
  cpu->pc += ins.bytes;
  return ins;
}

uint8_t cpu_fetch(cpu_t* cpu) {
  return cpu->memory[cpu->pc];
}

void cpu_execute(cpu_t* cpu, instruction_t ins) {
  uint8_t cycles = 0;
  switch (ins.inst) {
  case INS_ADC: cpu_adc(cpu, ins); break;
  case INS_AND: cpu_and(cpu, ins); break;
  case INS_ASL: cpu_asl(cpu, ins); break; 
  case INS_BCC: assert(0 && "Not implmented"); break;
  case INS_BCS: assert(0 && "Not implmented"); break;
  case INS_BEQ: assert(0 && "Not implmented"); break;
  case INS_BIT: assert(0 && "Not implmented"); break;
  case INS_BMI: assert(0 && "Not implmented"); break;
  case INS_BNE: assert(0 && "Not implmented"); break;
  case INS_BPL: assert(0 && "Not implmented"); break;
  case INS_BRK: assert(0 && "Not implmented"); break;
  case INS_BVC: assert(0 && "Not implmented"); break;
  case INS_BVS: assert(0 && "Not implmented"); break;
  case INS_CLC: cpu->status_flags &= ~(SF_CARRY);          break; //clear the carry bit
  case INS_CLD: cpu->status_flags &= ~(SF_DECIMAL);        break; //clear the decimal bit
  case INS_CLI: cpu->status_flags &= ~(SF_INTDISA);        break; //clear the interupt disable bit
  case INS_CLV: cpu->status_flags &= ~(SF_OVERFLOW);       break; //clear the overflow bit
  case INS_CMP: cpu_cmp(cpu, ins, &cpu->regA);             break;
  case INS_CPX: cpu_cmp(cpu, ins, &cpu->regX);             break;
  case INS_CPY: cpu_cmp(cpu, ins, &cpu->regX);             break;
  case INS_DEC: cpu_dec_mem(cpu, ins); break;
  case INS_DEX: cpu->regX--; 
                cpu->regX == 0 ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regX  < 0 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INS_DEX
  case INS_DEY: cpu->regY--;
                cpu->regY == 0 ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regY  < 0 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INS_DEY
  case INS_EOR: assert(0 && "Not implemented"); break;
  case INS_INC: cpu_inc_mem(cpu, ins); break;
  case INS_INX: cpu->regX++;
                cpu->regX == 0 ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regX  < 0 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INC_INX
  case INS_INY: cpu->regY++;
                cpu->regY == 0 ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regY  < 0 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INS_INY
  case INS_JMP: cpu_jmp(cpu, ins); break;
  case INS_JSR: assert(0 && "Not implemented"); break;
  case INS_LDA: cpu_lda(cpu, ins); break;
  case INS_LDX: cpu_ldx(cpu, ins); break;
  case INS_LDY: cpu_ldy(cpu, ins); break;
  case INS_LSR: assert(0 && "Not implemented"); break;
  case INS_NOP: /* nothing TODO: enter cycle count */ break;
  case INS_ORA: cpu_ora(cpu, ins); break;
  case INS_PHA: assert(0 && "Not implemented"); break;
  case INS_PHP: assert(0 && "Not implemented"); break;
  case INS_PLA: assert(0 && "Not implemented"); break;
  case INS_PLP: assert(0 && "Not implemented"); break;
  case INS_ROL: assert(0 && "Not implemented"); break;
  case INS_ROR: assert(0 && "Not implemented"); break;
  case INS_RTI: assert(0 && "Not implemented"); break;
  case INS_RTS: assert(0 && "Not implemented"); break;
  case INS_SBC: assert(0 && "Not implemented"); break;
  case INS_SEC: cpu->status_flags |= SF_CARRY;   break;
  case INS_SED: cpu->status_flags |= SF_DECIMAL; break;
  case INS_SEI: cpu->status_flags |= SF_INTDISA; break;
  case INS_STA: cpu_sta(cpu, ins); break;
  case INS_STX: cpu_stx(cpu, ins); break;
  case INS_STY: cpu_sty(cpu, ins); break;
  case INS_TAX: assert(0 && "Not implemented"); break;
  case INS_TAY: assert(0 && "Not implemented"); break;
  case INS_TSX: assert(0 && "Not implemented"); break;
  case INS_TXA: assert(0 && "Not implemented"); break;
  case INS_TXS: assert(0 && "Not implemented"); break;
  case INS_TYA: assert(0 && "Not implemented"); break;
  }
}

void cpu_adc(cpu_t* cpu, instruction_t ins) {
  uint16_t r = 0;
  switch (ins.am) {
    case AM_IMMEDIATE: r = cpu->regA + ins.raw[1]                                                 + (cpu->status_flags & SF_CARRY); break;
    case AM_ZP:        r = cpu->regA + cpu->memory[(uint16_t)((uint8_t)ins.raw[1])]               + (cpu->status_flags & SF_CARRY); break;
    case AM_ZP_X:      r = cpu->regA + cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]   + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS:       r = cpu->regA + cpu->memory[(uint16_t)ins.raw[1]                       ]   + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS_X:     r = cpu->regA + cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regX)] + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS_Y:     r = cpu->regA + cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regY)] + (cpu->status_flags & SF_CARRY); break;
    case AM_IND_X:     assert(0 && "ADC (indirect, X) not supported"); break;
    case AM_IND_Y:     assert(0 && "ADC (indirect, Y) not supported"); break;
    default:           assert(0 && "Fatal default"); break;
  }
  cpu->regA = r;

  // Set/Clear the appropriate status flags
  (r & 0b10000000)               ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
  cpu->regA == 0                 ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  r > 0xff                       ? (cpu->status_flags |= SF_OVERFLOW) : (cpu->status_flags &= ~(SF_OVERFLOW));
  r & 0x100                      ? (cpu->status_flags |= SF_CARRY)    : (cpu->status_flags &= ~(SF_CARRY));
}

void cpu_and(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regA &= ins.raw[1]; break;
    case AM_ZP:        cpu->regA &= cpu->memory[*(uint8_t*)  (ins.raw + 1)            ]; break;
    case AM_ZP_X:      cpu->regA &= cpu->memory[*(uint8_t*)  (ins.raw + 1) + cpu->regX]; break;
    case AM_ABS:       cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1)            ]; break;
    case AM_ABS_X:     cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1) + cpu->regX]; break;
    case AM_ABS_Y:     cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1) + cpu->regY]; break;
    case AM_IND_X:     assert(0 && "AND (indirect, X) doesn't exist"); break;
    case AM_IND_Y:     assert(0 && "AND (indirect, Y) doesn't exist"); break;
    default:           assert(0 && "Fatal default");
  }

  cpu->regA == 0              ? (cpu->status_flags |= SF_ZERO) : (cpu->status_flags &= ~(SF_ZERO));
  (cpu->regA & 0b10000000)    ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_asl(cpu_t* cpu, instruction_t ins) {
  // Deal with carry bit
  switch (ins.am) {
    case AM_ACCUMULATOR: (cpu->regA & 0b10000000)                                       ? (cpu->status_flags |= SF_CARRY) : (cpu->status_flags &= ~(SF_CARRY)); break;
    case AM_ZP:          cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1))] & 0b10000000 ? (cpu->status_flags |= SF_CARRY) : (cpu->status_flags &= ~(SF_CARRY)); break;
    case AM_ZP_X:        break;
    case AM_ABS:         break;
    case AM_ABS_X:       break;
    default:             assert(0 && "Fatal default");
  }
  // Deal with shift
  uint8_t value;  // value that resulted
  switch (ins.am) {
    case AM_ACCUMULATOR: value = (cpu->regA <<= 1);                                                      break;
    case AM_ZP:          value = (cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1))] <<= 1);              break;
    case AM_ZP_X:        value = (cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1) + cpu->regX)] <<= 1);  break;
    case AM_ABS:         value = (cpu->memory[         (*(uint16_t*)(ins.raw + 1))] <<= 1);              break;
    case AM_ABS_X:       value = (cpu->memory[         (*(uint16_t*)(ins.raw + 1) + cpu->regX)] <<= 1);  break;
    default:             assert(0 && "Fatal default");
  }
  // printf("asl ;: %2X\n", cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1))]);
  value == 0              ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  value &  0b10000000     ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_jmp(cpu_t* cpu, instruction_t ins) {
  printf("jmp addr = " PRIu16 "\n", (uint16_t) ins.raw[1]);
  switch (ins.am) {
    case AM_ABS:     cpu->pc = (uint16_t) ins.raw[1]; break;
    case AM_IND:     cpu->pc = (uint16_t) cpu->memory[(uint16_t)ins.raw[1]]; break;  // NOTE: There is a catch here. Look into it.
  }
}

void cpu_cmp(cpu_t* cpu, instruction_t ins, uint8_t* reg) {
  int r;
  switch (ins.am) {
    case AM_IMMEDIATE: r = *reg - ins.raw[1];                                               break;
    case AM_ZP:        r = *reg - cpu->memory[(uint16_t)((uint8_t)ins.raw[1])];             break;
    case AM_ZP_X:      r = *reg - cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]; break;
    case AM_ZP_Y:      /**/                                                                 break;
    case AM_ABS:       r = *reg - cpu->memory[(uint16_t)ins.raw[1]];                        break;
    case AM_ABS_X:     r = *reg - cpu->memory[(uint16_t)ins.raw[1] + cpu->regX];            break;
    case AM_ABS_Y:     r = *reg - cpu->memory[(uint16_t)ins.raw[1] + cpu->regY];            break;
    case AM_IND_X:     assert(0 && "CMP (indirect, X) not implemented");                    break;
    case AM_IND_Y:     assert(0 && "CMP (indirect, Y) not implemented");                    break;
    case AM_RELATIVE:  /**/                                                                 break;
    case AM_IMPLIED:   /**/                                                                 break;
    default:           assert(0 && "Fatal default");
  }
  // Set/Clear the appropriate status flags
  (r > 0)          ? (cpu->status_flags |= SF_CARRY   ) : (cpu->status_flags &= ~(SF_CARRY));
  (r == 0)         ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
  (r & 0b10000000) ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_dec_mem(cpu_t* cpu, instruction_t ins) {
  uint8_t* value_ptr = 0x0;
  switch (ins.am) {
    case AM_ZP:    value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1])];             break;
    case AM_ZP_X:  value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1] + cpu->regX)]; break;
    case AM_ABS:   value_ptr = &cpu->memory[(uint16_t)ins.raw[1]];                        break;
    case AM_ABS_X: value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1] + cpu->regX)]; break;
    default:           assert(0 && "Fatal default");
  }
  (*value_ptr) --;
  (*value_ptr == 0)         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  (*value_ptr & 0b10000000) ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_inc_mem(cpu_t* cpu, instruction_t ins) {
  uint8_t* value_ptr = 0x0;
  switch (ins.am) {
    case AM_ZP:    value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1])];             break;
    case AM_ZP_X:  value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1] + cpu->regX)]; break;
    case AM_ABS:   value_ptr = &cpu->memory[(uint16_t)ins.raw[1]];                        break;
    case AM_ABS_X: value_ptr = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1] + cpu->regX)]; break;
    default:           assert(0 && "Fatal default");
  }
  (*value_ptr) --;
  (*value_ptr == 0)         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  (*value_ptr & 0b10000000) ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_lda(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regA = ins.raw[1]; break;
    case AM_ZP:        cpu->regA = cpu->memory[(uint16_t)(ins.raw[1])];             break;
    case AM_ZP_X:      cpu->regA = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regX)]; break; 
    case AM_ZP_Y:      cpu->regA = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regY)]; break;
    case AM_ABS:       cpu->regA = cpu->memory[(uint16_t)ins.raw[1]];               break;
    case AM_ABS_X:     cpu->regA = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regX)]; break;
    case AM_ABS_Y:     cpu->regA = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regY)]; break;
    case AM_IND_X:     assert(0 && "LDA (indirect, X) not implemented");            break;
    case AM_IND_Y:     assert(0 && "LDA (indirect, Y) not implemented");            break;
    default:           assert(0 && "Fatal default");
  }
  (cpu->regA == 0)          ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
  (cpu->regA & 0b10000000)  ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_ldx(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regX = ins.raw[1]; break;
    case AM_ZP:        cpu->regX = cpu->memory[(uint16_t)(ins.raw[1])];             break;
    case AM_ZP_Y:      cpu->regX = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regY)]; break;
    case AM_ABS:       cpu->regX = cpu->memory[(uint16_t)ins.raw[1]];               break;
    case AM_ABS_Y:     cpu->regX = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regY)]; break;
    default:           assert(0 && "Fatal default");
  }
  (cpu->regX == 0)          ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
  (cpu->regX & 0b10000000)  ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_ldy(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regY = ins.raw[1]; break;
    case AM_ZP:        cpu->regY = cpu->memory[(uint8_t)ins.raw[1]];             break;
    case AM_ZP_X:      cpu->regY = cpu->memory[(uint8_t)(ins.raw[1] + cpu->regX)]; break;
    case AM_ABS:       cpu->regY = cpu->memory[(uint16_t)ins.raw[1]];               break;
    case AM_ABS_X:     cpu->regY = cpu->memory[(uint16_t)(ins.raw[1] + cpu->regX)]; break;
    default:           assert(0 && "Fatal default");
  }
  (cpu->regY == 0)          ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
  (cpu->regY & 0b10000000)  ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_sta(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ZP:    cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))] = cpu->regA; break;
    case AM_ZP_X:  cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regX))] = cpu->regA; break;
    case AM_ABS:   cpu->memory[(uint16_t)ins.raw[1]]                          = cpu->regA; break;
    case AM_ABS_X: cpu->memory[(uint16_t)(ins.raw[1] + cpu->regX)]            = cpu->regA; break;
    case AM_ABS_Y: cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]))]             = cpu->regA; break;
    case AM_IND_X: assert(0 && "STA (indirect, X) not implemented");                       break;
    case AM_IND_Y: assert(0 && "STA (indirect, Y) not implemented");                       break;
    default:           assert(0 && "Fatal default");
  }
}

void cpu_stx(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ZP:    cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))] = cpu->regX; break;
    case AM_ZP_Y:  cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regY))] = cpu->regX; break;
    case AM_ABS:   cpu->memory[(uint16_t)ins.raw[1]]                          = cpu->regX; break;
    default:           assert(0 && "Fatal default");
  }
}

void cpu_sty(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ZP:    cpu->regY = cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))]; break;
    case AM_ZP_X:  cpu->regY = cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regX))]; break;
    case AM_ABS:   cpu->regY = cpu->memory[(uint16_t)ins.raw[1]];                          break;
    default:           assert(0 && "Fatal default");
  }
}

void cpu_ora(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE:  cpu->regA |= ins.raw[1]; break;
    case AM_ZP:         cpu->regA |= cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))]; break;
    case AM_ZP_X:       cpu->regA |= cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regX))]; break;
    case AM_ABS:        cpu->regA |= cpu->memory[(uint16_t)ins.raw[1]];                          break;
    case AM_ABS_X:      cpu->regA |= cpu->memory[(uint16_t)(ins.raw[1] + cpu->regX)];            break;
    case AM_ABS_Y:      cpu->regA |= cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]))];             break;
    case AM_IND_X:      assert(0 && "STA (indirect, X) not implemented");                       break;
    case AM_IND_Y:      assert(0 && "STA (indirect, Y) not implemented");                       break;
  }
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
    case 0x0A: STR_APPEND(str_rep, "%s", "ASL A"); ins.bytes = 1; ins.inst = INS_ASL; ins.am = AM_ACCUMULATOR; break;

    case 0x20: STR_APPEND(str_rep, "%s", "JSR"); ins.bytes = 3; ins.inst = INS_JSR; ins.am = AM_ABS; break;

    case 0x10: STR_APPEND(str_rep, "%s", "BPL"); ins.bytes = 1; ins.inst = INS_BPL; ins.am = AM_RELATIVE; break;
    case 0x30: STR_APPEND(str_rep, "%s", "BMI"); ins.bytes = 1; ins.inst = INS_BMI; ins.am = AM_RELATIVE; break;
    case 0x50: STR_APPEND(str_rep, "%s", "BVC"); ins.bytes = 1; ins.inst = INS_BVC; ins.am = AM_RELATIVE; break;
    case 0x70: STR_APPEND(str_rep, "%s", "BVS"); ins.bytes = 1; ins.inst = INS_BVS; ins.am = AM_RELATIVE; break;
    case 0x90: STR_APPEND(str_rep, "%s", "BCC"); ins.bytes = 1; ins.inst = INS_BCC; ins.am = AM_RELATIVE; break;
    case 0xB0: STR_APPEND(str_rep, "%s", "BCS"); ins.bytes = 1; ins.inst = INS_BCS; ins.am = AM_RELATIVE; break;
    case 0xD0: STR_APPEND(str_rep, "%s", "BNE"); ins.bytes = 1; ins.inst = INS_BNE; ins.am = AM_RELATIVE; break;
    case 0xF0: STR_APPEND(str_rep, "%s", "BEQ"); ins.bytes = 1; ins.inst = INS_BEQ; ins.am = AM_RELATIVE; break;

    case 0x00: STR_APPEND(str_rep, "%s", "BRK"); ins.bytes = 1; ins.inst = INS_BRK; ins.am = AM_IMPLIED; break;
    case 0x40: STR_APPEND(str_rep, "%s", "RTI"); ins.bytes = 1; ins.inst = INS_RTI; ins.am = AM_IMPLIED; break;
    case 0x60: STR_APPEND(str_rep, "%s", "RTS"); ins.bytes = 1; ins.inst = INS_RTS; ins.am = AM_IMPLIED; break;

    case 0x08: STR_APPEND(str_rep, "%s", "PHP"); ins.bytes = 1; ins.inst = INS_PHP; ins.am = AM_IMPLIED; break;
    case 0x28: STR_APPEND(str_rep, "%s", "PLP"); ins.bytes = 1; ins.inst = INS_PLP; ins.am = AM_IMPLIED; break;
    case 0x48: STR_APPEND(str_rep, "%s", "PHA"); ins.bytes = 1; ins.inst = INS_PHA; ins.am = AM_IMPLIED; break;
    case 0x68: STR_APPEND(str_rep, "%s", "PLA"); ins.bytes = 1; ins.inst = INS_PLA; ins.am = AM_IMPLIED; break;
    case 0x88: STR_APPEND(str_rep, "%s", "DEY"); ins.bytes = 1; ins.inst = INS_DEY; ins.am = AM_IMPLIED; break;
    case 0xA8: STR_APPEND(str_rep, "%s", "TAY"); ins.bytes = 1; ins.inst = INS_TAY; ins.am = AM_IMPLIED; break;
    case 0xC8: STR_APPEND(str_rep, "%s", "INY"); ins.bytes = 1; ins.inst = INS_INY; ins.am = AM_IMPLIED; break;
    case 0xE8: STR_APPEND(str_rep, "%s", "INX"); ins.bytes = 1; ins.inst = INS_INX; ins.am = AM_IMPLIED; break;

    case 0x18: STR_APPEND(str_rep, "%s", "CLC"); ins.bytes = 1; ins.inst = INS_CLC; ins.am = AM_IMPLIED; break;
    case 0x38: STR_APPEND(str_rep, "%s", "SEC"); ins.bytes = 1; ins.inst = INS_SEC; ins.am = AM_IMPLIED; break;
    case 0x58: STR_APPEND(str_rep, "%s", "CLI"); ins.bytes = 1; ins.inst = INS_CLI; ins.am = AM_IMPLIED; break;
    case 0x78: STR_APPEND(str_rep, "%s", "SEI"); ins.bytes = 1; ins.inst = INS_SEI; ins.am = AM_IMPLIED; break;
    case 0x98: STR_APPEND(str_rep, "%s", "TYA"); ins.bytes = 1; ins.inst = INS_TYA; ins.am = AM_IMPLIED; break;
    case 0xB8: STR_APPEND(str_rep, "%s", "CLV"); ins.bytes = 1; ins.inst = INS_CLV; ins.am = AM_IMPLIED; break;
    case 0xD8: STR_APPEND(str_rep, "%s", "CLD"); ins.bytes = 1; ins.inst = INS_CLD; ins.am = AM_IMPLIED; break;
    case 0xF8: STR_APPEND(str_rep, "%s", "SED"); ins.bytes = 1; ins.inst = INS_SED; ins.am = AM_IMPLIED; break;

    case 0x8A: STR_APPEND(str_rep, "%s", "TXA"); ins.bytes = 1; ins.inst = INS_TXA; ins.am = AM_IMPLIED; break;
    case 0x9A: STR_APPEND(str_rep, "%s", "TXS"); ins.bytes = 1; ins.inst = INS_TXS; ins.am = AM_IMPLIED; break;
    case 0xAA: STR_APPEND(str_rep, "%s", "TAX"); ins.bytes = 1; ins.inst = INS_TAX; ins.am = AM_IMPLIED; break;
    case 0xBA: STR_APPEND(str_rep, "%s", "TSX"); ins.bytes = 1; ins.inst = INS_TSX; ins.am = AM_IMPLIED; break;
    case 0xCA: STR_APPEND(str_rep, "%s", "DEX"); ins.bytes = 1; ins.inst = INS_DEX; ins.am = AM_IMPLIED; break;
    case 0xEA: STR_APPEND(str_rep, "%s", "NOP"); ins.bytes = 1; ins.inst = INS_NOP; ins.am = AM_IMPLIED; break;
  }

  // multibyte instructions (only if we didn't find a single byte instruction first)
  if (ins.bytes == 0 && cc == 0b00000000) {
    switch (aaa) {
      case 0b00000001: STR_APPEND(str_rep, "%s", "BIT "); ins.bytes += 1; ins.inst = INS_BIT; break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "JMP "); ins.bytes += 1; ins.inst = INS_JMP; break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "JMP "); ins.bytes += 1; ins.inst = INS_JMP; break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STY "); ins.bytes += 1; ins.inst = INS_STY; break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDY "); ins.bytes += 1; ins.inst = INS_LDY; break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "CPY "); ins.bytes += 1; ins.inst = INS_CPY; break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "CPX "); ins.bytes += 1; ins.inst = INS_CPX; break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_IMMEDIATE; break; // #immediate
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP;        break; // zeropage
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS;       break; // absolute
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP_X;      break; // zeropage, X
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS_X;     break; // absolute, X
      
      default: break;
    }
  }
  else if (ins.bytes == 0 && cc == 0b00000001) {
    switch (aaa) {
      case 0b00000000: STR_APPEND(str_rep, "%s", "ORA "); ins.bytes += 1; ins.inst = INS_ORA; break;
      case 0b00000001: STR_APPEND(str_rep, "%s", "AND "); ins.bytes += 1; ins.inst = INS_AND; break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "EOR "); ins.bytes += 1; ins.inst = INS_EOR; break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "ADC "); ins.bytes += 1; ins.inst = INS_ADC; break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STA "); ins.bytes += 1; ins.inst = INS_STA; break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDA "); ins.bytes += 1; ins.inst = INS_LDA; break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "CMP "); ins.bytes += 1; ins.inst = INS_CMP; break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "SBC "); ins.bytes += 1; ins.inst = INS_SBC; break;

      default: break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "($%02X, X)"  , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_UNKNOWN; break; // (zeropage, X)
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_ZP; break; // zeropage
      case 0b00000010: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_IMMEDIATE; break; // #immediate
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; ins.am = AM_ABS; break; // absolute
      case 0b00000100: STR_APPEND(str_rep, "($%02X), Y"  , cpu->memory[index + 1]);                           ins.bytes += 2; ins.am = AM_ZP_Y; break; // (zeropage, Y)
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_ZP_X; break; // indirect, X
      case 0b00000110: STR_APPEND(str_rep, "$%02X%02X, Y", cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; ins.am = AM_ABS_Y; break; // absolute, Y
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; ins.am = AM_ABS_X; break; // absolute, X
      
      default: break;
    }
  }
  // Group 2
  else if (ins.bytes == 0 && cc == 0b00000010) {
    switch (aaa) {
      case 0b00000000: STR_APPEND(str_rep, "%s", "ASL "); ins.bytes += 1; ins.inst = INS_ASL; break;
      case 0b00000001: STR_APPEND(str_rep, "%s", "ROL "); ins.bytes += 1; ins.inst = INS_ROL; break;
      case 0b00000010: STR_APPEND(str_rep, "%s", "LSR "); ins.bytes += 1; ins.inst = INS_LSR; break;
      case 0b00000011: STR_APPEND(str_rep, "%s", "ROR "); ins.bytes += 1; ins.inst = INS_ROR; break;
      case 0b00000100: STR_APPEND(str_rep, "%s", "STX "); ins.bytes += 1; ins.inst = INS_STX; break;
      case 0b00000101: STR_APPEND(str_rep, "%s", "LDX "); ins.bytes += 1; ins.inst = INS_LDX; break;
      case 0b00000110: STR_APPEND(str_rep, "%s", "DEC "); ins.bytes += 1; ins.inst = INS_DEC; break;
      case 0b00000111: STR_APPEND(str_rep, "%s", "INC "); ins.bytes += 1; ins.inst = INS_INC; break;

      default: break;
    }
    switch (bbb) {
      case 0b00000000: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_IMMEDIATE;  break; // #immediate
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP;         break; // zeropage
      case 0b00000010: STR_APPEND(str_rep, "");                                                             ins.bytes += 0; ins.am = AM_IMPLIED;    break; // accumulator
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS;  break; // absolute
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP_X;  break; // zeropage, X
      case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS_X; break; // absolute, X
      
      default: break;
    };
    // we found the translation
  }
  ins.valid = ins.bytes > 0;
  memcpy(ins.raw, cpu->memory + index, ins.bytes);
  return ins;
}

int cpu_is_instruction_in_range(instruction_t ins, int lower_addr, int upper_addr) {
  return ins.address > lower_addr && ins.address < upper_addr;
}
