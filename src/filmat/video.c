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
#include "filmat.h"

/* Nombre de los ficheros imagenes */
char *filmat_image_names[FIL_NUM_IMAGES]={
	"hero00.png",
	"hero10.png",
	"hero20.png",
	"hero30.png",
	"hero40.png",
	"hero50.png",
	"hero60.png",
	"hero70.png",
	"hero01.png",
	"hero11.png",
	"hero21.png",
	"hero31.png",
	"hero41.png",
	"hero51.png",
	"hero61.png",
	"hero71.png",
	"hero02.png",
	"hero12.png",
	"hero22.png",
	"hero32.png",
	"hero42.png",
	"hero52.png",
	"hero62.png",
	"hero72.png",
	"hero0e.png",
	"hero1e.png",
	"hero2e.png",
	"hero3e.png",
	"dummy0.png",
	"dummy1.png",
	"dummy2.png",
	"laser0.png",
	"laser1.png",
	"laser2.png",
	"laser3.png",
	"expl0.png",
	"expl1.png",
	"expl2.png",
	"expl3.png",
	"expl4.png",
	"energy0.png",
	"energy1.png",
	"energy2.png",
	"energy3.png",
	"energy4.png",
	"energy5.png",
	"energy6.png",
	"energy7.png",
	"energy8.png",
	"pyramid0.png",
	"pyramid1.png",
	"pyramid2.png",
	"ship.png",
	"block.png",
	"robot0.png",
	"robot1.png",
	"robot2.png",
	"robot3.png",
	"robot4.png",
	"robot5.png",
	"robot6.png",
	"robot7.png",
	"cyclop0.png",
	"cyclop1.png",
	"cyclop2.png",
	"cyclop3.png",
	"cyclop4.png",
	"cyclop5.png",
	"cyclop6.png",
	"cyclop7.png",
	"mina0.png",
	"mina1.png",
	"mina2.png",
	"mina3.png",
	"mina4.png",
	"mina5.png",
	"mina6.png",
	"mina7.png",
	"tile.png",
	"brick0.png",
	"brick1.png",
	"brick2.png",
	"brick3.png",
	"fire0.png",
	"fire1.png",
	"fire2.png",
	"fire3.png",
	"boca0.png",
	"boca1.png",
	"boca2.png",
	"boca3.png",
	"boca4.png",
	"boca5.png",
	"boca6.png",
	"boca7.png",
	"disco0.png",
	"disco1.png",
	"disco2.png",
	"disco3.png",
	"disco4.png",
	"disco5.png",
	"disco6.png",
	"disco7.png",
	"dustbin.png",
	"flame.png",
	"defl1.png",
	"defl2.png",
	"floor0.png",
	"floor1.png",
	"floor2.png",
	"floor3.png",
	"floor4.png",
	"floor5.png",
	"floor6.png",
	"floor7.png",
};

#define FL0 FIL_IMG_FLOOR_0
#define FL1 FIL_IMG_FLOOR_1
#define FL2 FIL_IMG_FLOOR_2
#define FL3 FIL_IMG_FLOOR_3
#define FL4 FIL_IMG_FLOOR_4
#define FL5 FIL_IMG_FLOOR_5
#define FL6 FIL_IMG_FLOOR_6
#define FL7 FIL_IMG_FLOOR_7

