#ifndef EMU_H
#define EMU_H

void emu_run();
void emu_reset();
void emu_pause(int paused);
int emu_paused(void);

void loadConfig(int *videomode);
void saveConfig(int *videomode);

#endif


