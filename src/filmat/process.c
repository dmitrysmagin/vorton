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
-----------
- Proceso -
-----------
*/


/* Definiciones generales */
#include "filmat.h"

static Uint32 next_time=0;	/* Proximo tiempo que se debe cumplir el frame actual */

filmat_object filmat_hero;		/* Objeto Heroe */
p_filmat_object filmat_obj;	/* Resto de Objetos */

int filmat_ship_x, filmat_ship_y;

/*
  FUNCION: real_collision
  	Ejecuta el choque de los objectos
*/
static inline void real_collision(p_filmat_object this, p_filmat_object other)
{
	this->collision(this, other);
	other->collision(other, this);
}

/*
  FUNCION: getKey
	Lee las pulsaciones y actualiza los booleanos
*/
static inline void getKey()
{
	SDL_Event event;
	SDLKey key;
	int hat=0;

	while (SDL_PollEvent(&event) > 0)
	{
		int set=-1;
		switch (event.type)
		{
			case SDL_QUIT:
				theend=1;
				break;
#ifndef GP32
			case SDL_JOYHATMOTION:
#ifdef DREAMCAST
			    	hat=15^SDL_JoystickGetHat(joy,event.jhat.hat);
#else
			    	hat=SDL_JoystickGetHat(joy,event.jhat.hat);
#endif
				if (hat & SDL_HAT_UP)
					up_pressed=-1;
				else {
					up_pressed=0;
					if (hat & SDL_HAT_DOWN)
						down_pressed=-1;
					else
						down_pressed=0;
				}
				if (hat & SDL_HAT_RIGHT)
					right_pressed=-1;
				else {
					right_pressed=0;
					if (hat & SDL_HAT_LEFT)
						left_pressed=-1;
					else
						left_pressed=0;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				set=0;
			case SDL_JOYBUTTONUP:
				switch (event.jbutton.button)
				{
					case 1:
						theend=-1; break;
					case 3:
						filmat_pauseScreen(); break;
					default:
						hit1_pressed=set;
				}
				break;
#endif
			case SDL_KEYUP:
				set=0;
			case SDL_KEYDOWN:
				key = event.key.keysym.sym;
				switch(key)
				{
					case SDLK_RETURN:
						filmat_pauseScreen(); break;
					case SDLK_SPACE:
						space_pressed=set; break;
					case SDLK_RIGHT:
						right_pressed=set; break;
					case SDLK_LEFT:
						left_pressed=set; break;
					case SDLK_UP:
						up_pressed=set; break;
					case SDLK_DOWN:
						down_pressed=set; break;
#ifdef GP32
					case SDLK_TAB:
#else
					case SDLK_ESCAPE:
#endif
						theend=-1; break;
					case SDLK_BACKSPACE:
						backspace_pressed=set; break;
					case SDLK_LCTRL:
						hit1_pressed=set; break;
					case SDLK_LALT:
						hit2_pressed=set; break;
				}
			}
	}
}

/*
  FUNCION: collision
  	Choque entre procesos
*/
static inline void collision(p_filmat_object actual)
{
	int ax0=actual->x;
	int ay0=actual->y;
	int ax1=actual->x+actual->size_x;
	int ay1=actual->y+actual->size_y;
	p_filmat_object next=filmat_first;
	while(next!=NULL)
	{
		if (actual->x < 0)
			break;
		if (next!=actual)
		{
			int bx0=next->x;
			int by0=next->y;
			int bx1=next->x+next->size_x;
			int by1=next->y+next->size_y;
			if (ax0 < bx0)
			{
				if (ax1 > bx0)
				{
					if (ay0 < by0)
					{
						if (ay1 > by0)
						{
							real_collision(actual,next);
						}
					}
					else
					{
						if (ay0 < by1)
						{
							real_collision(actual,next);
						}
					}
				}
				
			}
			else
			{
				if (ax0 < bx1)
				{
					if (ay0 < by0)
					{
						if (ay1 > by0)
						{
							real_collision(actual,next);
						}
					}
					else
					{
						if (ay0 < by1)
						{
							real_collision(actual,next);
						}	
					}
				}
			}
		}
		next=next->next;
	}
}
	

/*
  FUNCION: real_process
	Procesos de los objetos.
*/
static inline void real_process(void)
{
	p_filmat_object actual=filmat_first;
	int hy=filmat_hero.y;

	while (actual!=NULL)
	{
		register int y=actual->y;

		if ( ((hy>y-FIL_PYRAMID_VIEW)&&(hy<y+FIL_PYRAMID_VIEW)) ||
			(actual->obj_type==FIL_TYPE_DUMMY) )
		{
			actual->process(actual);

			if (actual->back_x!=actual->x || actual->back_y!=actual->y)
			{
				actual->push_x=0;
				actual->push_y=0;
				collision(actual);
			}
			actual->back_x=actual->x;
			actual->back_y=actual->y;
		}
		actual=actual->next;
	}
}

/*
  FUNCION: calcule_time
	Proceso general del juego
*/
static inline void calcule_time(void)
{
	if (!(frame&63))
	{
		game_time--;
		if (game_time<0)
			theend=1;
		else if (game_time<20)
		{
			if ((game_time==19)||(game_time==10)||(game_time==5))
			{
				music_down();
				play_sound(SND_HURRY);
				filmat_video_msg="A PRISA !";
			}
			else
			{
				music_up();
				filmat_video_msg=NULL;
			}
		}
	}
}

/*
  FUNCION: isframe
	Evalua si el frame actual debe ser pintado
	FIXME: Desactivado en GP32, hay que afinar para quitar frames

static inline int isframe(void)
{
	static int back_frame=-1;
	int ret=-1;

	long end=next_time-SDL_GetTicks();
	if ( end > -FIL_FPS2 )
	{
		if ((back_frame)&&(end>(FIL_FPS+FIL_FPS2)))
			SDL_Delay(end-FIL_FPS);
		ret=-1;
	}
	else
		ret=0;
	next_time+=FIL_FPS;
	back_frame=ret;
	return ret;
}
*/


/*
  FUNCION: process_init
	Inicia los objetos
*/
int filmat_process_init(void)
{
	int i;

	/* Inicia el Objeto heroe */
	filmat_hero_init(64,20,8);

	game_time=999;
	filmat_lasers=0;
	filmat_spin_laser=0;

	filmat_obj=(filmat_object *)calloc(sizeof(filmat_object),FIL_MAX_OBJ);
	free_all_obj();
	filmat_pyramid_init(&filmat_obj[0]);

	for(i=1;i<=lives;i++)
	{
		filmat_dummy_init(&filmat_obj[i],i<<3);
		filmat_obj[i].position=i;
	}

	for(i=lives+1;i<lives+5;i++)
	{
		filmat_obj[i].position=i;
		filmat_obj[i].x=FIL_MIN_X+(2*(FIL_TILE_SIZE*(i-lives)))-(FIL_TILE_SIZE/2);
		filmat_obj[i].y=0;
		filmat_obj[i].img=FIL_IMG_BLOCK_0;
		filmat_obj[i].obj_type=FIL_TYPE_BLOQ;
	}

	return filmat_parse_map();
}

void filmat_process_exit(void)
{
	free_all_obj();
	if (filmat_obj)
		free(filmat_obj);
	filmat_obj=NULL;
}

static inline void dummy_move()
{
	int i;
	extern int filmat_dummy_color;
	extern int filmat_dummy_img[];

	filmat_dummy_color=(filmat_dummy_color+1)&0x3;

	for(i=1;i<=lives;i++)
		filmat_obj[i].img=filmat_dummy_img[filmat_dummy_color];
	filmat_hero.img=filmat_dummy_img[filmat_dummy_color];
	filmat_hero.rot=0;
}

static inline void filmat_init_level()
{
	int i;

	filmat_global_pos=128;
	for(i=0;i<=lives;i++)
	{
		filmat_obj[i].x=-2000;
		filmat_obj[i].y=-2000;
	}
	filmat_hero.x=-2000;
	filmat_hero.y=-2000;

	next_time=SDL_GetTicks()+FIL_FPS;
	filmat_ship_x=-2000;
	filmat_ship_y=-2000;
	for(i=0;i<60;i++)
	{
		filmat_draw_init_level_presentation(i, "PREPARADO");
		filmat_delay_frame_time();
	}
	for(i=0;i<25;i++)
	{
		filmat_draw_init_level_presentation(59,"PREPARADO");
		filmat_delay_frame_time();
	}
	filmat_ship_x=-200;
	filmat_ship_y=-40;
	play_thrust();
	for(i=0;i<100;i++)
	{
		filmat_draw_init_level();
		filmat_delay_frame_time();
		filmat_ship_x+=2;
		filmat_ship_y++;
	}

	filmat_obj[0].x=FIL_MIN_X+((FIL_MAX_X-FIL_MIN_X)/2);
	filmat_obj[0].y=filmat_global_pos-16; //1940-16; //112;
	for(i=1;i<=lives;i++)
		filmat_obj[i].x=filmat_obj[0].x;
	filmat_hero.x=filmat_obj[0].x;

	for(i=0;i<128;i++)
	{
		int n;
		p_filmat_object actual, next;
		filmat_draw_init_level();
		filmat_delay_frame_time();
		if (!(i&1))
			filmat_obj[0].y++;
		filmat_obj[0].img=FIL_IMG_PYRAMID0+((i%45)/15);
		if (!(i%10))
			dummy_move();
		actual=&filmat_obj[0];
		next=actual->next;
		n=FIL_TILE_SIZE;
		while(1)
		{
			if (i>n)
				next->y=actual->y-FIL_TILE_SIZE;
			else
				break;
			actual=next;
			next=actual->next;
			n+=(FIL_TILE_SIZE*2);
			if (actual->obj_type==FIL_TYPE_HERO)
			{
				filmat_ship_y-=3;
				filmat_ship_x+=2;
				break;
			}
		}
	}
	stop_thrust();
	filmat_ship_x=-2000;
	filmat_ship_y=-2000;
	play_sound(SND_ACTIVE);
	filmat_hero.rot=6;
	filmat_hero.vel=2;
}

static inline void filmat_end_level(void)
{
	int i,j,step=1+((FIL_MAX_Y+8)-filmat_hero.y)/20;

	for(i=0;i<FIL_MAX_OBJ;i++)
	{
		if (filmat_obj[i].obj_type>FIL_TYPE_DUMMY)
			free_obj(i);
	}

	filmat_ship_x=-300;
	filmat_ship_y=10;
	play_thrust();
	for(i=0;i<100;i++)
	{
		filmat_draw_init_level();
		filmat_delay_frame_time();
		filmat_ship_x+=2;
		filmat_ship_y++;
		j=1;
		if (i>28)
		{
			while(filmat_obj[j].obj_type==FIL_TYPE_DUMMY)
			{
				filmat_obj[j].y--;
				j++;
			}
			if (filmat_hero.x<64)
				filmat_hero.x++;
			else
				if (filmat_hero.x>64)
					filmat_hero.x--;
			filmat_hero.y--;
		}
		else
		{
			if (filmat_hero.x<60 || filmat_hero.x>68)
				if (filmat_hero.y<FIL_MAX_Y+8)
					filmat_hero.y+=step;
		}
		if (!(i%10))
			dummy_move();
	}
	for(i=0;i<100;i++)
	{
		if (i==70)
			stop_thrust();
		filmat_draw_init_level();
		filmat_delay_frame_time();
		filmat_ship_y-=3;
		filmat_ship_x+=2;
		if (!(i%10))
			dummy_move();
	}
	music_stop_fade();
	play_sound(SND_BOOM);
	filmat_draw_end_level();
	for(i=59;i>=0;i--)
	{
		filmat_draw_end_level_presentation(i, "IMPRESIONANTE");
		filmat_delay_frame_time();
		if (i==40)
			play_sound(SND_AWESOME);
	}
	SDL_Delay(1500);
	music_stop();
}



void filmat_init_frame_time(void)
{
	next_time=SDL_GetTicks()+FIL_FPS;
}

void filmat_delay_frame_time(void)
{
	Uint32 t=SDL_GetTicks();

	if (t<next_time)
	{
		SDL_Delay(next_time-t);
		next_time+=FIL_FPS;
	}
	else
		next_time=t+FIL_FPS;
}

/*
  FUNCION: process
	Proceso general del juego
*/
void filmat_process()
{
	int i;

	play_sound(SND_READY);
	music_play_fade(MUSIC_LEVEL);
	filmat_init_level();
	filmat_init_frame_time();
	theend=0;
	completed=0;

	while(!theend)
	{
		frame++;
		
		getKey();

		real_process();
		calcule_time();

		filmat_draw();
		filmat_delay_frame_time();
	}
	if(!completed)
	{
		music_stop_fade();
		for(i=59;i>=0;i--)
		{
			filmat_draw_init_level_presentation(i, "FIN DEL JUEGO");
			filmat_delay_frame_time();
			if (i==40)
				play_sound(SND_GAMEOVER);
		}
		SDL_Delay(1500);
		music_stop();
	}
	else
		filmat_end_level();
}