/* Mapa del fondo: La ultima linea debe ser igual que la primera para repeticion */
static Uint8 floors[]={
	FL7,FL6,FL5,FL4,FL1,FL0,FL0,FL1,FL0,
	FL2,FL0,FL0,FL1,FL3,FL1,FL1,FL0,FL1,
	FL3,FL1,FL1,FL0,FL2,FL0,FL0,FL1,FL0,
	FL1,FL0,FL1,FL0,FL0,FL1,FL0,FL1,FL0,
	FL1,FL2,FL0,FL1,FL3,FL0,FL1,FL1,FL0,
	FL0,FL1,FL0,FL1,FL1,FL0,FL0,FL1,FL0,
	FL0,FL3,FL1,FL0,FL2,FL1,FL0,FL0,FL1,
	FL0,FL0,FL1,FL2,FL3,FL1,FL0,FL0,FL1,
	FL0,FL1,FL0,FL1,FL1,FL0,FL0,FL1,FL0,
	FL1,FL3,FL0,FL1,FL1,FL0,FL1,FL2,FL1,
	FL3,FL2,FL1,FL0,FL1,FL0,FL0,FL1,FL0,
	FL2,FL0,FL0,FL1,FL3,FL1,FL1,FL0,FL1,
	FL3,FL1,FL1,FL0,FL2,FL0,FL0,FL1,FL0,
	FL1,FL0,FL1,FL0,FL0,FL1,FL0,FL1,FL0,
	FL1,FL2,FL0,FL1,FL3,FL0,FL1,FL1,FL0,
	FL0,FL3,FL1,FL0,FL2,FL1,FL0,FL0,FL1,
	FL0,FL0,FL1,FL2,FL3,FL1,FL0,FL0,FL1,
	FL0,FL1,FL0,FL1,FL1,FL0,FL0,FL1,FL0,
	FL0,FL1,FL0,FL1,FL1,FL0,FL0,FL1,FL0
};

static unsigned char precal_presentation[60]=
        {
		0x01, 0x04, 0x07, 0x0A, 0x0E, 0x11, 0x14, 0x17, 0x1A, 0x1D,
		0x20, 0x23, 0x26, 0x29, 0x2C, 0x2F, 0x32, 0x35, 0x37, 0x3A,
		0x3D, 0x40, 0x42, 0x45, 0x47, 0x4A, 0x4C, 0x4F, 0x51, 0x53,
		0x56, 0x58, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0x63, 0x65, 0x67,
		0x68, 0x6A, 0x6B, 0x6C, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73,
		0x74, 0x74, 0x75, 0x75, 0x76, 0x76, 0x77, 0x77, 0x77, 0x77,
        };


/* Objeto primero en la lista */
p_filmat_object filmat_first;

char *filmat_video_msg=NULL;


