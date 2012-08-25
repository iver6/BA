/////////////////////////////////////////////////////////////////////////
// $Id: bmi32.cc 10888 2011-12-29 20:52:44Z sshwarts $
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

#if BX_SUPPORT_AVX

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::ANDN_GdBdEdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());
  Bit32u op2_32 = BX_READ_32BIT_REG(i->vvv());

  op1_32 = op1_32 & ~op2_32;

  SET_FLAGS_OSZAxC_LOGIC_32(op1_32); // keep PF unchanged

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::MULX_GdBdEdR(bxInstruction_c *i)
{
  Bit32u op1_32 = EDX;
  Bit32u op2_32 = BX_READ_32BIT_REG(i->rm());
  Bit64u product_64  = ((Bit64u) op1_32) * ((Bit64u) op2_32);

  BX_WRITE_32BIT_REGZ(i->vvv(), GET32L(product_64));
  BX_WRITE_32BIT_REGZ(i->nnn(), GET32H(product_64));

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSI_BdEdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());
  bx_bool tmpCF = (op1_32 != 0);

  op1_32 = (-op1_32) & op1_32;

  SET_FLAGS_OSZAxC_LOGIC_32(op1_32); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_32BIT_REGZ(i->vvv(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSMSK_BdEdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());
  bx_bool tmpCF = (op1_32 == 0);

  op1_32 = (op1_32-1) ^ op1_32;

  SET_FLAGS_OSZAxC_LOGIC_32(op1_32); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_32BIT_REGZ(i->vvv(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSR_BdEdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());
  bx_bool tmpCF = (op1_32 == 0);

  op1_32 = (op1_32-1) & op1_32;

  SET_FLAGS_OSZAxC_LOGIC_32(op1_32); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_32BIT_REGZ(i->vvv(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::RORX_GdEdIbR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());

  unsigned count = i->Ib() & 0x1f;
  if (count) {
    op1_32 = (op1_32 >> count) | (op1_32 << (32 - count));
  }

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SHRX_GdEdBdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x1f;
  if (count)
    op1_32 >>= count;

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SARX_GdEdBdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x1f;
  if (count) {
    /* count < 32, since only lower 5 bits used */
    op1_32 = ((Bit32s) op1_32) >> count;
  }

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::SHLX_GdEdBdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());

  unsigned count = BX_READ_32BIT_REG(i->vvv()) & 0x1f;
  if (count)
    op1_32 <<= count;

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BEXTR_GdEdBdR(bxInstruction_c *i)
{
  Bit16u control = BX_READ_16BIT_REG(i->vvv());
  unsigned start = control & 0xff;
  unsigned len   = control >> 8;
  Bit32u op1_32 = 0;

  if (start < 32 && len > 0) {
    op1_32 = BX_READ_32BIT_REG(i->rm());
    op1_32 >>= start;

    if (len < 32) {
      Bit32u extract_mask = (1 << len) - 1;
      op1_32 &= extract_mask;
    }
  }

  SET_FLAGS_OSZAPC_LOGIC_32(op1_32);

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BZHI_GdEdBdR(bxInstruction_c *i)
{
  unsigned control = BX_READ_16BIT_REG(i->vvv()) & 0xff;
  bx_bool tmpCF = 0;
  Bit32u op1_32 = BX_READ_32BIT_REG(i->rm());
  
  if (control < 32) {
    Bit32u mask = (1 << control) - 1;
    op1_32 &= mask;
  }
  else {
    tmpCF = 1;
  }

  SET_FLAGS_OSZAxC_LOGIC_32(op1_32); // keep PF unchanged
  set_CF(tmpCF);

  BX_WRITE_32BIT_REGZ(i->nnn(), op1_32);

  BX_NEXT_INSTR(i);
}


BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::PEXT_GdEdBdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->vvv());
  Bit32u op2_32 = BX_READ_32BIT_REG(i->rm()), result_32 = 0;

  Bit32u wr_mask = 0x1;

  for (; op2_32 != 0; op2_32 >>= 1)
  {
    if (op2_32 & 0x1) {
      if (op1_32 & 0x1) result_32 |= wr_mask;
      wr_mask <<= 1;
    }
    op1_32 >>= 1;
  }
  
  BX_WRITE_32BIT_REGZ(i->nnn(), result_32);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::PDEP_GdEdBdR(bxInstruction_c *i)
{
  Bit32u op1_32 = BX_READ_32BIT_REG(i->vvv());
  Bit32u op2_32 = BX_READ_32BIT_REG(i->rm()), result_32 = 0;

  Bit32u wr_mask = 0x1;

  for (; op2_32 != 0; op2_32 >>= 1)
  {
    if (op2_32 & 0x1) {
      if (op1_32 & 0x1) result_32 |= wr_mask;
      op1_32 >>= 1;
    }
    wr_mask <<= 1;
  }
  
  BX_WRITE_32BIT_REGZ(i->nnn(), result_32);

  BX_NEXT_INSTR(i);
}

#endif