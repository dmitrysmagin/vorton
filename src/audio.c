/*
 * Vorton - Highway Encounter SDL Remake Game.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
----------------------------------
- Musicas y efectos sonoros -
----------------------------------
*/


/* Definiciones generales */
#include "vorton.h"

/* Nombre de los ficheros musicales */
char *music_names[NUM_MUSICS]={
	DATA_COMMON "loading.mod",
	DATA_COMMON "menu.mod",
	DATA_COMMON "records.mod",
	"level.mod",
};

char *sounds_names[NUM_SOUNDS]={
	DATA_COMMON "laser.wav",
	DATA_COMMON "hit.wav",
	DATA_COMMON "blow.wav",
	DATA_COMMON "expl.wav",
	DATA_COMMON "active.wav",
	DATA_COMMON "thrust.wav",
	DATA_COMMON "welcome.wav",
	DATA_COMMON "bye.wav",
	DATA_COMMON "awesome.wav",
	DATA_COMMON "gameover.wav",
	DATA_COMMON "hurry.wav",
	DATA_COMMON "pause.wav",
	DATA_COMMON "ready.wav",
	DATA_COMMON "ping.wav",
	DATA_COMMON "boom.wav",
	DATA_COMMON "dummy.wav",
	DATA_COMMON "laser2.wav",
};

Mix_Music *music[NUM_MUSICS];
Mix_Chunk *sound[NUM_SOUNDS];

/*
  FUNCION: load_musics
	Carga las musicas.
*/
int load_musics(void)
{
	int i,ini,max;
	char *cad=calloc(1024,1);

	if (!level)
	{
		ini=MUSIC_MENU;
		max=MUSIC_LEVEL;
	}
	else
	{
		ini=MUSIC_LEVEL;
		max=NUM_MUSICS;
	}

	FLI_InitDefaultFont();
	
	if (cad==NULL)
		return 0;

	/* Carga las imagenes */
	for(i=ini;i<max;i++)
	{
		memset(cad,0,1024);
		if (!level)
			strcpy(cad,music_names[i]);
		else
			sprintf(cad,"%s%i%s%s",DATA_PREFIX,level,FILE_SEPARATOR,music_names[i]);
		display_gauge(i-ini,max-ini,cad);
		music[i]=Mix_LoadMUS(cad);
		if (music[i]==NULL)
		{
			free(cad);
			FLI_FreeDefaultFont();
			return 0;
		}
	}
	free(cad);
	FLI_FreeDefaultFont();
	return -1;
}

/*
  FUNCION: load_sounds
	Carga las musicas.
*/
static inline int load_sounds(void)
{
	int i;

	FLI_InitDefaultFont();

	/* Carga las imagenes */
	for(i=0;i<NUM_SOUNDS;i++)
	{
		display_gauge(i,NUM_SOUNDS,sounds_names[i]);
		sound[i]=Mix_LoadWAV(sounds_names[i]);
		if (sound[i]==NULL)
		{
			FLI_FreeDefaultFont();
			return 0;
		}

	}	
	FLI_FreeDefaultFont();
	return -1;
}

/*
  FUNCION: audio_init
	Inicio del mixer.
*/
int audio_init(void)
{
#ifndef NOSOUND
	if (!load_sounds())
		return 0;
	/* Se cargan las musicas */
	return load_musics();
#else
	return 1;
#endif
}


void music_stop(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
	while (Mix_PlayingMusic())
	{
		Mix_HaltMusic();

		SDL_Delay(100);
	}
#ifdef ARGS
	}
#endif
#endif
}

void music_stop_fade(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
		Mix_FadeOutMusic(3000);
#ifdef ARGS
	}
#endif
#endif
}

void music_play(int n_music)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
		music_stop();
		Mix_PlayMusic(music[n_music],-1);
#ifdef ARGS
	}
#endif
#endif
}

void music_play_fade(int n_music)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
		music_stop();
		Mix_FadeInMusic(music[n_music],-1,2000);
#ifdef ARGS
	}
#endif
#endif
}


void play_sound(int n_sound)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
		Mix_PlayChannel(-1, sound[n_sound],0);
#ifdef ARGS
	}
#endif
#endif
}

void play_thrust(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
//		Mix_PlayChannel(0, sound[SND_THRUST],-1);
		Mix_FadeInChannel(1, sound[SND_THRUST],-1,1000);
#ifdef ARGS
	}
#endif
#endif
}

void stop_thrust(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
	Mix_FadeOutChannel(1,1000);
#ifdef ARGS
	}
#endif
#endif
}

void music_up(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
	Mix_VolumeMusic(128);
#ifdef ARGS
	}
#endif
#endif
}

void music_down(void)
{
#ifndef NOSOUND
#ifdef ARGS
	if (sound_arg)
	{
#endif
	Mix_VolumeMusic(96);
#ifdef ARGS
	}
#endif
#endif
}

#define WAIT_TIME_OUT 100

void wait_channels(void)
{
#ifndef NOSOUND
	int i,j;
#ifdef ARGS
	if (sound_arg)
	{
#endif
		for(i=0,j=0;i<4;i++,j=0)
			while((Mix_Playing(i)&&(j<WAIT_TIME_OUT)))
				SDL_Delay(100);
#ifdef ARGS
	}
#endif
#endif
}