/*
  FUNCION: load_images
	Carga y optimiza las imagenes.
*/
int filmat_load_images(void)
{
	char *cad=calloc(1024,1);
	SDL_Surface *tmp;
	int i;

	if (cad==NULL)
		return 0;

	FLI_InitDefaultFont();

	/* Carga las imagenes */
	for(i=0;i<FIL_NUM_IMAGES;i++)
	{
		memset(cad,0,1024);
		if (i<FIL_IMG_BLOCK_0)
			sprintf(cad,"%s%s",FIL_DATA_COMMON,filmat_image_names[i]);
		else
			sprintf(cad,"%s%i%s%s",DATA_PREFIX,level,FILE_SEPARATOR,filmat_image_names[i]);
		display_gauge(i,FIL_NUM_IMAGES,cad);
		tmp=IMG_Load(cad);
		if (tmp==NULL)
		{
			free(cad);
			FLI_FreeDefaultFont();
			return 0;
		}

		/* Optimiza la imagen cargada */
		filmat_image[i]=SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		
		/* Se establece el color transparente negro (RGB:0,0,0 = 0) */
		if (i<FIL_IMG_FLOOR_0)
#ifndef DREAMCAST
			SDL_SetColorKey(filmat_image[i],SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
#else
			SDL_SetColorKey(filmat_image[i],SDL_SRCCOLORKEY, 0);
#endif
	}	
	free(cad);
	FLI_FreeDefaultFont();
	return -1;
}

void filmat_unload(void)
{
	int i;
	for(i=0;i<FIL_NUM_IMAGES;i++)
		SDL_FreeSurface(filmat_image[i]);
	filmat_process_exit();
}


/*
  FUNCION: draw_floor
	Dibujado del fondo.
*/
static inline void filmat_draw_floor(void)
{
	SDL_Rect s,d;
	int i,tmpY;
	int filmat_floors_index;
	int filmat_floors_offset;

	/* Se calcula el offset */
	int pos=4096-filmat_global_pos;

	filmat_floors_offset=31-(pos&0x1f);
	filmat_floors_index=pos>>5; //(pos/32);

	/* Calculo de ancho, largo e Y de la superficie fuente */
	s.h=filmat_image[FIL_IMG_FLOOR_0]->h;
	s.w=320;
	s.y=0;

	/* Calculo del ancho, largo y X de la superficie destino */
	d.h=filmat_image[FIL_IMG_FLOOR_0]->h;
	d.w=filmat_image[FIL_IMG_FLOOR_0]->w;
	d.x=0;
	
	/* Se establece x e y iniciales del bucle */
	s.x=filmat_floors_offset<<1;	/* Rotar a la derecha es igual que multiplicar por dos pero mas rapido */
	d.y=filmat_floors_offset-32;

	/* Bucle: Se pinta los bloques del suelo */
	for(i=0;i<10;i++)
	{
		tmpY=d.y;
		SDL_BlitSurface(filmat_image[floors[i+filmat_floors_index]],&s,screen,&d);
		s.x+=64;
		d.y=tmpY+32;
	}
	
}


/*
  FUNCION: set_obj
	Se inserta de forma ordenada un objeto en la lista de visualizacion.
*/
static inline void filmat_set_obj(p_filmat_object obj)
{
	p_filmat_object actual=filmat_first;

	while(1)
	{
		if (obj->draw_y<actual->draw_y)
		{
			p_filmat_object back=actual->back;
			obj->back=back;
			obj->next=actual;
			actual->back=obj;
			if (back==NULL)
				filmat_first=obj;
			else
				back->next=obj;
			return;
		}
		if (actual->next!=NULL)
			actual=actual->next;
		else
		{
			actual->next=obj;
			obj->next=NULL;
			obj->back=actual;
			break;
		}
	}
}


/*
  FUNCION: sort_obj
	Se calcula las coordenadas de pantalla de cada objeto.
	Si el objeto esta en la zona visible (o cerca) se inserta en la lista.
*/
static inline void filmat_sort_obj(void)
{
	int i;

	/* Se establece el primero en la lista el objeto Heroe */
	filmat_first=&filmat_hero;
	filmat_first->next=NULL;
	filmat_first->back=NULL;
	
	/* Calculo de las coordenas de pantalla del Heroe */
	filmat_hero.draw_x=(filmat_hero.x+filmat_hero.y-filmat_global_pos)<<1;
	filmat_hero.draw_y=(filmat_hero.x-filmat_hero.y+filmat_global_pos);

	/* Bucle para todo objeto */
	for(i=0;i<FIL_MAX_OBJ;i++)
	{
		/* Calculo de las coordenadas de pantalla */
		register int x=filmat_obj[i].x;
		if (x>=0)
		{
			register int y=filmat_obj[i].y;
			register int draw_x=(x+y-filmat_global_pos)<<1;
			register int draw_y=(x-y+filmat_global_pos);

			/* Si esta dentro de la zona de visualizacion */
			if ((draw_y>FIL_MIN_VISUAL)&&(draw_y<FIL_MAX_VISUAL))
			{
				/* Se guardan las coordenadas dentro de la estructura objecto */
				filmat_obj[i].draw_x=draw_x;
				filmat_obj[i].draw_y=draw_y;
	
				/* Insercion dentro de la lista de visualizacion */
				filmat_set_obj(&filmat_obj[i]);
			}
		}
	}
}

/*
  FUNCION: draw_obj
	Dibujado de los objetos visibles
	Se recorre la lista de visualizacion ordenada para .
*/
static inline void filmat_draw_obj(void)
{
	SDL_Rect r;
	SDL_Surface *tmp;

	/* Se inicia el objecto del bucle con el primero de la lista */
	p_filmat_object actual=filmat_first;

	/* Bucle */
	while(actual!=NULL)
	{
		/* La imagen a visualizar se calcula respecto a su imagen base y su rotacion */
		tmp=filmat_image[actual->img+actual->rot];

		/* Se establecen las posiciones de visualizacion */
		r.x=actual->draw_x;
		r.y=actual->draw_y;
		r.w=tmp->w;
		r.h=tmp->h;
		/* A pintar... */
		SDL_BlitSurface(tmp,NULL,screen,&r);

		/* Proximo objeto */
		actual=actual->next;
	}
}


/*
  FUNCION: draw_energy
  	Pinta la energia del heroe
*/
static inline void filmat_draw_energy(void)
{
	SDL_Surface *s=filmat_image[FIL_IMG_ENERGY_0+filmat_hero.state];
	SDL_Rect r;
	r.x=4;
	r.y=4;
	r.h=s->h;
	r.w=s->w;
	SDL_BlitSurface(s, NULL, screen, &r);
}

/*
  FUNCION: draw_score
  	Pinta los puntos
*/
static inline void filmat_draw_score(void)
{
	sprintf(tmp_string,"PUNTOS     %i",score);
	font_put(screen,160,220,tmp_string);
}

/*
  FUNCION: draw_time
  	Pinta los puntos
*/
static inline void filmat_draw_time(void)
{
	sprintf(tmp_string,"%i",game_time);
	fontnumber_put(screen,264,2,tmp_string);
}

static inline void filmat_draw_percent(void)
{
	int i=(filmat_hero.y*100)/FIL_MAX_Y;
	sprintf(tmp_string,"%i%%",i);
	font_put(screen,275,200,tmp_string);
}

/*
  FUNCION: draw
	Proceso general de pintado.
*/
void filmat_draw(void)
{
	filmat_sort_obj();
	filmat_draw_floor();
	filmat_draw_obj();
	filmat_draw_energy();
	filmat_draw_score();
	filmat_draw_time();
	filmat_draw_percent();
	if (filmat_video_msg)
		fontgame_centered(screen,100,filmat_video_msg);
	SDL_Flip(screen);
}

static inline void filmat_draw_ship(void)
{
	SDL_Surface *s=filmat_image[FIL_IMG_SHIP];
	SDL_Rect r;
	r.x=filmat_ship_x;
	r.y=filmat_ship_y;
	r.h=s->h;
	r.w=s->w;
	SDL_BlitSurface(s, NULL, screen, &r);
}

static inline void filmat_real_draw_init_level(void)
{
	int i;
	p_filmat_object actual=&filmat_obj[0];
	filmat_first=actual;

	actual->back=NULL;

	for(i=1;i<=lives;i++)
	{
		actual->draw_x=(actual->x+actual->y-filmat_global_pos)<<1;
		actual->draw_y=(actual->x-actual->y+filmat_global_pos);
		actual->next=&filmat_obj[i];
		filmat_obj[i].back=actual;
		actual=&filmat_obj[i];
	}
	actual->draw_x=(actual->x+actual->y-filmat_global_pos)<<1;
	actual->draw_y=(actual->x-actual->y+filmat_global_pos);
	actual->next=&filmat_hero;
	filmat_hero.back=actual;
	filmat_hero.next=&filmat_obj[i];
	filmat_hero.draw_x=(filmat_hero.x+filmat_hero.y-filmat_global_pos)<<1;
	filmat_hero.draw_y=(filmat_hero.x-filmat_hero.y+filmat_global_pos);

	actual=&filmat_hero;
	for(i=lives+1;i<lives+11;i++)
	{
		filmat_obj[i].draw_x=(filmat_obj[i].x+filmat_obj[i].y-filmat_global_pos)<<1;
		filmat_obj[i].draw_y=(filmat_obj[i].x-filmat_obj[i].y+filmat_global_pos);
		filmat_obj[i].next=&filmat_obj[i+1];
		filmat_obj[i].back=actual;
		actual=&filmat_obj[i];
	}
	actual->next=NULL;
}


static inline void filmat_draw_init_level_real(void)
{
	filmat_draw_floor();
	filmat_real_draw_init_level();
	filmat_draw_obj();
	filmat_draw_ship();
}

void filmat_draw_end_level(void)
{
	int i,j;
	for(i=0;i<32;i++)
	{
		invert_screen_color();
		SDL_Flip(screen);
		filmat_delay_frame_time();
		invert_screen_color();
		SDL_Flip(screen);
		filmat_delay_frame_time();
	}
	for(i=255;i>0;i-=2)
	{
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,i,i,i));
		SDL_Flip(screen);
		filmat_delay_frame_time();
	}
	for(j=0;j<240;j+=32)
		for(i=0;i<=320;i+=64)
		{
			SDL_Rect r;
			r.x=i-((j&0x20)>>1);
			r.y=j;
			r.w=filmat_image[FIL_IMG_FLOOR_7]->w;
			r.h=filmat_image[FIL_IMG_FLOOR_7]->h;
			SDL_BlitSurface(filmat_image[FIL_IMG_FLOOR_7],NULL,screen,&r);
		}
	invert_screen_color();
	SDL_Flip(screen);

	for(i=0;i<16;i++)
		filmat_delay_frame_time();
}

