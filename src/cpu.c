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

void cpu_dump_regs(const cpu_t* cpu) {
  #define BYTE_BIT_FMT "%d%d%d%d%d%d%d%d"
  #define BYTE_BIT_ARG(x) ((x) >> 7)&1, ((x) >> 6)&1, ((x) >> 5)&1, ((x) >> 4)&1, ((x) >> 3)&1, ((x) >> 2)&1, ((x) >> 1)&1, ((x) >> 0)&1

  printf("A: %02X\n", cpu->regA);
  printf("X: %02X\n", cpu->regX);
  printf("Y: %02X\n", cpu->regY);
  printf("Status: " BYTE_BIT_FMT, BYTE_BIT_ARG(cpu->status_flags));
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
  case INS_BCC: if ((cpu->status_flags & SF_CARRY) == 0)              { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BCS: if ((cpu->status_flags & SF_CARRY) == SF_CARRY)       { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BEQ: if ((cpu->status_flags & SF_ZERO) == SF_ZERO)         { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BMI: if ((cpu->status_flags & SF_NEGATIVE) == SF_NEGATIVE) { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BNE: if ((cpu->status_flags & SF_ZERO) == 0)               { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BPL: if ((cpu->status_flags & SF_NEGATIVE) == 0)           { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BVC: if ((cpu->status_flags & SF_OVERFLOW) == 0)           { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BVS: if ((cpu->status_flags & SF_OVERFLOW) == SF_OVERFLOW) { cpu->pc += (*(int8_t*)(ins.raw + 1)); } break;
  case INS_BRK: cpu_brk(cpu, ins); break;
  case INS_BIT: cpu_bit(cpu, ins); break;
  case INS_CLC: cpu->status_flags &= ~(SF_CARRY);          break; //clear the carry bit
  case INS_CLD: cpu->status_flags &= ~(SF_DECIMAL);        break; //clear the decimal bit
  case INS_CLI: cpu->status_flags &= ~(SF_INTDISA);        break; //clear the interupt disable bit
  case INS_CLV: cpu->status_flags &= ~(SF_OVERFLOW);       break; //clear the overflow bit
  case INS_CMP: cpu_cmp(cpu, ins, &cpu->regA);             break;
  case INS_CPX: cpu_cmp(cpu, ins, &cpu->regX);             break;
  case INS_CPY: cpu_cmp(cpu, ins, &cpu->regY);             break;
  case INS_DEC: cpu_dec_mem(cpu, ins); break;
  case INS_DEX: cpu->regX--; 
                cpu->regX == 0         ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regX & 0b10000000 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INS_DEX
  case INS_DEY: cpu->regY--;
                cpu->regY == 0         ? (cpu->status_flags |= SF_ZERO    ) : (cpu->status_flags &= ~(SF_ZERO));
                cpu->regY & 0b10000000 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
                break; // INS_DEY
  case INS_EOR: cpu_eor(cpu, ins); break;
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
  case INS_JSR: cpu_jsr(cpu, ins); break;
  case INS_LDA: cpu_lda(cpu, ins); break;
  case INS_LDX: cpu_ldx(cpu, ins); break;
  case INS_LDY: cpu_ldy(cpu, ins); break;
  case INS_LSR: cpu_lsr(cpu, ins); break;
  case INS_NOP: /* nothing TODO: enter cycle count */ break;
  case INS_ORA: cpu_ora(cpu, ins); break;
  case INS_PHA: cpu->memory[CPU_STACK_BASE + cpu->sp--] = cpu->regA;                               break;
  case INS_PHP: cpu->memory[CPU_STACK_BASE + cpu->sp--] = cpu->status_flags;                       break;
  case INS_PLA: cpu->regA                               = cpu->memory[CPU_STACK_BASE + ++cpu->sp]; break;
  case INS_PLP: cpu->status_flags                       = cpu->memory[CPU_STACK_BASE + ++cpu->sp]; break;
  case INS_ROL: cpu_rol(cpu, ins); break;
  case INS_ROR: cpu_ror(cpu, ins); break;
  case INS_RTI: assert(0 && "Not implemented"); break;
  case INS_RTS: assert(0 && "Not implemented"); break;
  case INS_SBC: cpu_sbc(cpu, ins); break;
  case INS_SEC: cpu->status_flags |= SF_CARRY;   break;
  case INS_SED: cpu->status_flags |= SF_DECIMAL; break;
  case INS_SEI: cpu->status_flags |= SF_INTDISA; break;
  case INS_STA: cpu_sta(cpu, ins); break;
  case INS_STX: cpu_stx(cpu, ins); break;
  case INS_STY: cpu_sty(cpu, ins); break;
  case INS_TAX: cpu_transfer(cpu, &cpu->regA, &cpu->regX); break; // transfer A  to X
  case INS_TAY: cpu_transfer(cpu, &cpu->regA, &cpu->regY); break; // transfer A  to Y
  case INS_TSX: cpu_transfer(cpu, &cpu->sp  , &cpu->regX); break; // transfer SP to X
  case INS_TXA: cpu_transfer(cpu, &cpu->regX, &cpu->regA); break; // transfer X  to A
  case INS_TYA: cpu_transfer(cpu, &cpu->regY, &cpu->regA); break; // transfer Y  to A
  case INS_TXS: cpu_txs(cpu);                              break; // transfer X  to SP
  }
  // cpu->pc += ins.bytes; // increment program counter
}

void cpu_rol(cpu_t* cpu, instruction_t ins) {
  uint8_t* value_ptr;
  switch (ins.am) {
    case AM_ACCUMULATOR: value_ptr = &cpu->regA;                                                      break;
    case AM_ZP:          value_ptr = &cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1))];              break;
    case AM_ZP_X:        value_ptr = &cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1) + cpu->regX)];  break;
    case AM_ABS:         value_ptr = &cpu->memory[         (*(uint16_t*)(ins.raw + 1))];              break;
    case AM_ABS_X:       value_ptr = &cpu->memory[         (*(uint16_t*)(ins.raw + 1) + cpu->regX)];  break;
    default:             assert(0 && "Fatal default");
  }

  // 
  // NOTE: Inconsistency in the documentation for the zero flag. Look into this.
  //
  int bit7   = (*value_ptr) & 0b10000000;
  *value_ptr <<= 1;
  *value_ptr = *value_ptr | (0b00000001 & (cpu->status_flags & SF_CARRY));

  (*value_ptr & 0b10000000)  ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_ror(cpu_t* cpu, instruction_t ins) {
  uint8_t* value_ptr;
  switch (ins.am) {
    case AM_ACCUMULATOR: value_ptr = &cpu->regA;                                                      break;
    case AM_ZP:          value_ptr = &cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1))];              break;
    case AM_ZP_X:        value_ptr = &cpu->memory[(uint16_t)(*(uint8_t*)(ins.raw + 1) + cpu->regX)];  break;
    case AM_ABS:         value_ptr = &cpu->memory[         (*(uint16_t*)(ins.raw + 1))];              break;
    case AM_ABS_X:       value_ptr = &cpu->memory[         (*(uint16_t*)(ins.raw + 1) + cpu->regX)];  break;
    default:             assert(0 && "Fatal default");
  }

  // 
  // NOTE: Inconsistency in the documentation for the zero flag. Look into this.
  //
  int bit0   = (*value_ptr) & 0b00000001;
  int bit7   = ((*value_ptr) & 0b10000000) >> 7;
  *value_ptr >>= 1;
  *value_ptr |= ((cpu->status_flags & SF_CARRY) << 7);

  bit0 == 1                  ? (cpu->status_flags |= SF_CARRY) : (cpu->status_flags &= ~(SF_CARRY));
  (*value_ptr & 0b10000000)  ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_eor(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regA ^= (uint8_t)ins.raw[1]; break;
    case AM_ZP:        cpu->regA ^= cpu->memory[(uint16_t)((uint8_t)ins.raw[1]            )]; break;
    case AM_ZP_X:      cpu->regA ^= cpu->memory[(uint16_t)((uint8_t)ins.raw[1] + cpu->regX)]; break;
    case AM_ABS:       cpu->regA ^= cpu->memory[(uint16_t)ins.raw[1]                       ]; break;
    case AM_ABS_X:     cpu->regA ^= cpu->memory[(uint16_t)(ins.raw[1]          + cpu->regX)]; break;
    case AM_ABS_Y:     cpu->regA ^= cpu->memory[(uint16_t)(ins.raw[1]          + cpu->regY)]; break;
    case AM_IND_X:     cpu->regA ^= cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]) % 0xff]; break;
    case AM_IND_Y:     cpu->regA ^= cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY]) % 0xff]; break;
    default:           assert(0 && "Fatal default"); break;
  }
  (cpu->regA & 0b10000000)               ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
  (cpu->regA & 0b01000000)               ? (cpu->status_flags |= SF_OVERFLOW) : (cpu->status_flags &= ~(SF_OVERFLOW));
  cpu->regA == 0                         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
}

