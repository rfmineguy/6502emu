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
  AM_IMMEDIATE, // #
  AM_ZP,        // zeropage
  AM_ZP_X,      // zeropage, X
  AM_ZP_Y,      // zeropage, Y
  AM_ABS,       // $
  AM_ABS_X,     // $__, X
  AM_ABS_Y,     // $__, Y
  AM_IND,       // JMP only  JMP ($fffc)   jmps to addresss stored at $FFFC
  AM_IND_X,     // (ind,X)
  AM_IND_Y,     // (ind,Y)
  AM_RELATIVE,  // ....
  AM_IMPLIED,   // ....
  AM_ACCUMULATOR,
} addressing_mode_e;

// instruction_e
typedef enum {
  INS_ADC,
  INS_AND,
  INS_ASL,
  INS_BCC,
  INS_BCS,
  INS_BEQ,
  INS_BIT,
  INS_BMI,
  INS_BNE,
  INS_BPL,
  INS_BRK,
  INS_BVC,
  INS_BVS,
  INS_CLC,
  INS_CLD,
  INS_CLI,
  INS_CLV,
  INS_CMP,
  INS_CPX,
  INS_CPY,
  INS_DEC,
  INS_DEX,
  INS_DEY,
  INS_EOR,
  INS_INC,
  INS_INX,
  INS_INY,
  INS_JMP,
  INS_JSR,
  INS_LDA,
  INS_LDX,
  INS_LDY,
  INS_LSR,
  INS_NOP,
  INS_ORA,
  INS_PHA,
  INS_PHP,
  INS_PLA,
  INS_PLP,
  INS_ROL,
  INS_ROR,
  INS_RTI,
  INS_RTS,
  INS_SBC,
  INS_SEC,
  INS_SED,
  INS_SEI,
  INS_STA,
  INS_STX,
  INS_STY,
  INS_TAX,
  INS_TAY,
  INS_TSX,
  INS_TXA,
  INS_TXS,
  INS_TYA
} instruction_e;

// status_flags_e
typedef enum {
  SF_NEGATIVE = 0b10000000,
  SF_OVERFLOW = 0b01000000,
  SF_DECIMAL  = 0b00001000,
  SF_INTDISA  = 0b00000100,
  SF_ZERO     = 0b00000010,
  SF_CARRY    = 0b00000001
} status_flags_e;

//register_e
typedef enum {
  REG_A,
  REG_X,
  REG_Y
} register_e;

//instruction_t
typedef struct {
  char      str[50];  // the translated string of the instruction
  uint8_t   raw[3];   // the raw string of bytes for the instruction
  uint16_t  address;  // address that this instruction resides in
  uint8_t   bytes;    // the amount of bytes this instruction uses (1-3)
  int       valid;    // not used as of now
  
  instruction_e     inst;
  addressing_mode_e am;
  int       is_bp;

  int       cycles;
} instruction_t;

//cpu_t
typedef struct {
  uint8_t  memory[0xffff];
  uint8_t  status_flags;
  uint8_t  regA, regX, regY;
  uint8_t  sp;
  uint16_t pc;
  uint32_t cycles;

  uint8_t  loaded;
} cpu_t;

uint8_t*      cpu_util_get_reg_ptr(register_e, cpu_t*);

void          cpu_reset(cpu_t*);
void          cpu_dump(cpu_t*);
void          cpu_reg_load(register_e, addressing_mode_e, cpu_t*);
void          cpu_reg_store(register_e, addressing_mode_e, cpu_t*);

int           cpu_load_program(const char*, cpu_t*);
instruction_t cpu_step(cpu_t*);

uint8_t       cpu_fetch(cpu_t*);
void          cpu_execute(cpu_t*, instruction_t);

void          cpu_adc(cpu_t*, instruction_t);
void          cpu_and(cpu_t*, instruction_t);
void          cpu_asl(cpu_t*, instruction_t);

void          cpu_bcc(cpu_t*, instruction_t);
void          cpu_bcs(cpu_t*, instruction_t);
void          cpu_beq(cpu_t*, instruction_t);
void          cpu_bit(cpu_t*, instruction_t);
void          cpu_bmi(cpu_t*, instruction_t);
void          cpu_bne(cpu_t*, instruction_t);
void          cpu_bpl(cpu_t*, instruction_t);
void          cpu_brk(cpu_t*, instruction_t);
void          cpu_bvc(cpu_t*, instruction_t);
void          cpu_bvs(cpu_t*, instruction_t);

void          cpu_jmp(cpu_t*, instruction_t);

void          cpu_cmp(cpu_t*, instruction_t, uint8_t* reg /*A,X,Y*/);
void          cpu_dec_mem(cpu_t*, instruction_t);
void          cpu_inc_mem(cpu_t*, instruction_t);

void          cpu_lda(cpu_t*, instruction_t);
void          cpu_ldx(cpu_t*, instruction_t);
void          cpu_ldy(cpu_t*, instruction_t);

void          cpu_sta(cpu_t*, instruction_t);
void          cpu_stx(cpu_t*, instruction_t);
void          cpu_sty(cpu_t*, instruction_t);

void          cpu_ora(cpu_t*, instruction_t);

instruction_t cpu_get_instruction(int, const cpu_t*);

int           cpu_is_instruction_in_range(instruction_t, int, int);

#ifdef __cplusplus
}
#endif

#endif