void filmat_draw_end_level_presentation(int y, char *str)
{
	int h = screen->h;
	int yy = precal_presentation[y];

	if (yy<h)
	{
		int pitch = screen->pitch;
		void *src = screen->pixels + (yy * pitch) + yy;
		void *dest= src + pitch;
		int i, many=pitch-(yy<<1);

		for(i=yy+1,h-=yy;i<h;i++,dest+=pitch)
			memcpy(dest,src,many);
	}
	fontgame_centered(screen,100,str);
	SDL_Flip(screen);
}

void filmat_draw_init_level(void)
{
	filmat_draw_init_level_real();
	SDL_Flip(screen);
}

void filmat_draw_init_level_presentation(int y, char *str)
{
	filmat_draw_init_level_real();
	int h = screen->h;
	int yy = precal_presentation[y];

	if (yy<h)
	{
		int pitch = screen->pitch;
		void *src = screen->pixels + (yy * pitch) + yy;
		void *dest= src + pitch;
		int i, many=pitch-(yy<<1);

		for(i=yy+1,h-=yy;i<h;i++,dest+=pitch)
			memcpy(dest,src,many);
	}
	fontgame_centered(screen,100,str);
	SDL_Flip(screen);
}

void filmat_pauseScreen(void)
{
	int i,j;
	Uint32 col=SDL_MapRGB(screen->format,128,128,128);
	Uint8 *buf8=(Uint8 *)screen->pixels;;
	Uint16 *buf16=(Uint16 *)screen->pixels;
	Uint32 *buf32=(Uint32 *)screen->pixels;
	SDL_Event event;

	music_down();
	play_sound(SND_PAUSE);

	switch(screen->format->BitsPerPixel)
	{
		case 8:
			for(i=0;i<240;i+=2)
				for(j=0;j<320;j+=2)
				{
					buf8[j+i*320]=(Uint8)col;
					buf8[1+j+(i+1)*320]=(Uint8)col;
				}
			break;
		case 16:
			for(i=0;i<240;i+=2)
				for(j=0;j<320;j+=2)
				{
					buf16[j+i*320]=(Uint16)col;
					buf16[1+j+(i+1)*320]=(Uint16)col;
				}
			break;
		case 32:
			for(i=0;i<240;i+=2)
				for(j=0;j<320;j+=2)
				{
					buf32[j+i*320]=(Uint32)col;
					buf32[1+j+(i+1)*320]=(Uint32)col;
				}
			break;
	}
	fontgame_centered(screen,100,"PAUSA");
	SDL_Flip(screen);
	SDL_Delay(200);
	while (SDL_PollEvent(&event)>0)
	{
		if (event.type == SDL_QUIT)
		{
			theend=1;
			break;
		}
		SDL_Delay(10);
	}
	if (!theend)
	{
		while (!SDL_PollEvent(&event))
			SDL_Delay(100);
		do{
			if (event.type == SDL_QUIT)
			{
				theend=1;
				break;
			}
			else
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym!=SDLK_RETURN)
					break;
			}
			else
			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym==SDLK_RETURN)
					break;
			}
			else
			if (event.type == SDL_JOYBUTTONDOWN)
			{
				if (event.jbutton.button==3)
					break;
			}
			else
			if (event.type == SDL_JOYBUTTONUP)
			{
				if (event.jbutton.button!=3)
					break;
			}
			SDL_Delay(100);
		}while (SDL_PollEvent(&event)>=0);
	}
	music_up();
	filmat_init_frame_time();
}