void cpu_bit(cpu_t* cpu, instruction_t ins) {
  uint8_t r;
  switch (ins.am) {
    case AM_ZP:   r = (cpu->regA & cpu->memory[(uint16_t)((uint8_t)ins.raw[1])]); break;
    case AM_ABS:  r = (cpu->regA & cpu->memory[*(uint16_t*)(ins.raw + 1)]);       break;
    default:      assert(0 && "Fatal default"); break;
  }
  // printf("addr: %04X; *addr: %02X\n", *(uint16_t*)(ins.raw + 1), cpu->memory[*(uint16_t*)(ins.raw + 1)]);
  // printf("bit: r = %2X\n", r);
  (r & 0b10000000)               ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
  (r & 0b01000000)               ? (cpu->status_flags |= SF_OVERFLOW) : (cpu->status_flags &= ~(SF_OVERFLOW));
  r == 0                         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
}

void cpu_adc(cpu_t* cpu, instruction_t ins) {
  uint16_t r = 0;
  switch (ins.am) {
    case AM_IMMEDIATE: r = cpu->regA + ins.raw[1]                                                                     + (cpu->status_flags & SF_CARRY); break;
    case AM_ZP:        r = cpu->regA + cpu->memory[(uint16_t)((uint8_t)ins.raw[1])]                                   + (cpu->status_flags & SF_CARRY); break;
    case AM_ZP_X:      r = cpu->regA + cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]                       + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS:       r = cpu->regA + cpu->memory[(uint16_t)ins.raw[1]                       ]                       + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS_X:     r = cpu->regA + cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regX)]                     + (cpu->status_flags & SF_CARRY); break;
    case AM_ABS_Y:     r = cpu->regA + cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regY)]                     + (cpu->status_flags & SF_CARRY); break;
    case AM_IND_X:     r = cpu->regA + cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]) % 0xff] + (cpu->status_flags & SF_CARRY); break;
    case AM_IND_Y:     r = cpu->regA + cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY]) % 0xff] + (cpu->status_flags & SF_CARRY); break;
    default:           assert(0 && "Fatal default"); break;
  }
  cpu->regA = r;

  // Set/Clear the appropriate status flags
  (r & 0b10000000)               ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
  r == 0                         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  r > 0xff                       ? (cpu->status_flags |= SF_OVERFLOW) : (cpu->status_flags &= ~(SF_OVERFLOW));
  r & 0x100                      ? (cpu->status_flags |= SF_CARRY)    : (cpu->status_flags &= ~(SF_CARRY));
}

