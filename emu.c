


#include "defs.h"
#include "regs.h"
#include "hw.h"
#include "cpu.h"
#include "mem.h"
#include "lcd.h"
#include "rc.h"
#include "rtc.h"
#include "sys.h"
#include "sound.h"
#include "cpu.h"

#include <3ds.h>

static int framelen = 16743;
static int framecount;
static int paused;

int dmg_pallete;

rcvar_t emu_exports[] =
{
	RCV_INT("framelen", &framelen, ""),
	RCV_INT("framecount", &framecount, ""),
	RCV_END
};



void emu_pause(int dopause) {
	paused = dopause;
}

int emu_paused(void) {
	return paused;
}

void emu_init()
{
	
}


/*
 * emu_reset is called to initialize the state of the emulated
 * system. It should set cpu registers, hardware registers, etc. to
 * their appropriate values at powerup time.
 */

void emu_reset()
{
	hw_reset();
	lcd_reset();
	cpu_reset();
	mbc_reset();
	sound_reset();
	mem_mapbootrom();
}





void emu_step()
{
	cpu_emulate(cpu.lcdc);
}



/* This mess needs to be moved to another module; it's just here to
 * make things work in the mean time. */

void *sys_timer();

void loadConfig(int *videomode)
{
	int dmg_pallete;

	FILE *config = fopen("sdmc:/3ds/GNUBoy/GNUBoy.cfg", "r");
	if (config)
	{
		fscanf(config, "%d %d",videomode, &dmg_pallete);
		fclose(config);
	} else {
		printf("GNUBoy.cfg not found.\n");
	}
	

	for(int a = 0; a < 4; a++)
	{
		for(int b = 0; b < 4; b++)
		{
			switch (dmg_pallete)
			{
				case 0:
					dmg_pal[a][b] = def_gnu_pal[0][b];
					break;
				case 1:
					dmg_pal[a][b] = def_dmg_pal[0][b];
					break;
				case 2:
					dmg_pal[a][b] = def_pocket_pal[0][b];
					break;
				case 3:
					dmg_pal[a][b] = def_light_pal[0][b];
					break;
				default:
					dmg_pal[a][b] = def_dmg_pal[0][b];
					break;
			}
		}
	}
}

void saveConfig(int *videomode)
{
	FILE *config = fopen("sdmc:/3ds/GNUBoy/GNUBoy.cfg", "w");
	fprintf(config, "%d %d",*videomode, dmg_pallete);
	fclose(config);
}

unsigned int saveCounter = 0;

void saveThreadFunc(void(*))
{
	while(true) {
		saveCounter++;
		if (saveCounter > 130000000)
		{
			sram_save();
			//rtc_save();
			saveCounter = 0;
			//printf("Saved\n");
		}
		
		if (framecount) { if (!--framecount) die("finished\n"); }
	}
}

void emu_run()
{
	void *timer = sys_timer();
	int delay;

	vid_begin();
	lcd_begin();
	//APT_SetAppCpuTimeLimit(10);
	//Thread saveThread = threadCreate(saveThreadFunc, 0x0, 1024, 0x18, 1, true);
	for (;;)
	{
		cpu_emulate(2280);
		while (R_LY > 0 && R_LY < 144)
			emu_step();
		
		vid_end();
		rtc_tick();
		sound_mix();
		if (!pcm_submit())
		{
			delay = framelen - sys_elapsed(timer);
			sys_sleep(delay);
			sys_elapsed(timer);
		}
		doevents();

		//Currently to slow
		/*saveCounter++;
		if (saveCounter > 2000)
		{
			sram_save(); //Added
			rtc_save(); //Added
			saveCounter = 0;
		}*/

		if (paused)	return;
		
		vid_begin();
		if (framecount) { if (!--framecount) die("finished\n"); }
		if (!(R_LCDC & 0x80))
			cpu_emulate(32832);
		
		while (R_LY > 0) /* wait for next frame */
			emu_step();
	}
}












