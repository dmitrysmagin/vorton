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

#define MAX_RECORDS 5

static char *record[MAX_RECORDS]={ "JUAN", "PEPE", "PEDRO", "LUIS", "DAVID" };

static char bpos_x[160]={
		24,23,23,23,23,23,23,23,22,22,22,21,21,20,20,19,
		19,18,18,17,16,16,15,14,14,13,12,11,10,10,9,8,
		7,6,5,4,3,2,1,0,0,0,-1,-2,-3,-4,-5,-6,
		-7,-8,-9,-10,-10,-11,-12,-13,-14,-14,-15,-16,-16,-17,-18,-18,
		-19,-19,-20,-20,-21,-21,-22,-22,-22,-23,-23,-23,-23,-23,-23,-23,
		-24,-23,-23,-23,-23,-23,-23,-23,-22,-22,-22,-21,-21,-20,-20,-19,
		-19,-18,-18,-17,-16,-16,-15,-14,-14,-13,-12,-11,-10,-10,-9,-8,
		-7,-6,-5,-4,-3,-2,-1,0,0,0,1,2,3,4,5,6,
		7,8,9,10,10,11,12,13,14,14,15,16,16,17,18,18,
		19,19,20,20,21,21,22,22,22,23,23,23,23,23,23,23,
	};
static char bpos_y[160]={
		0,0,1,2,3,4,5,6,7,8,9,10,10,11,12,13,
		14,14,15,16,16,17,18,18,19,19,20,20,21,21,22,22,
		22,23,23,23,23,23,23,23,24,23,23,23,23,23,23,23,
		22,22,22,21,21,20,20,19,19,18,18,17,16,16,15,14,
		14,13,12,11,10,10,9,8,7,6,5,4,3,2,1,0,
		0,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-11,-12,-13,
		-14,-14,-15,-16,-16,-17,-18,-18,-19,-19,-20,-20,-21,-21,-22,-22,
		-22,-23,-23,-23,-23,-23,-23,-23,-24,-23,-23,-23,-23,-23,-23,-23,
		-22,-22,-22,-21,-21,-20,-20,-19,-19,-18,-18,-17,-16,-16,-15,-14,
		-14,-13,-12,-11,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,
	};

static inline void show(void)
{
	int i,j,k;
	Uint8 *buf8=(Uint8 *)screen->pixels;
	Uint16 *buf16=(Uint16 *)screen->pixels;
	Uint32 *buf32=(Uint32 *)screen->pixels;

	for(k=0;k<3;k++)
		for(i=63;i>1;i-=3)
		{
			switch(screen->format->BitsPerPixel)
			{
				case 8:
					for(j=k;j<320*240;j+=i)
						buf8[j]=0;
					break;
				case 16:
					for(j=k;j<320*240;j+=i)
						buf16[j]=0;
					break;
				case 32:
					for(j=k;j<320*240;j+=i)
						buf32[j]=0;
					break;
			}
			SDL_Flip(screen);
#ifndef GP32
			SDL_Delay(10);
#endif
		}
			
}


static int star_x[32]={
	271, 159, 316, 183, 131, 247, 135, 159, 71, 159, 159, 103, 159, 159,
	235, 174, 234, 159, 19, 257, 258, 304, 117, 249, 175, 269, 2, 159,
	139, 135, 270, 158  };

static int star_y[32]={
	87, 71, 119, 215, 147, 75, 115, 119, 229, 206, 119, 231, 109, 11, 214,
	209, 119, 179, 119, 217, 152, 177, 119, 179, 159, 174, 119, 191, 114,
	125, 230, 121  };

static int star_ax[32]={
	7, 1, 1, 1, -1, 4, -6, 1, -4, 1, 1, -2, 0, 0, 4, 1, 3, 1, -4, 7, 3, 5,
	-3, 6, 2, 2, -1, 1, -4, -4, 3, -1,  };

static int star_ay[32]={
	-2, -3, 0, 4, 1, -2, -1, 0, 5, 1, 0, 4, -1, -3, 5, 6, 0, 5, 0, 7, 1, 2,
	0, 4, 5, 1, 0, 1, -1, 1, 3, 2,  };

#include <math.h>

