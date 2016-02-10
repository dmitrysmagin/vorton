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
- Calculo perspectiva y Dibujado -
----------------------------------
*/


/* Definiciones generales */
#include "vorton.h"

char *tmp_string=NULL;

/* Nombre de los ficheros imagenes */
char *image_names[NUM_IMAGES]={
	"logo.png",
	"recordb0.png",
	"recordb1.png",
	"langbkgr.png",
	"star.png",
	"menu.png",
	"font.png",
	"numbers.png",
	"font_opt.png",
	"fontgame.png",
	"ball.png",
};

/* Superficies */
SDL_Surface *screen, *image[NUM_IMAGES];
SFont_FontInfo font,fontgame,font_opt,fontnumber;

static char* loading_string="Loading";

void display_gauge(int i, int max, char *cad)
{
	SDL_Rect r;

	SDL_FillRect(screen, NULL, -1);
	if (image[IMG_LOGO]!=NULL)
	{
		r.x=(screen->w-image[IMG_LOGO]->w)>>1;
		r.y=(screen->h-image[IMG_LOGO]->h)>>1;
		r.w=image[IMG_LOGO]->w;
		r.h=image[IMG_LOGO]->h;
		SDL_BlitSurface(image[IMG_LOGO],NULL,screen,&r);
	}
	r.x=0;
	r.y=screen->h-24;
	r.w=screen->w;
	r.h=24;
#ifdef GP32
	SDL_FillRect(screen,&r,2);
#else
	SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,0,0,128));
#endif
	r.x+=2;
	r.y+=2;
	r.h-=4;
	r.w=((screen->w*(i+1))-4)/max;
	SDL_FillRect(screen,&r,0);
	SFont_PutString(screen,4,r.y+4,loading_string);
	SFont_PutString(screen,SFont_TextWidth(loading_string)+8,r.y+4,cad);
	SDL_Flip(screen);
#ifndef GP32
	SDL_Delay(10);
#endif
}

void display_gauge_only(char *cad)
{
	SDL_Rect r;

	FLI_InitDefaultFont();
	SDL_FillRect(screen, NULL, -1);
	if (image[IMG_LOGO]!=NULL)
	{
		r.x=(screen->w-image[IMG_LOGO]->w)>>1;
		r.y=(screen->h-image[IMG_LOGO]->h)>>1;
		r.w=image[IMG_LOGO]->w;
		r.h=image[IMG_LOGO]->h;
		SDL_BlitSurface(image[IMG_LOGO],NULL,screen,&r);
	}
	r.x=0;
	r.y=screen->h-24;
	r.w=screen->w;
	r.h=24;
#ifdef GP32
	SDL_FillRect(screen,&r,2);
#else
	SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,0,0,128));
#endif
	r.x+=2;
	r.y+=2;
	r.h-=4;
	r.w=320;
	SDL_FillRect(screen,&r,0);
	SFont_XCenteredString(screen, r.y+4, cad);
	SDL_Flip(screen);
#ifndef GP32
	SDL_Delay(10);
#endif
	FLI_FreeDefaultFont();
}

void display_gauge_string(int i, int max, char *cad)
{
	SDL_Rect r;

	SDL_FillRect(screen, NULL, -1);
	if (image[IMG_LOGO]!=NULL)
	{
		r.x=(screen->w-image[IMG_LOGO]->w)>>1;
		r.y=(screen->h-image[IMG_LOGO]->h)>>1;
		r.w=image[IMG_LOGO]->w;
		r.h=image[IMG_LOGO]->h;
		SDL_BlitSurface(image[IMG_LOGO],NULL,screen,&r);
	}
	r.x=0;
	r.y=screen->h-24;
	r.w=screen->w;
	r.h=24;
#ifdef GP32
	SDL_FillRect(screen,&r,2);
#else
	SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,0,0,128));
#endif
	r.x+=2;
	r.y+=2;
	r.h-=4;
	r.w=((screen->w*(i+1))-4)/max;
	SDL_FillRect(screen,&r,0);
	SFont_XCenteredString(screen, r.y+4, cad);
	SDL_Flip(screen);
#ifndef GP32
	SDL_Delay(10);
#endif
}


/*
  FUNCION: load_images
	Carga y optimiza las imagenes.
*/
int load_images()
{
	char *cad=calloc(1024,1);
	SDL_Surface *tmp;
	int i;

	if (cad==NULL)
		return 0;

	FLI_InitDefaultFont();

	/* Carga las imagenes */
	for(i=0;i<NUM_IMAGES;i++)
	{
		memset(cad,0,1024);
		sprintf(cad,"%s%s",DATA_COMMON,image_names[i]);
		display_gauge(i,NUM_IMAGES,cad);
		tmp=IMG_Load(cad);
		if (tmp==NULL)
		{
			free(cad);
			FLI_FreeDefaultFont();
			return 0;
		}

		/* Optimiza la imagen cargada */
		image[i]=SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		
		/* Se establece el color transparente negro (RGB:0,0,0 = 0) */
		if (i>IMG_MENU)
#ifndef DREAMCAST
			SDL_SetColorKey(image[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
#else
			SDL_SetColorKey(image[i],SDL_SRCCOLORKEY, 0);
#endif

		/* Se inician los tipos de letra */

		if (i==IMG_FONT)
		{
			font.Surface=image[i];
			SFont_InitFontInfo(&font);
		}
		else if (i==IMG_FONT_OPTIONS)
		{
			font_opt.Surface=image[i];
			SFont_InitFontInfo(&font_opt);
		}
		else if (i==IMG_FONT_GAME)
		{
			fontgame.Surface=image[i];
			SFont_InitFontInfo(&fontgame);
		}
		else if (i==IMG_NUMBERS)
		{
			fontnumber.Surface=image[i];
			SFont_InitFontInfo(&fontnumber);
		}

	}	
	free(cad);
	FLI_FreeDefaultFont();
	return -1;
}


/*
  FUNCION: video_init
	Inicio del modo grafico o ventana.
*/
int video_init(void)
{
	int video_flags=SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE;
#ifdef ARGS
	if (fullscreen_arg)
		video_flags|=SDL_FULLSCREEN;
#endif
	/* Modo grafico a 320x240 con profundidad de 8bits */
	/* Flags: DoubleBuffer, Superficie Hardware y Paleta Hardware */
	screen=SDL_SetVideoMode(320, 240,
#ifdef GP32
				8,
#else
#ifdef ARGS
				bitscolor_arg,
#else
				16,
#endif
#endif
				video_flags
				);
	if (screen==NULL)
		return 0;
	if (tmp_string==NULL)
		tmp_string=calloc(128,1);
	if (tmp_string==NULL)
		return 0;
	SDL_WM_SetCaption("Vorton " VORTON_VERSION, NULL);
	SDL_ShowCursor(SDL_DISABLE);
	/* Se cargan las imagenes */
	return load_images();
}


