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

#ifndef VORTON_H
#define VORTON_H

#define VORTON_VERSION "Beta-1"


/*
------------------------------------
- Definiciones Generales del Juego -
------------------------------------
*/
#ifdef DREAMCAST
#include <kos.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "SDL_flic.h"
#include "SFont.h"

#ifdef DREAMCAST
#define SDL_Delay timer_spin_sleep
#define SDL_GetTicks timer_ms_gettime64
#endif

#define DATA_COMMON DATA_PREFIX "common" FILE_SEPARATOR

/* Enumeracion para la carga de imagenes */
enum {
	IMG_LOGO,
	IMG_RECORDS_BACKGROUND_0,
	IMG_RECORDS_BACKGROUND_1,
	IMG_LANGUAGE_BACKGROUND,
	IMG_STAR,
	IMG_MENU,
	IMG_FONT,
	IMG_NUMBERS,
	IMG_FONT_OPTIONS,
	IMG_FONT_GAME,
	IMG_BALL,
	NUM_IMAGES
};

enum{
	MUSIC_LOADING,
	MUSIC_MENU,
	MUSIC_RECORDS,
	MUSIC_LEVEL,
	NUM_MUSICS
};

enum{
	SND_LASER,
	SND_HIT,
	SND_BLOW,
	SND_EXPL,
	SND_ACTIVE,
	SND_THRUST,
	SND_WELCOME,
	SND_BYE,
	SND_AWESOME,
	SND_GAMEOVER,
	SND_HURRY,
	SND_PAUSE,
	SND_READY,
	SND_PING,
	SND_BOOM,
	SND_DUMMY,
	SND_SHIP_LASER,
	NUM_SOUNDS
};

#ifdef ARGS

/*Argumento Sonido*/
extern int sound_arg;

/* Argumento Fullscreen */
extern int fullscreen_arg;

/* Argumento bitscolor */
extern int bitscolor_arg;

/* Argumento mono */
extern int monosound_arg;

/* Argumento_lowsound */
extern int lowsound_arg;

#endif


/* Numero de vidas */
extern int lives;

/* Nivel actual */
extern int level;

/* Tiempo */
extern int game_time;

/* Fin de proceso */
extern int theend;
extern int completed;
extern Uint32 frame;

/* Puntos */
extern int score;

extern char* tmp_string;

/* Superficies */
extern SDL_Surface *screen, *image[NUM_IMAGES];

extern Mix_Music *music[NUM_MUSICS];
extern Mix_Chunk *sound[NUM_SOUNDS];

#ifndef GP32
extern SDL_Joystick *joy;
#endif


/* Iniciacion de video */
extern void display_gauge(int i, int max, char *cad);
extern void display_gauge_string(int i, int max, char *cad);
extern void display_gauge_only(char *cad);
extern int video_init(void);
extern int load_images(void);

extern int audio_init(void);
extern int load_musics(void);

extern void music_play(int n_music);
extern void music_play_fade(int n_music);
extern void music_stop(void);
extern void music_stop_fade(void);
extern void play_sound(int n_sound);
extern void play_thrust(void);
extern void stop_thrust(void);
extern void music_down(void);
extern void music_up(void);
extern void wait_channels(void);

extern void language(void);
extern void records(void);

extern void init_display_error(void);
extern void invert_screen_color(void);
extern void display_error(char *);
#define display_SDL_error() (display_error(SDL_GetError()))
extern void display_error_invert(char *);
#define display_SDL_error_invert() (display_error_invert(SDL_GetError()))

extern SFont_FontInfo font,fontgame,font_opt,fontnumber;

#define fontgame_centered(SCR,Y,MSG) (SFont_XCenteredStringInfo(SCR,&fontgame,Y,MSG))
#define font_opt_centered(SCR,Y,MSG) (SFont_XCenteredStringInfo(SCR,&font_opt,Y,MSG))
#define font_centered(SCR,Y,MSG) (SFont_XCenteredStringInfo(SCR,&font,Y,MSG))
#define fontnumber_centered(SCR,Y,MSG) (SFont_XCenteredStringInfo(SCR,&fontnumber,Y,MSG))

#define fontnumber_put(SCR,X,Y,MSG) (SFont_PutStringInfo(SCR,&fontnumber,X,Y,MSG))
#define fontgame_put(SCR,X,Y,MSG) (SFont_PutStringInfo(SCR,&fontgame,X,Y,MSG))
#define font_opt_put(SCR,X,Y,MSG) (SFont_PutStringInfo(SCR,&font_opt,X,Y,MSG))
#define font_put(SCR,X,Y,MSG) (SFont_PutStringInfo(SCR,&font,X,Y,MSG))

/* Variables Globales */
extern int left_pressed;		/* Booleano: Pulsado pad izquierda */
extern int right_pressed;		/* Booleano: Pulsado pad derecha */
extern int up_pressed;			/* Booleano: Pulsado pad arriba */
extern int down_pressed;		/* Booleano: Pulsado pad abajo */
extern int hit1_pressed;		/* Booleano: Pulsado disparo 1 */
extern int hit2_pressed;		/* Booleano: Pulsado disparo 2 */
extern int return_pressed;		/* Booleano: Pulsado Start */
extern int space_pressed;		/* Booleano: Pulsado Select */
extern int tab_pressed;			/* Booleano: Pulsado L */
extern int backspace_pressed;		/* Booleano: Pulsado R */

#ifndef DREAMCAST
#define direct_blit(SRC,DST,RECT)	SDL_BlitSurface(SRC,NULL,DST,RECT)
#else
static inline void direct_blit(SDL_Surface *src, SDL_Surface *dst, SDL_Rect *r)
{
	int i,j,x=0,y=0;
	int rx=r->x,ry=r->y,rh=r->h,rw=r->w;
	void *s,*d;

	if (rw>dst->w)
		rw=dst->w;
	if (rh>dst->h)
		rh=dst->h;

	if (rx<0)
	{
		rw+=rx;
		x=-rx;
		rx=0;
	}

	if (rx<dst->w)
	{
		if (rx+rw > dst->w)
			rw=dst->w - rx;
		if (rw > 0)
		{
			if (ry<0)
			{
				rh+=ry;
				y=-ry;
				ry=0;
			}
			if (ry<dst->h)
			{
				if (ry+rh > dst->h)
					rh=dst->h - ry;
				if (rh > 0)
				{
					s=src->pixels + (src->pitch * y) + (x<<1);
					d=dst->pixels + (dst->pitch * ry) + (rx<<1);
					for(i=0;i<rh;i++,d+=dst->pitch,s+=src->pitch)
					{
						Uint16 *dd=d,*ds=s;
						for(j=0;j<rw;j++)
							*dd++=*ds++;
					}
				}
			}
		}
	}
}
#endif

extern int GET_RAND[];
extern int get_rand_index;

#define GET_RAND_MAX 16384

static inline int getRand()
{
	get_rand_index=((get_rand_index+1)%GET_RAND_MAX);
	return GET_RAND[get_rand_index];
}

#endif
