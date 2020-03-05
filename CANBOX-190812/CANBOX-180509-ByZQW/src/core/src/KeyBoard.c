/*
 *
 */

#include "guilib.h"
#include "arch_ra8875.h"

enum {
	BTN_EVT_KEYB_NUM0 = 0,
	BTN_EVT_KEYB_NUM1,
	BTN_EVT_KEYB_NUM2,
	BTN_EVT_KEYB_NUM3,
	BTN_EVT_KEYB_NUM4,
	BTN_EVT_KEYB_NUM5,
	BTN_EVT_KEYB_NUM6,
	BTN_EVT_KEYB_NUM7,
	BTN_EVT_KEYB_NUM8,
	BTN_EVT_KEYB_NUM9,
	BTN_EVT_KEYB_NUMA,
	BTN_EVT_KEYB_NUMB,
	BTN_EVT_KEYB_NUMC,
	BTN_EVT_KEYB_NUMD,
	BTN_EVT_KEYB_NUME,
	BTN_EVT_KEYB_NUMF,
	BTN_EVT_KEYB_BACK,
	BTN_EVT_KEYB_ESC,
	BTN_EVT_KEYB_ENT,
	BTN_EVT_KEYB_MAX
};



/*
 * xxxxxxxxxxxxxxxxx
 * 1   2   3   A   B
 * 4   5   6   C   D
 * 7   8   9   E   F
 * 0  BACK  ESC  ENT
 *
 */
