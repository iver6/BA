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
#include "font/vga.bitmap.h"
#include <jni.h>

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



static unsigned int text_rows=25, text_cols=80;
static unsigned int font_height=16, font_width=8;
static Bit32u ScreenBitMap[16 * 8 * 25 * 80];

static Bit32u AndroidPalette[256];

extern jobject gbitmaplock;

void DrawChar(int x, int y, int width, int height, int fonty, char *bmap, char color, bx_bool gfxchar);

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

	for(i = 0; i < 256; i++)
	{
		AndroidPalette[i] = 0xFFFFFFFF;
	}


	return;

}


void bx_android_gui_c::text_update(Bit8u *old_text, Bit8u *new_text,                  \
                  unsigned long cursor_x, unsigned long cursor_y,           \
                  bx_vga_tminfo_t tm_info)
{
	Bit8u *old_line, *new_line;
	Bit8u cAttr, cChar;
	bx_bool force_update = 0,blink_mode,blink_state,gfxchar;
	unsigned int curs, hchars, offset, rows, x, y, xc, yc;

	blink_mode = (tm_info.blink_flags & BX_TEXT_BLINK_MODE) > 0;
	blink_state = (tm_info.blink_flags & BX_TEXT_BLINK_STATE) > 0;

	if (blink_mode) {
		if (tm_info.blink_flags & BX_TEXT_BLINK_TOGGLE)
			force_update = 1;
	  }

	if(charmap_updated) {
		force_update = 1;
		charmap_updated = 0;
	}

	if((tm_info.cs_start <= tm_info.cs_end) && (tm_info.cs_start < font_height) &&
	 (cursor_y < text_rows) && (cursor_x < text_cols)) {
		curs = cursor_y * tm_info.line_offset + cursor_x * 2;
		old_text[curs] = ~new_text[curs];
		} else {
		curs = 0xffff;
	}

	rows = text_rows;
	y = 0;
	do {
	hchars = text_cols;
	new_line = new_text;
	old_line = old_text;
	offset = y * tm_info.line_offset;
	yc = y * font_height ;
	x = 0;
	do {
	  if (force_update || (old_text[0] != new_text[0])
		  || (old_text[1] != new_text[1])) {
		cChar = new_text[0];
		if (blink_mode) {
		  cAttr = new_text[1] & 0x7F;
		  if (!blink_state && (new_text[1] & 0x80))
			cAttr = (cAttr & 0x70) | (cAttr >> 4);
		} else {
		  cAttr = new_text[1];
		}
		gfxchar = tm_info.line_graphics && ((cChar & 0xE0) == 0xC0);
		xc = x * font_width;
		DrawChar(xc, yc, font_width, font_height, 0, (char *)&vga_charmap[cChar<<5], cAttr, gfxchar);
		/*
		if(yc < rfbUpdateRegion.y) rfbUpdateRegion.y = yc;
		if((yc + font_height - rfbUpdateRegion.y) > rfbUpdateRegion.height) rfbUpdateRegion.height = (yc + font_height - rfbUpdateRegion.y);
		if(xc < rfbUpdateRegion.x) rfbUpdateRegion.x = xc;
		if((xc + font_width - rfbUpdateRegion.x) > rfbUpdateRegion.width) rfbUpdateRegion.width = (xc + font_width - rfbUpdateRegion.x);
		rfbUpdateRegion.updated = true;
		*/
		if (offset == curs) {
		  cAttr = ((cAttr >> 4) & 0xF) + ((cAttr & 0xF) << 4);
		  DrawChar(xc, yc + tm_info.cs_start, font_width, tm_info.cs_end - tm_info.cs_start + 1,
				   tm_info.cs_start, (char *)&vga_charmap[cChar<<5], cAttr, gfxchar);
		}
	  }
	  x++;
	  new_text+=2;
	  old_text+=2;
	  offset+=2;
	} while (--hchars);
	y++;
	new_text = new_line + tm_info.line_offset;
	old_text = old_line + tm_info.line_offset;
	} while (--rows);
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
	AndroidPalette[index] = (red << 16) | (green << 8) | blue;
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

void DrawChar(int x, int y, int width, int height, int fonty, char *bmap, char color, bx_bool gfxchar)
{
  static unsigned char newBits[8 * 16];
  unsigned char mask;
  int bytes = width * height;
  char fgcolor, bgcolor;
  static char vgaPalette[] = {
       (char)0x00, //Black
       (char)0x01, //Dark Blue
       (char)0x02, //Dark Green
       (char)0x03, //Dark Cyan
       (char)0x04, //Dark Red
       (char)0x05, //Dark Magenta
       (char)0x06, //Brown
       (char)0x07, //Light Gray
       (char)0x38, //Dark Gray
       (char)0x09, //Light Blue
       (char)0x12, //Green
       (char)0x1B, //Cyan
       (char)0x24, //Light Red
       (char)0x2D, //Magenta
       (char)0x36, //Yellow
       (char)0x3F  //White
  };

  bgcolor = vgaPalette[(color >> 4) & 0xF];
  fgcolor = vgaPalette[color & 0xF];

  for(int i = 0; i < bytes; i+=width) {
    mask = 0x80;
    for(int j = 0; j < width; j++) {
      if (mask > 0) {
        newBits[i + j] = (bmap[fonty] & mask) ? fgcolor : bgcolor;
      } else {
        if (gfxchar) {
          newBits[i + j] = (bmap[fonty] & 0x01) ? fgcolor : bgcolor;
        } else {
          newBits[i + j] = bgcolor;
        }
      }
      mask >>= 1;
    }
    fonty++;
  }
  //UpdateScreen(newBits, x, y, width, height, false);

  for(int i = 0;i < height; i++){
	  for (int j = 0; j < width; j++) {
		  ScreenBitMap[(y * font_height + i) * text_cols * font_width  + x * font_width + j ] = AndroidPalette[newBits[i + j]];
	  }
  }

}



#endif
