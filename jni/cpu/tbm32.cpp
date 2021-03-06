/////////////////////////////////////////////////////////////////////////
// $Id: tbm32.cc 10740 2011-10-20 14:06:12Z sshwarts $
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

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BEXTR_GdEdIdR(bxInstruction_c *i)
{
  Bit16u control = (Bit16u) i->Id();
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

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCFILL_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 + 1) & op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCI_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = ~(op_32 + 1) | op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCIC_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 + 1) & ~op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCMSK_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 + 1) ^ op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCS_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 + 1) | op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSFILL_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 - 1) | op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF(op_32 == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSIC_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 - 1) | ~op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF(op_32 == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::T1MSKC_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 + 1) | ~op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF((op_32 + 1) == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::TZMSK_BdEdR(bxInstruction_c *i)
{
  Bit32u op_32 = BX_READ_32BIT_REG(i->rm());

  Bit32u result_32 = (op_32 - 1) & ~op_32;

  SET_FLAGS_OSZAPC_LOGIC_32(result_32);
  set_CF(op_32 == 0);

  BX_WRITE_32BIT_REGZ(i->vvv(), result_32);
  
  BX_NEXT_INSTR(i);
}

#endif
