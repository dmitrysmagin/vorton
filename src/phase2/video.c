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


#include "phase2.h"

static char* phase2_image_names[PH2_NUM_IMAGES]={
        "ship0.png", 
        "ship1.png",
        "ship2.png",
        "ship3.png",
        "ship4.png",
        "shadow0.png", 
        "shadow1.png",
        "shadow2.png",
        "shoot.png", 
        "bad.png", 
        "backgrn0.png",
        "backgrn1.png",
        "backgrn2.png",
        "backgrn3.png",
        "backgrn4.png",
        "backgrn5.png",
        "backgrn6.png",
        "backgrn7.png",
        "sky.png", 
};

SDL_Surface *phase2_image[PH2_NUM_IMAGES];

mode7_object phase2_shoot, phase2_bad;


static inline void draw_shadow(int x, int y)
{
	SDL_Rect r;
	int i=1+((151-y)/26);
	SDL_Surface **shadow=&phase2_image[PH2_IMG_SHADOW_0];
	r.x=x+20; // +230-(x>>1)-(y>>1);
	r.y=248-(y>>3); //144+(y>>1);
	r.w=shadow[i]->w;
	r.h=shadow[i]->h;
	SDL_BlitSurface(shadow[i], NULL, screen, &r);
}

static inline void draw_ship(int x, int y)
{
	int wi=(x-64)/26;
	SDL_Rect r;
	SDL_Surface *s=phase2_image[PH2_IMG_SHIP_0+wi];

	r.x=x;
	r.y=y;
	r.h=s->h;
	r.w=s->w;
	SDL_BlitSurface(s, NULL, screen, &r);
}


static inline void draw_sky(int x)
{
	static int mul[]={-PH2_SKY_WIDTH, 0, PH2_SKY_WIDTH, PH2_SKY_WIDTH * 2, 
			PH2_SKY_WIDTH * 3, PH2_SKY_WIDTH * 4, PH2_SKY_WIDTH * 5,
			PH2_SKY_WIDTH * 6, PH2_SKY_WIDTH * 7, PH2_SKY_WIDTH * 8,
			PH2_SKY_WIDTH * 9, PH2_SKY_WIDTH * 10, PH2_SKY_WIDTH * 11 };
	SDL_Rect r;
	int x_init=PH2_SKY_WIDTH-((x>>2)&PH2_SKY_MASK);
	int i;
	int y_init=(mode7_z>>5)-16;

	r.w=PH2_SKY_WIDTH;
	r.h=PH2_SKY_HEIGHT;

	for(i=0;i<=PH2_SKY_ITER;i++)
	{
		r.y=y_init;
		r.x=x_init+mul[i];
//		SDL_BlitSurface(phase2_image[PH2_IMG_SKY],NULL,screen,&r);
		direct_blit(phase2_image[PH2_IMG_SKY],screen,&r);
	}
}

int phase2_load_images(void)
{
	char *cad=calloc(1024,1);
	SDL_Surface *tmp;
	int i;

	if (cad==NULL)
		return 0;

	FLI_InitDefaultFont();

	/* Carga las imagenes */
	for(i=0;i<PH2_NUM_IMAGES;i++)
	{
		memset(cad,0,1024);
		if (i<PH2_IMG_BAD)
			sprintf(cad,"%s%s",PH2_DATA_COMMON,phase2_image_names[i]);
		else
			sprintf(cad,"%s%i%s%s",DATA_PREFIX,level,FILE_SEPARATOR,phase2_image_names[i]);
		display_gauge(i,PH2_NUM_IMAGES,cad);
		tmp=IMG_Load(cad);
		if (tmp==NULL)
		{
			free(cad);
			FLI_FreeDefaultFont();
			return 0;
		}

		/* Optimiza la imagen cargada */
		phase2_image[i]=SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		
		/* Se establece el color transparente negro (RGB:0,0,0 = 0) */
		if (i<PH2_IMG_SHOOT)
#ifndef DREAMCAST
			SDL_SetColorKey(phase2_image[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
#else
			SDL_SetColorKey(phase2_image[i],SDL_SRCCOLORKEY, 0);
#endif
	}	
	free(cad);
	phase2_shoot.img=phase2_image[PH2_IMG_SHOOT];
	phase2_bad.img=phase2_image[PH2_IMG_BAD];

	FLI_FreeDefaultFont();
	return -1;
}

void phase2_unload(void)
{
	int i;

	for(i=0;i<PH2_NUM_IMAGES;i++)
		SDL_FreeSurface(phase2_image[i]);

	mode7_free();
}

void phase2_draw(void)
{
//SDL_FillRect(screen,NULL,255);
#ifdef USE_FIXED_POINT
        draw_sky(sllrotr(mode7_angle,23));
#else
	draw_sky(sll2int(mode7_angle*300.0));
#endif

        mode7(mode7_angle, mode7_x, mode7_y);
	draw_shadow(phase2_ship_x, phase2_ship_y);
	if (phase2_shoot.state<25)
	{
		mode7_draw_object(&phase2_shoot);
		phase2_shoot.x=slladd(phase2_shoot.x, phase2_shoot.dx);
		phase2_shoot.y=slladd(phase2_shoot.y, phase2_shoot.dy);
		phase2_shoot.state--;
		if (!phase2_shoot.state)
			phase2_shoot.state=100;
	}
	if (phase2_bad.state<1)
		phase2_bad.state=105;
	else
	if (phase2_bad.state<90)
	{
		mode7_draw_object(&phase2_bad);
		phase2_bad.x=slladd(phase2_bad.x, phase2_bad.dx);
		phase2_bad.y=slladd(phase2_bad.y, phase2_bad.dy);
	}
	else
		if (phase2_bad.state>100)
		{
			phase2_bad.dx=sllmul(mode7_dx,int2sll(-1));
			phase2_bad.dy=sllmul(mode7_dy,int2sll(-1));
			phase2_bad.x=sllsub(mode7_x,sllmul(phase2_bad.dx,int2sll(120)));
			phase2_bad.y=sllsub(mode7_y,sllmul(phase2_bad.dy,int2sll(120)));
			phase2_bad.alt=(rand()&3)<<3;
		}
	phase2_bad.state--;
	draw_ship(phase2_ship_x, phase2_ship_y);
	SDL_Flip(screen);
}
