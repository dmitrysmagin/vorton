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
----------------
- Objeto Heroe -
----------------
*/


/* Definiciones generales */
#include "filmat.h"

static int filmat_hero_moving=0;

int filmat_hero_mov[48]= {
		FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0,
		FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0,
		FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0,
		FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0, FIL_IMG_HERO_0_0,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2,
		FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2,
		FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2,
		FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2, FIL_IMG_HERO_0_2,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1,
		FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1, FIL_IMG_HERO_0_1
};

int filmat_hero_accel[10]= { 0,1,1,2,2,2,3,3,3,3 };

int filmat_hero_accel_x[10][8][2]=
{
	{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} },

	{ {-1,-1}, {-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}, { 1, 0}, { 0, 0}, {-1, 0} },
	{ {-1,-1}, {-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}, { 1, 0}, { 0, 0}, {-1, 0} },

	{ {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1}, { 0, 0}, {-1,-1} },
	{ {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1}, { 0, 0}, {-1,-1} },
	{ {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1}, { 0, 0}, {-1,-1} },

	{ {-3,-3}, {-2,-1}, { 0, 0}, { 2, 1}, { 3, 3}, { 2, 1}, { 0, 0}, {-2,-1} },
	{ {-3,-3}, {-2,-1}, { 0, 0}, { 2, 1}, { 3, 3}, { 2, 1}, { 0, 0}, {-2,-1} },
	{ {-3,-3}, {-2,-1}, { 0, 0}, { 2, 1}, { 3, 3}, { 2, 1}, { 0, 0}, {-2,-1} },
	{ {-3,-3}, {-2,-1}, { 0, 0}, { 2, 1}, { 3, 3}, { 2, 1}, { 0, 0}, {-2,-1} },
};

int filmat_hero_accel_y[10][8][2]=
{
	{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} },

	{ { 0, 0}, {-1, 0}, {-1,-1}, {-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}, { 1, 0} },
	{ { 0, 0}, {-1, 0}, {-1,-1}, {-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}, { 1, 0} },

	{ { 0, 0}, {-1,-1}, {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1} },
	{ { 0, 0}, {-1,-1}, {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1} },
	{ { 0, 0}, {-1,-1}, {-2,-2}, {-1,-1}, { 0, 0}, { 1, 1}, { 2, 2}, { 1, 1} },

	{ { 0, 0}, {-1,-2}, {-3,-3}, {-1,-2}, { 0, 0}, { 1, 2}, { 3, 3}, { 1, 2} },
	{ { 0, 0}, {-1,-2}, {-3,-3}, {-1,-2}, { 0, 0}, { 1, 2}, { 3, 3}, { 1, 2} },
	{ { 0, 0}, {-1,-2}, {-3,-3}, {-1,-2}, { 0, 0}, { 1, 2}, { 3, 3}, { 1, 2} },
	{ { 0, 0}, {-1,-2}, {-3,-3}, {-1,-2}, { 0, 0}, { 1, 2}, { 3, 3}, { 1, 2} },
};

int filmat_hero_hiting=0;


void filmat_hero_init(int x, int y, int state)
{
	filmat_hero.x=x;
	filmat_hero.y=y;
	filmat_hero.back_x=x;
	filmat_hero.back_y=y;
	filmat_hero.push_x=0;
	filmat_hero.push_y=0;
	filmat_hero.state=state;
	filmat_hero.rot=6;
	filmat_hero.vel=0;
	filmat_hero.obj_type=FIL_TYPE_HERO;
	filmat_hero.img=FIL_IMG_HERO_0_1;
	filmat_hero.process=(void *)&filmat_hero_process;
	filmat_hero.collision=(void *)&filmat_hero_collision;
	filmat_hero.size_x=FIL_TILE_SIZE;
	filmat_hero.size_y=FIL_TILE_SIZE;
}

/*
  FUNCION: hero_process
	Proceso del heroe.
	Cada 4 frames obtiene las pulsaciones del PAD y actua en consecuencia.
	Actualiza las coordenadas cartesianas del Heroe respecto a su velocidad actual.
*/

