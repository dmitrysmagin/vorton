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

#define MAX_LANG 5

static char *langs[MAX_LANG]={ "English", "Español", "Deutsch", "Français", "Italiano" };

static inline void show(void)
{
	int i,x,y;

	for (i=1;i<=40;i++)
	{
		for(y=0;y<6;y++)
			for(x=0;x<8;x++)
			{
				SDL_Rect r;
				r.x=160 + (i*(x-4));
				r.y=120 + (i*(y-3));
				r.h=40;
				r.w=40;

				SDL_BlitSurface(image[IMG_LANGUAGE_BACKGROUND],NULL,screen,&r);
			}
		SDL_Flip(screen);
#ifndef GP32
		SDL_Delay(10);
#endif
	}

}

static inline void draw_lang(void)
{
	int x,y;

	for(y=0;y<6;y++)
		for(x=0;x<8;x++)
		{
			SDL_Rect r;
			r.x=x*40;
			r.y=y*40;
			r.h=40;
			r.w=40;

			SDL_BlitSurface(image[IMG_LANGUAGE_BACKGROUND],NULL,screen,&r);
		}
	for(x=0;x<MAX_LANG;x++)
	{
		font_opt_centered(screen,(120/MAX_LANG)+x*(240/(MAX_LANG+1)),langs[x]);
	}
	SDL_Flip(screen);
}

void language(void)
{
	show();
	draw_lang();
	SDL_Delay(2000);
}
