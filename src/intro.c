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
#include "vorton.h"
#include <SDL/SDL.h>

#ifdef GP32
#define WIDTH 160
#define HEIGHT 144
#define BITS 16
#define RATE 22050
#define AUDIOBUF 256
#else
#define WIDTH 320
#define HEIGHT 240 
#define RATE 44100
#ifdef WINDOWS
#define AUDIOBUF 2048
#else
#define AUDIOBUF 512
#endif
#define BITS 16
#endif

#define FLCFILE DATA_COMMON "intro.flc"
#define MODFILE DATA_COMMON "intro.mod"
#define TEXTFILE DATA_COMMON "intro.txt"


void intro (void)
{
	SDL_Surface *screen;
	Uint32 video_args=SDL_HWSURFACE|SDL_DOUBLEBUF;
	int bits=BITS;

#ifdef ARGS
	if (fullscreen_arg)
		video_args|=SDL_FULLSCREEN;
	bits=bitscolor_arg;
#endif
	
	screen=SDL_SetVideoMode(WIDTH,HEIGHT,BITS,video_args);
	if (screen!=NULL)
	{
		SDL_ShowCursor(SDL_DISABLE);
#ifndef NOSOUND
#ifdef ARGS
		if (!sound_arg)
			FLI_Play(screen, NULL, FLCFILE, -1, NULL, NULL, TEXTFILE, -1);
		else
#endif
			FLI_Play(screen, NULL, FLCFILE, -1, MODFILE, NULL,  TEXTFILE, -1);
#else
		FLI_Play(screen, NULL, FLCFILE, -1, NULL, NULL, TEXTFILE, -1);
#endif
		SDL_FreeSurface(screen);
	}
}
	