void cpu_sbc(cpu_t* cpu, instruction_t ins) {
  uint16_t r = 0;
  switch (ins.am) {
    case AM_IMMEDIATE: r = cpu->regA - ins.raw[1]                                                                     - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_ZP:        r = cpu->regA - cpu->memory[(uint16_t)((uint8_t)ins.raw[1])]                                   - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_ZP_X:      r = cpu->regA - cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]                       - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_ABS:       r = cpu->regA - cpu->memory[(uint16_t)ins.raw[1]                       ]                       - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_ABS_X:     r = cpu->regA - cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regX)]                     - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_ABS_Y:     r = cpu->regA - cpu->memory[(uint16_t)(ins.raw[1]            + cpu->regY)]                     - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_IND_X:     r = cpu->regA - cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]) % 0xff] - (1 - (cpu->status_flags & SF_CARRY)); break;
    case AM_IND_Y:     r = cpu->regA - cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY]) % 0xff] - (1 - (cpu->status_flags & SF_CARRY)); break;
    default:           assert(0 && "Fatal default"); break;
  }
  cpu->regA = r;

  // Set/Clear the appropriate status flags
  (r & 0b10000000)               ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
  r == 0                         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  r > 0xff                       ? (cpu->status_flags |= SF_OVERFLOW) : (cpu->status_flags &= ~(SF_OVERFLOW));
  r & 0x100                      ? (cpu->status_flags &= ~(SF_CARRY)) : (cpu->status_flags |= SF_CARRY);
}

