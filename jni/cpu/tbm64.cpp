/////////////////////////////////////////////////////////////////////////
// $Id: tbm64.cc 10740 2011-10-20 14:06:12Z sshwarts $
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

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BEXTR_GqEqIdR(bxInstruction_c *i)
{
  Bit16u control = (Bit16u) i->Id();
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

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCFILL_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 + 1) & op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCI_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = ~(op_64 + 1) | op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCIC_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 + 1) & ~op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCMSK_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 + 1) ^ op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLCS_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 + 1) | op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSFILL_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 - 1) | op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF(op_64 == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::BLSIC_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 - 1) | ~op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF(op_64 == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);
  
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::T1MSKC_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 + 1) | ~op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF((op_64 + 1) == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::TZMSK_BqEqR(bxInstruction_c *i)
{
  Bit64u op_64 = BX_READ_64BIT_REG(i->rm());

  Bit64u result_64 = (op_64 - 1) & ~op_64;

  SET_FLAGS_OSZAPC_LOGIC_64(result_64);
  set_CF(op_64 == 0);

  BX_WRITE_64BIT_REG(i->vvv(), result_64);
  
  BX_NEXT_INSTR(i);
}

#endif
