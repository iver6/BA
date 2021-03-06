/////////////////////////////////////////////////////////////////////////
// $Id: bmi64.cc 10888 2011-12-29 20:52:44Z sshwarts $
/////////////////////////////////////////////////////////////////////////
//
//   Copyright (c) 2011 Stanislav Shwartsman
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
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA B 02110-1301 USA
/////////////////////////////////////////////////////////////////////////

#define NEED_CPU_REG_SHORTCUTS 1
#include "bochs.h"
#include "cpu.h"
#define LOG_THIS BX_CPU_THIS_PTR

#if BX_SUPPORT_X86_64 && BX_SUPPORT_AVX

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::ANDN_GqBqEqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());
  Bit64u op2_64 = BX_READ_64BIT_REG(i->vvv());

  op1_64 = op1_64 & ~op2_64;

  SET_FLAGS_OSZAxC_LOGIC_64(op1_64); // keep PF unchanged

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

extern void long_mul(Bit128u *product, Bit64u op1, Bit64u op2);

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::MULX_GqBqEqR(bxInstruction_c *i)
{
  Bit64u op1_64 = RDX;
  Bit64u op2_64 = BX_READ_64BIT_REG(i->rm());

  Bit128u product_128;

  // product_128 = ((Bit128u) op1_64) * ((Bit128u) op2_64);
  // product_64l = (Bit64u) (product_128 & 0xFFFFFFFFFFFFFFFF);
  // product_64h = (Bit64u) (product_128 >> 64);

  long_mul(&product_128,op1_64,op2_64);

  BX_WRITE_64BIT_REG(i->vvv(), product_128.lo);
  BX_WRITE_64BIT_REG(i->nnn(), product_128.hi);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSI_BqEqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());
  bx_bool tmpCF = (op1_64 != 0);

  op1_64 = (-op1_64) & op1_64;

  SET_FLAGS_OSZAxC_LOGIC_64(op1_64); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_64BIT_REG(i->vvv(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSMSK_BqEqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());
  bx_bool tmpCF = (op1_64 == 0);

  op1_64 = (op1_64-1) ^ op1_64;

  SET_FLAGS_OSZAxC_LOGIC_64(op1_64); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_64BIT_REG(i->vvv(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSR_BqEqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());
  bx_bool tmpCF = (op1_64 == 0);

  op1_64 = (op1_64-1) & op1_64;

  SET_FLAGS_OSZAxC_LOGIC_64(op1_64); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_64BIT_REG(i->vvv(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::RORX_GqEqIbR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());

  unsigned count = i->Ib() & 0x3f;
  if (count) {
    op1_64 = (op1_64 >> count) | (op1_64 << (64 - count));
  }

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SHRX_GqEqBqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x3f;
  if (count)
    op1_64 >>= count;

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SARX_GqEqBqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x3f;
  if (count) {
    /* count < 64, since only lower 6 bits used */
    op1_64 = ((Bit64s) op1_64) >> count;
  }

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SHLX_GqEqBqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x3f;
  if (count)
    op1_64 <<= count;

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BEXTR_GqEqBqR(bxInstruction_c *i)
{
  Bit16u control = BX_READ_16BIT_REG(i->vvv());
  unsigned start = control & 0xff;
  unsigned len   = control >> 8;
  Bit64u op1_64 = 0;
  
  if (start < 64 && len > 0) {
    op1_64 = BX_READ_64BIT_REG(i->rm());
    op1_64 >>= start;

    if (len < 64) {
      Bit64u extract_mask = (BX_CONST64(1) << len) - 1;
      op1_64 &= extract_mask;
    }
  }

  SET_FLAGS_OSZAPC_LOGIC_64(op1_64);

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BZHI_GqEqBqR(bxInstruction_c *i)
{
  unsigned control = BX_READ_16BIT_REG(i->vvv()) & 0xff;
  bx_bool tmpCF = 0;
  Bit64u op1_64 = BX_READ_64BIT_REG(i->rm());
  
  if (control < 64) {
    Bit64u mask = (BX_CONST64(1) << control) - 1;
    op1_64 &= mask;
  }
  else {
    tmpCF = 1;
  }

  SET_FLAGS_OSZAxC_LOGIC_64(op1_64); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_64BIT_REG(i->nnn(), op1_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::PEXT_GqEqBqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->vvv());
  Bit64u op2_64 = BX_READ_64BIT_REG(i->rm()), result_64 = 0;

  Bit64u wr_mask = 0x1;

  for (; op2_64 != 0; op2_64 >>= 1)
  {
    if (op2_64 & 0x1) {
      if (op1_64 & 0x1) result_64 |= wr_mask;
      wr_mask <<= 1;
    }
    op1_64 >>= 1;
  }

  BX_WRITE_64BIT_REG(i->nnn(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::PDEP_GqEqBqR(bxInstruction_c *i)
{
  Bit64u op1_64 = BX_READ_64BIT_REG(i->vvv());
  Bit64u op2_64 = BX_READ_64BIT_REG(i->rm()), result_64 = 0;

  Bit64u wr_mask = 0x1;

  for (; op2_64 != 0; op2_64 >>= 1)
  {
    if (op2_64 & 0x1) {
      if (op1_64 & 0x1) result_64 |= wr_mask;
      op1_64 >>= 1;
    }
    wr_mask <<= 1;
  }
  
  BX_WRITE_64BIT_REG(i->nnn(), result_64);

  BX_NEXT_INSTR(i);
}

#endif
