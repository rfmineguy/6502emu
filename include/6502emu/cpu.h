#ifndef CPU_H
#define CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
  CPU_STATUS_FLAGS_NEGATIVE          = 0b10000000,
  CPU_STATUS_FLAGS_OVERFLOW          = 0b01000000,
  // ------UNUSED---------- = 0b00100000
  CPU_STATUS_FLAGS_BREAK             = 0b00010000,
  CPU_STATUS_FLAGS_DECIMAL           = 0b00001000,
  CPU_STATUS_FLAGS_INTERRUPT_DISABLE = 0b00000100,
  CPU_STATUS_FLAGS_ZERO              = 0b00000010,
  CPU_STATUS_FLAGS_CARRY             = 0b00000001
} cpu_status_flags_e;

// opcodes_e
//    presented in order of http://6502.org/users/obelisk/6502/reference.html
typedef enum {
  // EMU opcodes
  OP_EMU_EXIT   = 0x2,

  // LDA opcodes
  OP_LDA_IMM    = 0xA9,
  OP_LDA_ZP     = 0xA5,
  OP_LDA_ZP_X   = 0xB5,
  OP_LDA_ABS    = 0xAD,
  OP_LDA_ABS_X  = 0xBD,
  OP_LDA_ABS_Y  = 0xB9,
  OP_LDA_IND_X  = 0xA1,
  OP_LDA_IND_Y  = 0xB1,

  // STA opcodes
  OP_STA_ZP     = 0x85,
  OP_STA_ZP_X   = 0x95,
  OP_STA_ABS    = 0x8D,
  OP_STA_ABS_X  = 0x9D,
  OP_STA_ABS_Y  = 0x99,
  OP_STA_IND_X  = 0x81,
  OP_STA_IND_Y  = 0x91,

  // LDX opcodes
  OP_LDX_IMM    = 0xA2,
  OP_LDX_ZP     = 0xA6,
  OP_LDX_ZP_Y   = 0xB6,
  OP_LDX_ABS    = 0xAE,
  OP_LDX_ABS_Y  = 0xBE,

  // LDY opcodes
  OP_LDY_IMM    = 0xA0,
  OP_LDY_ZP     = 0xA4,
  OP_LDY_ZP_X   = 0xB4,
  OP_LDY_ABS    = 0xAC,
  OP_LDY_ABS_X  = 0xBC,
} opcode_e;

//addressing_mode_e
typedef enum {
  AM_IMMEDIATE,
  AM_ZP,
  AM_ZP_X,
  AM_ABS,
  AM_ABS_X,
  AM_ABS_Y,
  AM_IND_X,
  AM_IND_Y
} addressing_mode_e;

//register_e
typedef enum {
  REG_A,
  REG_X,
  REG_Y
} register_e;

typedef struct {
  uint8_t  memory[0xffff];
  uint8_t  status_flags;
  uint8_t  regA, regX, regY;
  uint8_t  sp;
  uint16_t pc;
  uint32_t cycles;
} cpu_t;

uint8_t*  cpu_util_get_reg_ptr(register_e, cpu_t*);

void      cpu_reset(cpu_t*);
void      cpu_dump(cpu_t*);
opcode_e  cpu_fetch(cpu_t*);
void      cpu_reg_load(register_e, addressing_mode_e, cpu_t*);
void      cpu_reg_store(register_e, addressing_mode_e, cpu_t*);

int       cpu_load_program(const char*, cpu_t*);
int       cpu_step(cpu_t*);

#ifdef __cplusplus
}
#endif

#endif