void cpu_and(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMMEDIATE: cpu->regA &= ins.raw[1]; break;
    case AM_ZP:        cpu->regA &= cpu->memory[*(uint8_t*)  (ins.raw + 1)            ]; break;
    case AM_ZP_X:      cpu->regA &= cpu->memory[*(uint8_t*)  (ins.raw + 1) + cpu->regX]; break;
    case AM_ABS:       cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1)            ]; break;
    case AM_ABS_X:     cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1) + cpu->regX]; break;
    case AM_ABS_Y:     cpu->regA &= cpu->memory[*(uint16_t*) (ins.raw + 1) + cpu->regY]; break;
    case AM_IND_X:     cpu->regA &= cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]) % 0xff]; break;
    case AM_IND_Y:     cpu->regA &= cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY]) % 0xff]; break;
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

void cpu_lsr(cpu_t* cpu, instruction_t ins) {
  uint8_t* v;
  switch (ins.am) {
    case AM_ACCUMULATOR: v = &cpu->regA;  break;
    case AM_ZP:          v = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1])];             break;
    case AM_ZP_X:        v = &cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]; break;
    case AM_ABS:         v = &cpu->memory[(uint16_t)ins.raw[1]];                        break;
    case AM_ABS_X:       v = &cpu->memory[(uint16_t)ins.raw[1] + cpu->regX];            break;
  }
  cpu->status_flags |= (SF_CARRY & ((*v) & 0x1));
  *v >>= 1;
  (*v == 0)           ? (cpu->status_flags |= SF_ZERO) : (cpu->status_flags &= ~(SF_ZERO));
  ((*v) & 0b10000000) ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_jsr(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ABS:    cpu->memory[CPU_STACK_BASE + (cpu->sp-=2)] = cpu->pc - 1;
                    cpu->pc = *(uint16_t*)(ins.raw + 1);
                    break;
  }
}

void cpu_jmp(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ABS:     cpu->pc = *(uint16_t*)(ins.raw + 1); break;
    case AM_IND:     cpu->pc = cpu->memory[*(uint16_t*)(ins.raw + 1)]; break;  // NOTE: There is a catch here. Look into it.
  }
}

void cpu_cmp(cpu_t* cpu, instruction_t ins, uint8_t* reg) {
  int r;
  switch (ins.am) {
    case AM_IMMEDIATE: r = *reg - ins.raw[1];                                                                     break;
    case AM_ZP:        r = *reg - cpu->memory[(uint16_t)((uint8_t)ins.raw[1])];                                   break;
    case AM_ZP_X:      r = *reg - cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX];                       break;
    case AM_ZP_Y:      /**/                                                                                       break;
    case AM_ABS:       r = *reg - cpu->memory[(uint16_t)ins.raw[1]];                                              break;
    case AM_ABS_X:     r = *reg - cpu->memory[(uint16_t)ins.raw[1] + cpu->regX];                                  break;
    case AM_ABS_Y:     r = *reg - cpu->memory[(uint16_t)ins.raw[1] + cpu->regY];                                  break;
    case AM_IND_X:     r = *reg - cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX]) % 0xff]; break;
    case AM_IND_Y:     r = *reg - cpu->memory[(cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY]) % 0xff]; break;
    case AM_RELATIVE:  /**/                                                                 break;
    case AM_IMPLIED:   /**/                                                                 break;
    default:           assert(0 && "Fatal default");
  }
  // Set/Clear the appropriate status flags
  (r >= 0)         ? (cpu->status_flags |= SF_CARRY   ) : (cpu->status_flags &= ~(SF_CARRY));
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

