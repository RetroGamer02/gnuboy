/*
 * sdl-audio.c
 * sdl audio interface
 *
 * (C) 2001 Laguna
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "rc.h"
#include "pcm.h"

#include <3ds.h>


struct pcm pcm;

#define SAMPLERATE 22050
#define BYTESPERSAMPLE 4
#define SAMPLESPERBUF (SAMPLERATE / 60)

static int sound = 1;
static int samplerate = SAMPLERATE;

static int stereo = 1;
static volatile int audio_done;
static int paused;

rcvar_t pcm_exports[] =
{
	RCV_BOOL("sound", &sound, "enable sound"),
	RCV_INT("stereo", &stereo, "enable stereo"),
	RCV_INT("samplerate", &samplerate, "samplerate, recommended: 32768"),
	RCV_END
};


/*static void audio_callback(void *blah, byte *stream, int len)
{
	memcpy(stream, pcm.buf, len);
	audio_done = 1;
}*/

/*typedef struct bufferData
{
	byte *stream;
	int len;
} bufData;*/

void fillBufferThread(void *data)
{
	//bufData = (bufferData)data;
	//memcpy(bufData.stream, pcm.buf, bufData.len);
	audio_done = 1;
}

void pcm_init()
{
	int i;
	//SDL_AudioSpec as = {0}, ob;

	if (!sound) return;

	//SDL_InitSubSystem(SDL_INIT_AUDIO);

	ndspWaveBuf waveBuf[2];

	u32 *audioBuffer = (u32*)linearAlloc(SAMPLESPERBUF*BYTESPERSAMPLE*2);

	ndspInit();

	if (stereo)
	{
		ndspSetOutputMode(NDSP_OUTPUT_STEREO);
		ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM8);
	}
	else
	{
		ndspSetOutputMode(NDSP_OUTPUT_MONO);
		ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM8);
	}

	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
	ndspChnSetRate(0, SAMPLERATE);

	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);
	
	memset(waveBuf,0,sizeof(waveBuf));
	waveBuf[0].data_vaddr = &audioBuffer[0];
	waveBuf[0].nsamples = SAMPLESPERBUF;
	waveBuf[1].data_vaddr = &audioBuffer[SAMPLESPERBUF];
	waveBuf[1].nsamples = SAMPLESPERBUF;

	//Thread audioThread = threadCreate(fillBufferThread, &bufData, 1024, 0x32, 0, true);

	/*as.freq = SAMPLERATE;
	as.format = AUDIO_U8;
	as.channels = 1 + stereo;
	as.samples = SAMPLERATE / 60;
	for (i = 1; i < as.samples; i<<=1);
	as.samples = i;
	as.callback = audio_callback;
	as.userdata = 0;*/
	/*if (SDL_OpenAudio(&as, &ob) == -1) {
		sound = 0;
		return;
	}*/

	/*pcm.hz = ob.freq;
	pcm.stereo = ob.channels - 1;
	pcm.len = ob.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);*/

	//SDL_PauseAudio(0);
}

int pcm_submit()
{
	if (!pcm.buf || paused) {
		pcm.pos = 0;
		return 0;
	}
	if (pcm.pos < pcm.len) return 1;
	while (!audio_done)
		SDL_Delay(4);
	audio_done = 0;
	pcm.pos = 0;
	return 1;
}

void pcm_close()
{
	if (sound) ndspExit();
}

void pcm_pause(int dopause)
{
	paused = dopause;
	//SDL_PauseAudio(paused);
}