void filmat_hero_process(p_filmat_object this)
{
	if (!filmat_hero_hiting)
	{
		if (!(frame%4))
		{
			if (left_pressed)
				filmat_hero.rot=((filmat_hero.rot+1)%8);
			else
			if (right_pressed)
				filmat_hero.rot=(((unsigned int)(filmat_hero.rot-1))%8);
			if (up_pressed)
			{
				if (filmat_hero.vel<FIL_MAX_VEL)
					filmat_hero.vel++;
				filmat_hero.push_x=0;
				filmat_hero.push_y=0;
			}
			else
			if (down_pressed)
				if (filmat_hero.vel>0)
					filmat_hero.vel--;
	
			if ((hit1_pressed)||(hit2_pressed))
				filmat_laser_init();
			else
				filmat_spin_laser--;
		}
	
		filmat_hero.back_x=filmat_hero.x;
		filmat_hero.back_y=filmat_hero.y;
		filmat_hero.x+=filmat_hero_accel_x[filmat_hero.vel][filmat_hero.rot][filmat_hero.move_type] + filmat_hero.push_x;
		filmat_hero.y+=filmat_hero_accel_y[filmat_hero.vel][filmat_hero.rot][filmat_hero.move_type] + filmat_hero.push_y;
		filmat_hero.move_type^=1;
		
		if (filmat_hero.x>FIL_MAX_X)
		{
			filmat_hero.x=FIL_MAX_X;
			filmat_hero.vel=0;
		}
		else
		if (filmat_hero.x<FIL_MIN_X)
		{
			filmat_hero.x=FIL_MIN_X;
			filmat_hero.vel=0;
		}
		if (filmat_hero.y>FIL_MAX_Y)
		{
			filmat_hero.y=FIL_MAX_Y;
			filmat_hero.vel=0;
		}
		else
		if (filmat_hero.y<FIL_MIN_Y)
		{
			filmat_hero.y=FIL_MIN_Y;
			filmat_hero.vel=0;
		}
	
		if (filmat_hero.y-filmat_global_pos>FIL_SCROLL_MAX)
			filmat_global_pos+=filmat_hero_accel[filmat_hero.vel] + filmat_hero.push_y;
		else
		if (filmat_hero.y-filmat_global_pos<FIL_SCROLL_MIN)
			filmat_global_pos-=filmat_hero_accel[filmat_hero.vel] - filmat_hero.push_y;
	
		if (filmat_hero.vel)
		{
			filmat_hero_moving+=filmat_hero.vel;
			filmat_hero_moving%=48;
			filmat_hero.img=filmat_hero_mov[filmat_hero_moving];
		}
		else
		{
			filmat_hero_moving=0;
			filmat_hero.img=FIL_IMG_HERO_0_1;
		}
	}
	else
	{
		filmat_hero_hiting++;
		filmat_hero.vel=0;
		if (filmat_hero_hiting>9)
		{
			if (filmat_hero.state<1)
			{
				int i,o=0;
				filmat_hero.rot=0;
				switch(filmat_hero_hiting)
				{
					case 10:
						play_sound(SND_EXPL);
					case 11:
					case 12:
					case 13:
					case 14:
						filmat_hero.img=FIL_IMG_HERO_HIT_0;
						break;
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
						filmat_hero.img=FIL_IMG_HERO_HIT_1;
						break;
					case 20:
					case 21:
					case 22:
					case 23:
					case 24:
						filmat_hero.img=FIL_IMG_HERO_HIT_2;
						SDL_Delay(10);
						filmat_init_frame_time();
						break;
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
						filmat_hero.img=FIL_IMG_HERO_HIT_3;
						break;
					case 30:
					case 31:
					case 32:
					case 33:
					case 34:
						filmat_hero.img=FIL_IMG_EXPL2;
						break;
					case 35:
					case 36:
					case 37:
					case 38:
					case 39:
						filmat_hero.img=FIL_IMG_EXPL1;
						break;
					case 40:
					case 41:
					case 42:
					case 43:
					case 44:
						filmat_hero.img=FIL_IMG_EXPL4;
						break;
					case 45:
					case 46:
					case 47:
					case 48:
					case 49:
						filmat_hero.img=FIL_IMG_EXPL0;
						break;
					default:
						for(i=0;i<FIL_MAX_OBJ;i++)
							if (filmat_obj[i].obj_type==FIL_TYPE_DUMMY)
								o=i;
							else
							if (filmat_obj[i].obj_type>FIL_TYPE_DUMMY)
								break;
						if (o)
						{
							filmat_hero_init(filmat_obj[o].x,filmat_obj[o].y,filmat_obj[o].state);
							free_obj(o);
							filmat_global_pos=filmat_hero.y;
							play_sound(SND_ACTIVE);
						}
						else
						{
							theend=-1;
						}
						filmat_hero_hiting=0;
				}
			}
			else
				filmat_hero_hiting=0;
		}
		else
			filmat_hero.rot=(filmat_hero.rot+1)%8;
	}
}

static inline void hero_stop(p_filmat_object this, p_filmat_object other)
{
	other->push_x+=this->x - this->back_x;
	other->push_y+=this->y - this->back_y;
	filmat_hero.x=filmat_hero.back_x;
	filmat_hero.y=filmat_hero.back_y;
	filmat_hero.vel=1;
}

static inline void hero_hit(p_filmat_object this, p_filmat_object other)
{
	if (!filmat_hero_hiting)
	{
		play_sound(SND_BLOW);
		filmat_hero_hiting=1;
		filmat_hero.state--;
	}
}

void filmat_hero_collision(p_filmat_object this, p_filmat_object other)
{
	switch (other->obj_type)
	{
		case FIL_TYPE_MINE:
		case FIL_TYPE_MOUTH:
		case FIL_TYPE_DISC:
		case FIL_TYPE_ELECTRIC:
		case FIL_TYPE_CYCLOP:
		case FIL_TYPE_ROBOT:
		case FIL_TYPE_FIRE:
			hero_hit(this, other);
		case FIL_TYPE_BLOQ:
		case FIL_TYPE_DUSTBIN:
		case FIL_TYPE_BARREL:
		case FIL_TYPE_DUMMY:
		case FIL_TYPE_BRICK:
		case FIL_TYPE_DEFL1:
		case FIL_TYPE_DEFL2:
		case FIL_TYPE_PYRAMID:
		case FIL_TYPE_TILE:
			hero_stop(this, other);
			break;
	}
}