int GUI_GetInput(int wx, int wy, int *dat, int bmax, int mode)
{
	int x, y;
	int k;
	int i, j;
	int cnt = -1;
	int data = *dat;
	int ret;
	int evt, sts;
	___HOTAREA_DEF key_buttons[19] = {
	// x    y    w    h   flags    event
	{   1,  60,  36,  36, 0x0000,  1},
	{   1,  60,  36,  36, 0x0000,  2},
	{   1,  60,  36,  36, 0x0000,  3},
	{   1,  60,  36,  36, 0x0000,  10},
	{   1,  60,  36,  36, 0x0000,  11},
	{   1,  60,  36,  36, 0x0000,  4},
	{   1,  60,  36,  36, 0x0000,  5},
	{   1,  60,  36,  36, 0x0000,  6},
	{   1,  60,  36,  36, 0x0000,  12},
	{   1,  60,  36,  36, 0x0000,  13},
	{   1,  60,  36,  36, 0x0000,  7},
	{   1,  60,  36,  36, 0x0000,  8},
	{   1,  60,  36,  36, 0x0000,  9},
	{   1,  60,  36,  36, 0x0000,  14},
	{   1,  60,  36,  36, 0x0000,  15},
	{   1,  60,  36,  36, 0x0000,  0},
	{   1,  60,  36,  36, 0x0000,  117},
	{   1,  60,  36,  36, 0x0000,  118},
	{   1,  60,  36,  36, 0x0000,  119},
};
	GUI_Can_SetClr(wx,wy,230,240,VM_COLOR_BG);
	
	RA8875_rounded_rectangle(wx, wy, wx+230, wy+230, VM_COLOR_BLUE,10,10,0);

	GUI_DrawRect(wx + 10, wy + 5, 205, 36, VM_COLOR_BLUE, 0x0);
	if (mode == 0)
		GUI_DrawAscii(wx+185, wy + 15, "Hex", VM_COLOR_RED, VM_COLOR_BG);
	else
		GUI_DrawAscii(wx+185, wy + 15, "Dec", VM_COLOR_RED, VM_COLOR_BG);

	RA8875_SetTextZoom(1, 1);
	k = 0;
	y = wy + 5;
	for(i = 0; i < 3; i++) {
		y += 46;
		x = wx + 5;
		for(j = 0; j < 5; j++) {
			GUI_DrawRect( x, y, 36, 36, VM_COLOR_BLUE, 0x0);
			if (mode == 1 && key_buttons[k].ha_event >= 10)
			{
				GUI_DrawHex( x + 10, y + 2, key_buttons[k].ha_event, VM_COLOR_BLACK, VM_COLOR_GRY, 1);
			}
			else
			{
				GUI_DrawHex( x + 10, y + 2, key_buttons[k].ha_event, VM_COLOR_BLACK, VM_COLOR_BG, 1);
			}
			key_buttons[k].ha_x = x;
			key_buttons[k].ha_y = y;
			key_buttons[k].ha_w = 36;
			key_buttons[k].ha_h = 36;
			k++;
			x += 46;
		}
	}

	x = wx + 5;
	y = wy + 5 + 46 * 4;

	GUI_DrawRect(x, y, 36, 36, VM_COLOR_BLUE, 0x0);
	GUI_DrawHex( x + 10, y + 2, key_buttons[k].ha_event, VM_COLOR_BLACK, VM_COLOR_BG, 1);
	key_buttons[k].ha_x = x;
	key_buttons[k].ha_y = y;
	key_buttons[k].ha_w = 36;
	key_buttons[k].ha_h = 36;
	k++;

	x += 46;
	GUI_DrawRect(x, y, 50, 36, VM_COLOR_BLUE, 0x0);
	{
	char ss[2];
	ss[0] = 0x1B;
	ss[1] = 0;
	RA8875_SetTextZoom(3, 1);
	GUI_DrawAscii(x + 7, y + 3, ss, VM_COLOR_BLACK, VM_COLOR_BG);
	}
	RA8875_SetTextZoom(1, 1);
	key_buttons[k].ha_x = x;
	key_buttons[k].ha_y = y;
	key_buttons[k].ha_w = 50;
	key_buttons[k].ha_h = 36;
	k++;
	x += 60;
	GUI_DrawRect(x, y, 50, 36, VM_COLOR_BLUE, 0x0);
	GUI_DrawAscii(x + 1, y + 1, "ESC", VM_COLOR_BLACK, VM_COLOR_BG);
	key_buttons[k].ha_x = x;
	key_buttons[k].ha_y = y;
	key_buttons[k].ha_w = 50;
	key_buttons[k].ha_h = 36;
	k++;
	x += 60;
	GUI_DrawRect(x, y, 54, 36, VM_COLOR_BLUE, 0x0);
	GUI_DrawAscii(x + 2, y + 1, "ENT", VM_COLOR_BLACK, VM_COLOR_BG);
	key_buttons[k].ha_x = x;
	key_buttons[k].ha_y = y;
	key_buttons[k].ha_w = 54;
	key_buttons[k].ha_h = 36;
	k++;

	touchctl_HotArea_Register(key_buttons, COUNTOF(key_buttons));
	if (mode == 0)
	{
		GUI_DrawHex(wx + 20, wy + 6, data, VM_COLOR_BLACK, VM_COLOR_BG, 10);
	}
	else 
	{
		GUI_DrawInt(wx + 20, wy + 6, data, VM_COLOR_BLACK, VM_COLOR_BG, 10);
	}
	while(1) {
		ret = system_event_pop(&evt, &sts);
		if(ret <= 0)
			continue;
		switch(evt) {
		case 117:
			if(cnt > 0) {
				data >>= 4;
				cnt--;
			}
			break;
		case 118:
			ret = 0;
			goto done;
		case 119:
			*dat = data;
			ret = 1;
		
			goto done;
		default:
			if (mode == 1 && (evt >= 10))
				break;
			
			if(cnt < 0)
			{
				data = 0;
				cnt = 0;
			}
			
			if(cnt < bmax)
			{
				if (mode == 0)
					data = (data << 4) | evt;
				else if (mode == 1)
					data = (data * 10) + evt;
				cnt++;
				if(data == 0) cnt = 0;
			}
			break;
		}

		if (mode == 0)
		{
			GUI_DrawHex(wx + 20, wy + 6, data, VM_COLOR_BLACK, VM_COLOR_BG, 10);
		}
		else 
		{
			GUI_DrawInt(wx + 20, wy + 6, data, VM_COLOR_BLACK, VM_COLOR_BG, 10);
		}
	}

done:
	touchctl_HotArea_Dregister(key_buttons);
	GUI_DrawRect(wx, wy, 230, 230, VM_COLOR_BG, 0x80);
	RA8875_SetTextZoom(0, 0);
	touchctl_HotArea_Dregister(key_buttons);
	return ret;
}

