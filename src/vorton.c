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
-------------
- Principal -
-------------
*/

/* Definiciones generales */
#include "vorton.h"
#include "filmat.h"
#include "phase2.h"

#ifdef DREAMCAST
extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT);
KOS_INIT_ROMDISK(romdisk);
#endif

#ifndef GP32
SDL_Joystick *joy;
#endif

/* Numero de vidas */
int lives=4;

/* Puntos */
int score=0;

/* Nivel actual */
int level=1;

int game_time=100;

int left_pressed=0;             /* Booleano: Pulsado pad izquierda */
int right_pressed=0;            /* Booleano: Pulsado pad derecha */
int up_pressed=0;               /* Booleano: Pulsado pad arriba */
int down_pressed=0;             /* Booleano: Pulsado pad abajo */
int hit1_pressed=0;             /* Booleano: Pulsado disparo 1 */
int hit2_pressed=0;             /* Booleano: Pulsado disparo 2 */
int return_pressed=0;           /* Booleano: Pulsado Start */
int space_pressed=0;            /* Booleano: Pulsado Select */
int tab_pressed=0;              /* Booleano: Pulsado L */
int backspace_pressed=0;        /* Booleano: Pulsado R */
int theend=0;		/* Booleano: Fin de proceso */
int completed=0;	/* Booleano: Fin de proceso */

Uint32 frame=0;		/* Numero de frames */

static int level_loaded=0;


#ifdef ARGS

/*Argumento Sonido*/
int sound_arg=1;

/* Argumento Fullscreen */
int fullscreen_arg=1;

/* Argumento bitscolor */
int bitscolor_arg=16;

/* Argumento mono */
int monosound_arg=0;

/* Argumento_lowsound */
int lowsound_arg=0;

void parse_args(int n_arg, char **argv)
{
	int i;
	for(i=1;i<n_arg;i++)
		if (!strcmp(argv[i],"-sound"))
			sound_arg=1;
		else
		if (!strcmp(argv[i],"-nosound"))
			sound_arg=0;
		else
		if (!strcmp(argv[i],"-highsound"))
			lowsound_arg=0;
		else
		if (!strcmp(argv[i],"-lowsound"))
			lowsound_arg=1;
		else
		if (!strcmp(argv[i],"-monosound"))
			monosound_arg=1;
		else
		if (!strcmp(argv[i],"-stereosound"))
			monosound_arg=0;
		else
		if (!strcmp(argv[i],"-windowscreen"))
			fullscreen_arg=0;
		else
		if (!strcmp(argv[i],"-fullscreen"))
			fullscreen_arg=1;
		else
		if (!strcmp(argv[i],"-8bits"))
			bitscolor_arg=8;
		else
		if (!strcmp(argv[i],"-16bits"))
			bitscolor_arg=16;
		else
		if (!strcmp(argv[i],"-32bits"))
			bitscolor_arg=32;
		else
		{
			printf("\nVorton %s version.\n\t\t\t\t\t\t\tHighway Devs. 2004.\n\n\t-sound\t\tPlay with sound\n\t-nosound\tPlay without sound\n\t-highsound\tAudio at 44100Hz\n\t-lowsound\tAudio at 22050Hz\n\t-monosound\tMono Audio\n\t-stereosound\tStereo Audio\n\t-windowscreen\tWindowed Video Mode\n\t-fullscreen\tFullscreen Video Mode\n\t-8bits\t\t8  bits color screen\n\t-16bits\t\t16 bits color screen\n\t-32bits\t\t32 bits color screen\n\n\nThis game is free software; you can redistribute it and/or modify it under\nthe terms of the GNU General Public License as published by the Free Software\nFoundation; either version 2 of the License, or (at your option) any later\nversion.  To view the licence online, go to:\n\t\t\t\t\thttp://www.gnu.org/copyleft/gpl.html \n",VORTON_VERSION);
			exit(0);
		}
}
#endif

/*
  FUNCION: init
	Iniciacion del juego
*/
static inline int init(void)
{
	level=0;
	level_loaded=0;
	if (!video_init())
		return 0;
#ifdef ARGS
	if (sound_arg)
#endif
#ifndef NOSOUND
		if (!audio_init())
			return 0;
#endif
	level=1;
	return -1;
}

