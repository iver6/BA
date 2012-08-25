/////////////////////////////////////////////////////////////////////////
// $Id: instrument.cc 10491 2011-07-23 19:58:38Z sshwarts $
/////////////////////////////////////////////////////////////////////////
//
//   Copyright (c) 2006-2009 Stanislav Shwartsman
//          Written by Stanislav Shwartsman [sshwarts at sourceforge net]
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA


#include <assert.h>

#include "bochs.h"
#include "cpu/cpu.h"
#include "disasm/disasm.h"

// maximum size of an instruction
#define MAX_OPCODE_LENGTH 16

// maximum physical addresses an instruction can generate
#define MAX_DATA_ACCESSES 1024

// Use this variable to turn on/off collection of instrumentation data
// If you are not using the debugger to turn this on/off, then possibly
// start this at 1 instead of 0.
static bx_bool active = 1;

static disassembler bx_disassembler;

static struct instruction_t {
  bx_bool  ready;         // is current instruction ready to be printed
  unsigned opcode_length;
  Bit8u    opcode[MAX_OPCODE_LENGTH];
  bx_bool  is32, is64;
  unsigned num_data_accesses;
  struct {
    bx_address laddr;     // linear address
    bx_phy_address paddr; // physical address
    unsigned op;          // BX_READ, BX_WRITE or BX_RW
    unsigned size;        // 1 .. 32
  } data_access[MAX_DATA_ACCESSES];
  bx_bool is_branch;
  bx_bool is_taken;
  bx_address target_linear;
} *instruction;

static logfunctions *instrument_log = new logfunctions ();
#define LOG_THIS instrument_log->

void bx_instr_init_env(void) {}
void bx_instr_exit_env(void) {}

void bx_instr_initialize(unsigned cpu)
{
  assert(cpu < BX_SMP_PROCESSORS);

  if (instruction == NULL)
      instruction = new struct instruction_t[BX_SMP_PROCESSORS];

  fprintf(stderr, "Initialize cpu %d\n", cpu);
}

void bx_instr_reset(unsigned cpu, unsigned type)
{
  instruction[cpu].ready = 0;
  instruction[cpu].num_data_accesses = 0;
  instruction[cpu].is_branch = 0;
}

void bx_print_instruction(unsigned cpu, const instruction_t *i)
{
  char disasm_tbuf[512];	// buffer for instruction disassembly
  unsigned length = i->opcode_length, n;
  bx_disassembler.disasm(i->is32, i->is64, 0, 0, i->opcode, disasm_tbuf);

  if(length != 0)
  {
    fprintf(stderr, "----------------------------------------------------------\n");
    fprintf(stderr, "CPU: %d: %s\n", cpu, disasm_tbuf);
    fprintf(stderr, "LEN: %d\tBYTES: ", length);
    for(n=0;n < length;n++) fprintf(stderr, "%02x", i->opcode[n]);
    if(i->is_branch)
    {
      fprintf(stderr, "\tBRANCH ");

      if(i->is_taken)
        fprintf(stderr, "TARGET " FMT_ADDRX " (TAKEN)", i->target_linear);
      else
        fprintf(stderr, "(NOT TAKEN)");
    }
    fprintf(stderr, "\n");
    for(n=0;n < i->num_data_accesses;n++)
    {
      fprintf(stderr, "MEM ACCESS[%u]: 0x" FMT_ADDRX " (linear) 0x" FMT_PHY_ADDRX " (physical) %s SIZE: %d\n", n,
                    i->data_access[n].laddr,
                    i->data_access[n].paddr,
                    i->data_access[n].op == BX_READ ? "RD":"WR",
                    i->data_access[n].size);
    }
    fprintf(stderr, "\n");
  }
}

void bx_instr_before_execution(unsigned cpu, bxInstruction_c *bx_instr)
{
  if (!active) return;

  instruction_t *i = &instruction[cpu];

  if (i->ready) bx_print_instruction(cpu, i);

  // prepare instruction_t structure for new instruction
  i->ready = 1;
  i->num_data_accesses = 0;
  i->is_branch = 0;

  i->is32 = BX_CPU(cpu)->sregs[BX_SEG_REG_CS].cache.u.segment.d_b;
  i->is64 = BX_CPU(cpu)->long64_mode();
  i->opcode_length = bx_instr->ilen();
  memcpy(i->opcode, bx_instr->get_opcode_bytes(), i->opcode_length);
}

void bx_instr_after_execution(unsigned cpu, bxInstruction_c *bx_instr)
{
  if (!active) return;

  instruction_t *i = &instruction[cpu];
  if (i->ready) {
    bx_print_instruction(cpu, i);
    i->ready = 0;
  }
}

static void branch_taken(unsigned cpu, bx_address new_eip)
{
  if (!active || !instruction[cpu].ready) return;

  // find linear address
  bx_address laddr = BX_CPU(cpu)->get_laddr(BX_SEG_REG_CS, new_eip);

  instruction[cpu].is_branch = 1;
  instruction[cpu].is_taken = 1;
  instruction[cpu].target_linear = laddr;
}

void bx_instr_cnear_branch_taken(unsigned cpu, bx_address new_eip)
{
  branch_taken(cpu, new_eip);
}

void bx_instr_cnear_branch_not_taken(unsigned cpu)
{
  if (!active || !instruction[cpu].ready) return;

  instruction[cpu].is_branch = 1;
  instruction[cpu].is_taken = 0;
}

void bx_instr_ucnear_branch(unsigned cpu, unsigned what, bx_address new_eip)
{
  branch_taken(cpu, new_eip);
}

void bx_instr_far_branch(unsigned cpu, unsigned what, Bit16u new_cs, bx_address new_eip)
{
  branch_taken(cpu, new_eip);
}

void bx_instr_interrupt(unsigned cpu, unsigned vector)
{
  if(active)
  {
    fprintf(stderr, "CPU %u: interrupt %02xh\n", cpu, vector);
  }
}

void bx_instr_exception(unsigned cpu, unsigned vector, unsigned error_code)
{
  if(active)
  {
    fprintf(stderr, "CPU %u: exception %02xh, error_code = %x\n", cpu, vector, error_code);
  }
}

void bx_instr_hwinterrupt(unsigned cpu, unsigned vector, Bit16u cs, bx_address eip)
{
  if(active)
  {
    fprintf(stderr, "CPU %u: hardware interrupt %02xh\n", cpu, vector);
  }
}

void bx_instr_mem_data_access(unsigned cpu, unsigned seg, bx_address offset, unsigned len, unsigned rw)
{
  unsigned index;
  bx_phy_address phy;

  if(!active || !instruction[cpu].ready) return;

  if (instruction[cpu].num_data_accesses >= MAX_DATA_ACCESSES)
  {
    return;
  }

  bx_address lin = BX_CPU(cpu)->get_laddr(seg, offset);
  bx_bool page_valid = BX_CPU(cpu)->dbg_xlate_linear2phy(lin, &phy);

  // If linear translation doesn't exist, a paging exception will occur.
  // Invalidate physical address data for now.
  if (!page_valid) phy = (bx_phy_address) (-1);

  index = instruction[cpu].num_data_accesses;
  instruction[cpu].data_access[index].laddr = lin;
  instruction[cpu].data_access[index].paddr = phy;
  instruction[cpu].data_access[index].op    = rw;
  instruction[cpu].data_access[index].size  = len;
  instruction[cpu].num_data_accesses++;
}
