/*
 * android.cpp
 *
 *  Created on: 2012-9-2
 *      Author: hxz
 */



#define BX_PLUGGABLE


#include "bochs.h"
#include "iodev/iodev.h"
#include "iodev/vga.h"


#if BX_WITH_ANDROID


class bx_android_gui_c : public bx_gui_c {
public:
  bx_android_gui_c (void) {}
  DECLARE_GUI_VIRTUAL_METHODS();

#if BX_SHOW_IPS
  virtual void show_ips(Bit32u ips_count);
#endif
};

// declare one instance of the gui object and call macro to insert the
// plugin code
static bx_android_gui_c *theGui = NULL;
IMPLEMENT_GUI_PLUGIN_CODE(android)

#define LOG_THIS theGui->


void bx_android_gui_c::specific_init(int argc, char **argv,                           \
         unsigned x_tilesize, unsigned y_tilesize,                          \
         unsigned header_bar_y)
{
	unsigned char fc, vc;
	int i;

	for(i = 0; i < 256; i++) {
		for(int j = 0; j < 16; j++) {
			vc = bx_vgafont[i].data[j];
			fc = 0;
			for (int b = 0; b < 8; b++) {
				fc |= (vc & 0x01) << (7 - b);
				vc >>= 1;
			}
			vga_charmap[i*32+j] = fc;
		}
	}


	return;

}


void bx_android_gui_c::text_update(Bit8u *old_text, Bit8u *new_text,                  \
                  unsigned long cursor_x, unsigned long cursor_y,           \
                  bx_vga_tminfo_t tm_info)
{

	bx_bool force_update = 0;

	blink_mode = (tm_info.blink_flags & BX_TEXT_BLINK_MODE) > 0;
	blink_state = (tm_info.blink_flags & BX_TEXT_BLINK_STATE) > 0;

	if (blink_mode) {
		if (tm_info.blink_flags & BX_TEXT_BLINK_TOGGLE)
		  forceUpdate = 1;
	  }

	if(charmap_updated) {
		force_update = 1;
		charmap_updated = 0;
	}
	return;
}



void bx_android_gui_c::graphics_tile_update(Bit8u *snapshot, unsigned x, unsigned y)
{
	return;
}


void bx_android_gui_c::handle_events(void)
{
	return;
}



void bx_android_gui_c::flush(void)
{
	return;
}


void bx_android_gui_c::clear_screen(void)
{
	return;
}


bx_bool bx_android_gui_c::palette_change(unsigned index,                              \
			unsigned red, unsigned green, unsigned blue)
{
	return 1;
}


void  bx_android_gui_c::dimension_update(unsigned x, unsigned y, unsigned fheight,   \
                          unsigned fwidth, unsigned bpp)
{
	return;
}


unsigned  bx_android_gui_c::create_bitmap(const unsigned char *bmap,                   \
			unsigned xdim, unsigned ydim)
{
	return 0; // return index as handle
}


unsigned  bx_android_gui_c::headerbar_bitmap(unsigned bmap_id, unsigned alignment,     \
			void (*f)(void))
{
	return 0;

}

void bx_android_gui_c::replace_bitmap(unsigned hbar_id, unsigned bmap_id)
{
	return;
}


void bx_android_gui_c::show_headerbar(void)
{
	return;
}


int bx_android_gui_c::get_clipboard_text(Bit8u **bytes, Bit32s *nbytes)
{
	return 0;
}

int bx_android_gui_c::set_clipboard_text(char *snapshot, Bit32u len)
{
	return 0;
}

void bx_android_gui_c::mouse_enabled_changed_specific (bx_bool val)
{
	return;
}

void bx_android_gui_c::exit(void)
{
	return;
}


void bx_android_gui_c::show_ips(Bit32u ips_count)
{
	return;
}

#endif