static inline int process_init(int l)
{
	if (l&1)
		return filmat_process_init();
	return phase2_process_init();
}

static inline void process()
{
	if (level&1)
		filmat_process();
	else
		phase2_process();
}

static inline int load_level(int l)
{
	if ((level==l)&&(level_loaded))
		return -1;
	music_play(MUSIC_LOADING);
	level_loaded=0;
	level=l;
	if (level_loaded)
	{
		if (l&1)
			filmat_unload();
		else
			phase2_unload();
		level_loaded=0;
	}
	if (l&1)
	{
		if (!filmat_load_images())
			return 0;
	}
	else
	{
		if (!phase2_load_images())
			return 0;
	}
#ifdef ARGS
	if (sound_arg)
#endif
#ifndef NOSOUND
		if (!load_musics())
			return 0;
#endif
	return -1;
}

void intro (void);
int selection (int mus);
void exit_game(void);

static inline int new_game(int l)
{
	if (load_level(l))
	{
		if (process_init(l))
		{
			display_error_invert("Parse Error");
			return -1;
		}
		level_loaded=-1;
		process();
		return 0;
	}
	display_SDL_error_invert();
	return -1;
}

static inline int continue_game(void)
{
	return new_game(2);
}

/*
  FUNCION: quit
	Finalizacion del juego
*/
static inline void quit(void)
{
	SDL_Quit();
}


#ifdef GP32
#define RATE 22050
#define AUDIOBUF 256
#else
#define RATE 44100
#ifdef WINDOWS
#define AUDIOBUF 2048
#else
#define AUDIOBUF 512
#endif
#endif

extern char *music_names[];

/*
  FUNCION: GpMain/main
	Funcion principal
*/
#ifdef GP32
void GpMain(void *argv)
#else
int main(int n_arg, char **argv)
#endif
{
	int exit_main_loop=0, rate=RATE, mus=0;
	Uint32 sdl_arg=SDL_INIT_VIDEO;
#ifndef GP32
	sdl_arg|=SDL_INIT_JOYSTICK;
#endif
#ifdef ARGS
	parse_args(n_arg,argv);

	if (sound_arg)
		sdl_arg|=SDL_INIT_AUDIO;
#else
	sdl_arg|=SDL_INIT_AUDIO;
#endif
	if (SDL_Init(sdl_arg))
	{
#ifndef GP32
		printf("SDL not open: %s\n",SDL_GetError());
		return -1;
#endif
	}
	init_display_error();
#ifndef NOSOUND
#ifdef ARGS
	if (lowsound_arg)
		rate=22050;
	if (sound_arg)
	{
#endif
		if (Mix_OpenAudio(rate, AUDIO_S16, 2, AUDIOBUF))
		{
#ifndef GP32
			printf("Mixer not open: %s\n",SDL_GetError());
#ifdef ARGS
			sound_arg=0;
			puts("Audio disabled");
#else
			return -2;
#endif
#endif
		}
		if ((music[MUSIC_LOADING]=Mix_LoadMUS(music_names[MUSIC_LOADING]))==NULL)
		{
			screen=SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE);
			SDL_FillRect(screen,NULL,2);
			display_SDL_error();
#ifndef GP32
			return -3;
#endif
		}
#ifdef ARGS
	}
#endif
#endif
#ifndef GP32
	SDL_JoystickEventState(SDL_ENABLE);
	joy = SDL_JoystickOpen(0);
#endif
	intro();

	music_play(MUSIC_LOADING);

	if (init())
	{
		play_sound(SND_WELCOME);
		while(!exit_main_loop)
		{
			switch (selection(mus))
			{
				case 0:
					exit_main_loop=new_game(1);
					mus=0;
					break;
				case 1:
					exit_main_loop=continue_game();
					mus=0;
					break;
				case 2:
					language();
					mus=1;
					break;
				case 3:
					records();
					mus=0;
					break;
				default:
					exit_game();
					exit_main_loop=1;
			}
		}
	}
	else
		display_SDL_error_invert();

	quit();

#ifdef GP32
	asm("mov        r3,#0; bx       r3;");
#else
	return 0;
#endif
}