static inline void draw_stars(void)
{
	int i;
	SDL_Rect r;

	r.h=3;
	r.w=3;

	for(i=0;i<32;i++)
	{
		star_x[i]+=star_ax[i];
		star_y[i]+=star_ay[i];
		if ((star_x[i]<0)||(star_x[i]>320)||(star_y[i]<0)||(star_y[i]>240))
		{
			star_x[i]=159;
			star_y[i]=119;
			register int ax=(getRand()&0x3);
			register int ay=(getRand()&0x3);

			if (!ax && !ay)
				star_ax[i]=-5000;
			else
			switch(getRand()&0x7)
			{
				case 0:
					star_ax[i]=2+ax;
					star_ay[i]=1+ay;
					break;
				case 1:
					star_ax[i]=-2-ax;
					star_ay[i]=1+ay;
					break;
				case 2:
					star_ax[i]=2+ax;
					star_ay[i]=-1-ay;
					break;
				case 3:
					star_ax[i]=-2-ax;
					star_ay[i]=-1-ay;
					break;
				case 4:
					star_ax[i]=1+ax;
					star_ay[i]=2+ay;
					break;
				case 5:
					star_ax[i]=-1-ax;
					star_ay[i]=2+ay;
					break;
				case 6:
					star_ax[i]=1+ax;
					star_ay[i]=-2-ay;
					break;
				default:
					star_ax[i]=-1-ax;
					star_ay[i]=-2-ay;
					break;
			}
		}
		else
		{
			r.x=star_x[i];
			r.y=star_y[i];
//			SDL_BlitSurface(image[IMG_STAR],NULL,screen,&r);
			direct_blit(image[IMG_STAR],screen,&r);
		}
	}
}

static inline void draw_records(void)
{
	int x,y;
	static int pos=0;
	SDL_Rect r;

	for(y=-1;y<7;y++)
		for(x=-1;x<9;x++)
		{
			r.x=bpos_x[pos]+(x*40);
			r.y=bpos_y[pos]+(y*40);
			r.h=40;
			r.w=40;
//			SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_0],NULL,screen,&r);
			direct_blit(image[IMG_RECORDS_BACKGROUND_0],screen,&r);
		}

	for(y=0;y<6;y++)
	{
			r.x=bpos_x[pos]+(y*53);
			r.y=bpos_y[pos]+(y*40);
			r.h=40;
			r.w=40;
//			SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_0],NULL,screen,&r);
			direct_blit(image[IMG_RECORDS_BACKGROUND_0],screen,&r);
			r.x=bpos_x[pos]+267+(y*53);
			r.y=bpos_y[pos]+(y*40);
//			SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_0],NULL,screen,&r);
			direct_blit(image[IMG_RECORDS_BACKGROUND_0],screen,&r);
			r.x=bpos_x[pos]+100+(y<<4);
			r.y=bpos_y[pos]+(y*40);
//			SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_0],NULL,screen,&r);
			direct_blit(image[IMG_RECORDS_BACKGROUND_0],screen,&r);
			r.x=bpos_x[pos]+267-(y*53);
			r.y=bpos_y[pos]+(y*40);
//			SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_0],NULL,screen,&r);
			direct_blit(image[IMG_RECORDS_BACKGROUND_0],screen,&r);
	}

	r.x=bpos_x[pos]+174;
	r.y=bpos_y[pos]+32;
	r.h=40;
	r.w=40;
//	SDL_BlitSurface(image[IMG_RECORDS_BACKGROUND_1],NULL,screen,&r);
	direct_blit(image[IMG_RECORDS_BACKGROUND_1],screen,&r);

	draw_stars();

	for(x=0;x<MAX_RECORDS;x++)
	{
		fontgame_put(screen,10,(120/MAX_RECORDS)+x*(240/(MAX_RECORDS+1)),record[x]);
		fontgame_put(screen,220,(120/MAX_RECORDS)+x*(240/(MAX_RECORDS+1)),"1000");
	}

	SDL_Flip(screen);
	pos=((pos+1)%160);

}

void records(void)
{
	SDL_Event event;

	music_stop_fade();
	show();
	music_play_fade(MUSIC_RECORDS);
	draw_records();
	while(SDL_PollEvent(&event)>0);
	while(!SDL_PollEvent(&event))
	{
		draw_records();
#ifndef GP32
		SDL_Delay(30);
#else
		SDL_Delay(10);
#endif
	}
	music_stop_fade();
	SDL_Delay(200);
	while(SDL_PollEvent(&event)>0);
}