void cpu_brk(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_IMPLIED: // generate inerrupt
      // Program counter is 16 bits, so pushing the status flags next crushes the pc
      cpu->memory[CPU_STACK_BASE + cpu->sp--] = cpu->pc;           // push program counter
      cpu->memory[CPU_STACK_BASE + cpu->sp--] = cpu->status_flags; // push status flags
      cpu->pc = *(uint16_t*)(cpu->memory + 0xfffe);
      cpu->status_flags |= SF_BRK_CMD;
      break;
  }
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
  (*value_ptr) ++;
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
    case AM_IND_X:     cpu->regA = cpu->memory[(uint16_t)((cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX])) % 0xff]; break;
    case AM_IND_Y:     cpu->regA = cpu->memory[(uint16_t)((cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY])) % 0xff]; break;
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
    case AM_ABS_Y:     cpu->regX = cpu->memory[(uint16_t)ins.raw[1] + cpu->regY];   break;
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
    case AM_ABS:   cpu->memory[*(uint16_t*)(ins.raw + 1)]                     = cpu->regA; break;
    case AM_ABS_X: cpu->memory[*(uint16_t*)(ins.raw + 1) + cpu->regX]         = cpu->regA; break;
    case AM_ABS_Y: cpu->memory[*(uint16_t*)(ins.raw + 1) + cpu->regY]         = cpu->regA; break;
    case AM_IND_X: cpu->memory[(uint16_t)cpu->memory[(uint16_t)ins.raw[1] + cpu->regX] % 0xff] = cpu->regA; break;
    case AM_IND_Y: cpu->memory[(uint16_t)cpu->memory[(uint16_t)ins.raw[1] + cpu->regY] % 0xff] = cpu->regA;break;
                   
    default:       assert(0 && "Fatal default");
  }
}

void cpu_stx(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ZP:    cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))] = cpu->regX; break;
    case AM_ZP_Y:  cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regY))] = cpu->regX; break;
    case AM_ABS:   cpu->memory[*(uint16_t*)(ins.raw + 1)]                     = cpu->regX; break;
    default:           assert(0 && "Fatal default");
  }
}

void cpu_sty(cpu_t* cpu, instruction_t ins) {
  switch (ins.am) {
    case AM_ZP:    cpu->memory[(uint16_t)((uint8_t)(ins.raw[1]            ))] = cpu->regY; break;
    case AM_ZP_X:  cpu->memory[(uint16_t)((uint8_t)(ins.raw[1] + cpu->regX))] = cpu->regY; break;
    case AM_ABS:   cpu->memory[*(uint16_t*)(ins.raw + 1)]                     = cpu->regY; break;
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
    case AM_ABS_Y:      cpu->regA |= cpu->memory[(uint16_t)(ins.raw[1] + cpu->regY)];            break;
    case AM_IND_X:      cpu->regA |= cpu->memory[(uint16_t)((cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regX])) % 0xff]; break;
    case AM_IND_Y:      cpu->regA |= cpu->memory[(uint16_t)((cpu->memory[(uint16_t)((uint8_t)ins.raw[1]) + cpu->regY])) % 0xff]; break;
  }

  cpu->regA == 0         ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  cpu->regA & 0b10000000 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
}

void cpu_txs(cpu_t* cpu) {
  cpu->sp = cpu->regX;
}

