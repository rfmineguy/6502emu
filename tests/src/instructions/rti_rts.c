#include "tests.h"
#include "6502emu/cpu.h"

MunitResult rti_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x40; // RTI
  
  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  /* Test whether it was parsed as the correct instruction */
  munit_assert_string_equal(ins.str, "RTI");

  cpu_execute(&cpu, ins);

  return MUNIT_ERROR; 
}

// http://archive.6502.org/datasheets/synertek_programming_manual.pdf<br>
//  Page 108
MunitResult rts_imp  (const MunitParameter params[], void* fixture) {
  cpu_t cpu = {0};
  cpu.memory[0] = 0x60;

  instruction_t ins = cpu_get_instruction(0x0, &cpu);

  munit_assert_string_equal(ins.str, "RTS");

  cpu.sp = CPU_STACK_SIZE;

  // before rts (simulating a jsr happening so we can test rts)
  cpu.pc = 0x0040;
  cpu.memory[CPU_STACK_BASE + cpu.sp - 2] = cpu.pc;            // push return address to stack
  cpu.sp -= 2;

  // we've executed our subroutine a bit (now at 0x54)
  cpu.pc += 0x0054;

  // we will now return from it and get back to our 0x40 + 1
  cpu_execute(&cpu, ins);
  munit_assert_int(cpu.pc,           ==, 0x0041); // goes one space before the return address. this feels wrong

  return MUNIT_OK; 
}
