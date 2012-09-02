# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


LOCAL_C_INCLUDES	:=	$(LOCAL_PATH)/instrument/stubs $(LOCAL_PATH)/cpu $(LOCAL_PATH)/iodev $(LOCAL_PATH)/gui
LOCAL_MODULE    := vmcore
LOCAL_SRC_FILES := main.cpp logio.cpp gui/siminterface.cpp pc_system.cpp iodev/devices.cpp cpu/init.cpp \
	cpu/debugstuff.cpp memory/misc_mem.cpp gui/gui.cpp config.cpp gui/paramtree.cpp iodev/virt_timer.cpp \
	cpu/cpu.cpp load32bitOShack.cpp gui/textconfig.cpp cpu/paging.cpp plugin.cpp \
	iodev/slowdown_timer.cpp iodev/cmos.cpp iodev/dma.cpp iodev/pic.cpp iodev/vga.cpp \
	iodev/floppy.cpp iodev/hdimage.cpp iodev/ioapic.cpp iodev/keyboard.cpp iodev/parallel.cpp \
	iodev/serial.cpp iodev/harddrv.cpp cpu/crregs.cpp cpu/flag_ctrl_pro.cpp cpu/segment_ctrl_pro.cpp \
	cpu/icache.cpp cpu/apic.cpp cpu/proc_ctrl.cpp cpu/access.cpp \
	cpu/cpudb/atom_n270.cpp cpu/fetchdecode.cpp cpu/bcd.cpp cpu/msr.cpp cpu/smm.cpp \
	cpu/cpudb/core_duo_t2400_yonah.cpp cpu/generic_cpuid.cpp cpu/cpudb/p2_klamath.cpp \
	cpu/cpudb/p3_katmai.cpp cpu/cpudb/p4_willamette.cpp \
	disasm/dis_decode.cpp disasm/dis_groups.cpp disasm/resolve.cpp disasm/syntax.cpp \
	gui/keymap.cpp gui/enh_dbg.cpp osdep.cpp cpu/exception.cpp cpu/jmp_far.cpp \
	memory/memory.cpp iodev/unmapped.cpp iodev/biosdev.cpp iodev/speaker.cpp iodev/extfpuirq.cpp \
	iodev/pit82c54.cpp iodev/cdrom.cpp iodev/vmware3.cpp iodev/vmware4.cpp iodev/vvfat.cpp \
	iodev/scancodes.cpp cpu/access32.cpp cpu/resolver.cpp \
	iodev/cdrom_misc.cpp cpu/tasking.cpp cpu/ctrl_xfer_pro.cpp cpu/vm8086.cpp \
	cpu/bit.cpp cpu/arith16.cpp cpu/arith32.cpp cpu/arith64.cpp cpu/arith8.cpp \
	cpu/logical16.cpp cpu/logical32.cpp cpu/logical64.cpp cpu/logical8.cpp  \
	cpu/segment_ctrl.cpp cpu/shift8.cpp cpu/shift16.cpp cpu/shift32.cpp cpu/shift64.cpp \
	cpu/xop.cpp cpu/xsave.cpp cpu/string.cpp cpu/stack16.cpp cpu/stack32.cpp cpu/stack64.cpp \
	cpu/call_far.cpp cpu/bit16.cpp cpu/bit32.cpp cpu/bit64.cpp cpu/bmi32.cpp cpu/bmi64.cpp \
	cpu/avx_fma.cpp cpu/avx_pfp.cpp cpu/avx.cpp cpu/avx2.cpp cpu/aes.cpp cpu/crc32.cpp \
	cpu/ctrl_xfer16.cpp cpu/ctrl_xfer32.cpp cpu/ctrl_xfer64.cpp \
	cpu/data_xfer16.cpp cpu/data_xfer32.cpp cpu/data_xfer64.cpp cpu/data_xfer8.cpp \
	cpu/flag_ctrl.cpp cpu/fpu_emu.cpp cpu/io.cpp cpu/iret.cpp cpu/gather.cpp \
	cpu/mult16.cpp cpu/mult32.cpp cpu/mult64.cpp cpu/mult8.cpp cpu/protect_ctrl.cpp \
	cpu/load.cpp cpu/vmx.cpp cpu/vmfunc.cpp cpu/vmexit.cpp  cpu/vmcs.cpp \
	cpu/tbm32.cpp cpu/tbm64.cpp  cpu/sse.cpp cpu/sse_string.cpp cpu/sse_rcp.cpp \
	cpu/soft_int.cpp  cpu/ret_far.cpp cpu/3dnow.cpp cpu/access64.cpp cpu/fetchdecode64.cpp \
	cpu/mmx.cpp cpu/sse_move.cpp cpu/sse_pfp.cpp \
	fpu/f2xm1.cpp fpu/ferr.cpp fpu/fpatan.cpp fpu/fprem.cpp fpu/fpu_arith.cpp fpu/fpu_compare.cpp \
	fpu/fpu_const.cpp fpu/fpu_load_store.cpp fpu/fpu_misc.cpp fpu/fpu_tags.cpp fpu/fpu_trans.cpp \
	fpu/fpu.cpp fpu/fsincos.cpp fpu/fyl2x.cpp fpu/poly.cpp fpu/softfloat-muladd.cpp \
	fpu/softfloat-round-pack.cpp fpu/softfloat-specialize.cpp fpu/softfloat.cpp \
	fpu/softfloat16.cpp fpu/softfloatx80.cpp gui/rfb.cpp iodev/pit_wrap.cpp bxmain.cpp gui/android.cpp
LOCAL_LDLIBS    := -llog -landroid -lc -stdc++

include $(BUILD_SHARED_LIBRARY)