void cpu_transfer(cpu_t* cpu, uint8_t* from, uint8_t* to) {
  *to = *from;
  *to == 0          ? (cpu->status_flags |= SF_ZERO)     : (cpu->status_flags &= ~(SF_ZERO));
  *to &  0b10000000 ? (cpu->status_flags |= SF_NEGATIVE) : (cpu->status_flags &= ~(SF_NEGATIVE));
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
    case 0x20: STR_APPEND(str_rep, "%s", "JSR");
               STR_APPEND(str_rep, " $%02X%02X", cpu->memory[index + 2], cpu->memory[index + 1]);
               ins.bytes = 3; ins.inst = INS_JSR; ins.am = AM_ABS; break;

    case 0x0A: STR_APPEND(str_rep, "%s", "ASL A"); ins.bytes = 1; ins.inst = INS_ASL; ins.am = AM_ACCUMULATOR; break;
    case 0x2A: STR_APPEND(str_rep, "%s", "ROL A"); ins.bytes = 1; ins.inst = INS_ROL; ins.am = AM_ACCUMULATOR; break;
    case 0x4A: STR_APPEND(str_rep, "%s", "LSR A"); ins.bytes = 1; ins.inst = INS_LSR; ins.am = AM_ACCUMULATOR; break;
    case 0x6A: STR_APPEND(str_rep, "%s", "ROR A"); ins.bytes = 1; ins.inst = INS_ROR; ins.am = AM_ACCUMULATOR; break;

    case 0x10: STR_APPEND(str_rep, "%s", "BPL"); ins.bytes = 2; ins.inst = INS_BPL; ins.am = AM_RELATIVE; break;
    case 0x30: STR_APPEND(str_rep, "%s", "BMI"); ins.bytes = 2; ins.inst = INS_BMI; ins.am = AM_RELATIVE; break;
    case 0x50: STR_APPEND(str_rep, "%s", "BVC"); ins.bytes = 2; ins.inst = INS_BVC; ins.am = AM_RELATIVE; break;
    case 0x70: STR_APPEND(str_rep, "%s", "BVS"); ins.bytes = 2; ins.inst = INS_BVS; ins.am = AM_RELATIVE; break;
    case 0x90: STR_APPEND(str_rep, "%s", "BCC"); ins.bytes = 2; ins.inst = INS_BCC; ins.am = AM_RELATIVE; break;
    case 0xB0: STR_APPEND(str_rep, "%s", "BCS"); ins.bytes = 2; ins.inst = INS_BCS; ins.am = AM_RELATIVE; break;
    case 0xD0: STR_APPEND(str_rep, "%s", "BNE"); ins.bytes = 2; ins.inst = INS_BNE; ins.am = AM_RELATIVE; break;
    case 0xF0: STR_APPEND(str_rep, "%s", "BEQ"); ins.bytes = 2; ins.inst = INS_BEQ; ins.am = AM_RELATIVE; break;

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
      case 0b00000000: STR_APPEND(str_rep, "($%02X, X)"  , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_IND_X; break; // (indirect, X)
      case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_ZP; break; // zeropage
      case 0b00000010: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_IMMEDIATE; break; // #immediate
      case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]);   ins.bytes += 2; ins.am = AM_ABS; break; // absolute
      case 0b00000100: STR_APPEND(str_rep, "($%02X), Y"  , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_IND_Y; break; // (indirect, Y)
      case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                           ins.bytes += 1; ins.am = AM_ZP_X; break; // zeropage, X
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
    if (aaa == 0b00000100      && bbb == 0b00000101) { // STX  ZP
      STR_APPEND(str_rep, "$%02X, Y", cpu->memory[index + 1]); ins.bytes += 1; ins.am = AM_ZP_Y;
      ins.bytes += 1;
    }
    else if (aaa == 0b00000101 && bbb == 0b00000101) { // LDX  ZP
      STR_APPEND(str_rep, "$%02X, Y", cpu->memory[index + 1]); ins.bytes += 1; ins.am = AM_ZP_Y;
      ins.bytes += 1;
    }
    else if (aaa == 0b00000100      && bbb == 0b00000111) { // STX  ABS
      STR_APPEND(str_rep, "$%02X%02X, Y", cpu->memory[index + 2], cpu->memory[index + 1]); ins.am = AM_ABS_Y;
      ins.bytes += 1;
    }
    else if (aaa == 0b00000101      && bbb == 0b00000111) { // LDX  ABS
      STR_APPEND(str_rep, "$%02X%02X, Y", cpu->memory[index + 2], cpu->memory[index + 1]); ins.am = AM_ABS_Y;
      ins.bytes += 1;
    }
    else {
      switch (bbb) {
        case 0b00000000: STR_APPEND(str_rep, "#$%02X"      , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_IMMEDIATE;  break; // #immediate
        case 0b00000001: STR_APPEND(str_rep, "$%02X"       , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP;         break; // zeropage
        case 0b00000010: STR_APPEND(str_rep, "");                                                             ins.bytes += 0; ins.am = AM_IMPLIED;    break; // accumulator
        case 0b00000011: STR_APPEND(str_rep, "$%02X%02X"   , cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS;  break; // absolute
        case 0b00000101: STR_APPEND(str_rep, "$%02X, X"    , cpu->memory[index + 1]);                         ins.bytes += 1; ins.am = AM_ZP_X;  break; // zeropage, X
        case 0b00000111: STR_APPEND(str_rep, "$%02X%02X, X", cpu->memory[index + 2], cpu->memory[index + 1]); ins.bytes += 2; ins.am = AM_ABS_X; break; // absolute, X
        default: break;
      }
    }
    // we found the translation
  }
  ins.valid = ins.bytes > 0;
  memcpy(ins.raw, cpu->memory + index, ins.bytes);
  return ins;
}

int cpu_is_instruction_in_range(instruction_t ins, int lower_addr, int upper_addr) {
  return ins.address > lower_addr && ins.address < upper_addr;
}
