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

#define MAX_MSG 70
#define MIN_SCROLL (-(MAX_MSG*32))

static char *exit_msg[MAX_MSG]=
{
	"",
	"",
	"Programación:",
	"- Chui -",
	"- Race8086 -",
	"- Yota -",
	"- Mortimor -",
	"",
	"",
	"",
	"Música:",
	"- Xenon -",
	"- DJ Syto -",
	"",
	"",
	"",
	"Gráficos:",
	"- Nacho -",
	"- Chui -",
	"- Etece -",
	"",
	"",
	"",
	"Mapas:",
	"- Enkonsierto -",
	"",
	"",
	"",
	"Alojamiento:",
	"- SourceForge -",
	"- GP32Spain -",
	"",
	"",
	"",
	"Agradecimientos:",
	"- Anarchy -",
	"- Ryleh -",
	"- TDP -",
	"- Fiera Soul -",
	"- RobBrown -",
	"- Atrkid -",
	"- Mr.Spiv -",
	"- Uint3 -",
	"- Pekele -",
	"y demás GP32ros",
	"",
	"",
	"",
	"",
	"",
	"",
	"GPL License",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"---X---",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

static inline int WaitKey(void)
{
	int ret=0;
	SDL_Event event;
	
	/* Poll for events */
	while( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			/* Keyboard event */
			case SDL_JOYBUTTONDOWN:
			case SDL_KEYDOWN:
			case SDL_QUIT:
				music_down();
				music_stop_fade();
				play_sound(SND_BYE);
				ret = -1;
				break;
//			default:
		} 
	}

	return ret;
}	

void exit_game(void)
{
	int i,x,y,max;
	SDL_Rect r;

	for(i=0;i<10;i++)
	{
		WaitKey();
		SDL_Delay(10);
	}

	x=0;
	y=(screen->h)-1;
	max=y+32;
	while(!WaitKey())
	{
		r.x=x;
		r.y=0;
		r.w=image[IMG_MENU]->w;
		r.h=image[IMG_MENU]->h;
direct_blit(image[IMG_MENU],screen,&r);
//		SDL_BlitSurface(image[IMG_MENU], &r, screen, NULL);
		r.x=screen->w+x;
direct_blit(image[IMG_MENU],screen,&r);
//		SDL_BlitSurface(image[IMG_MENU], &r, screen, NULL);
		for(i=0;i<MAX_MSG;i++)
		{
			int yy=y+(i<<5);
			if (yy<max)
			{
				if (yy>-32)
					font_opt_centered(screen,yy,exit_msg[i]);
			}
			else
				break;
		}
		SDL_Flip(screen);
#ifndef GP32
		SDL_Delay(10);
#endif
		y--;
		if (y<MIN_SCROLL)
			y=(screen->h)-1;
		x--;
		if (x<-screen->w)
			x=0;

	}
	wait_channels();
}
