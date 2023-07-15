/*
 * MinGW32 system file
 * based on nix.c and dos.c
 * req's SDL
 * -Dave Kiddell
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *strdup();

#include <3ds.h>

static uint64_t startTicks;
bool startedTicks;

void *sys_timer()
{
	uint32_t *tv;

	if (!startedTicks)
	{
		startTicks = svcGetSystemTick();
		startedTicks = true;
	}
	
	tv = malloc(sizeof *tv);
	//*tv = ((svcGetSystemTick() - startTicks) * 1000 / 268111856) * 1000;
	*tv = ((((svcGetSystemTick() - startTicks) * 1000) * 0x11) >> 32) * 1000;
	return tv;
}

int sys_elapsed(uint32_t *cl)
{
	uint32_t now;
	uint32_t usecs;

	now = ((((svcGetSystemTick() - startTicks) * 1000) * 0x11) >> 32) * 1000;
	usecs = now - *cl;
	*cl = now;
	return usecs;
}

void sys_sleep(int us)
{
	svcSleepThread((uint64_t)(us/1000) * 1000000ULL);
}

void sys_sanitize(char *s)
{
	int i;
	for (i = 0; s[i]; i++)
		if (s[i] == '\\') s[i] = '/';
}

void sys_initpath(char *exe)
{
	char *buf, *home, *p;

	home = strdup(exe);
	sys_sanitize(home);
	p = strrchr(home, '/');
	if (p) *p = 0;
	else
	{
		buf = "sdmc:/3ds/GNUBoy/.";
		rc_setvar("rcpath", 1, &buf);
		rc_setvar("savedir", 1, &buf);
		return;
	}
	buf = malloc(strlen(home) + 8);
	sprintf(buf, ".;%s/", home);
	rc_setvar("rcpath", 1, &buf);
	sprintf(buf, ".", home);
	rc_setvar("savedir", 1, &buf);
	free(buf);
}

void sys_checkdir(char *path, int wr)
{
}


