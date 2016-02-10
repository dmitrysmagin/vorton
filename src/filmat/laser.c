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
 
#include "filmat.h"

int filmat_lasers=0;
int filmat_spin_laser=0;

#define TILE2 (TILE_SIZE / 2)
// #define TILE2 ((4*TILE_SIZE)/3)

//static int init_x[8]= { -TILE2, -TILE2, 0, TILE2, TILE2, TILE2, 0, -TILE2 };
//static int init_y[8]= { 0, -TILE2, -TILE2, -TILE2, 0, TILE2, TILE2, TILE2 };

static int imag[FIL_HIT_TIME_LIFE+4]=
{
	FIL_IMG_LASER_0, FIL_IMG_LASER_0, FIL_IMG_LASER_0, FIL_IMG_LASER_0,
	FIL_IMG_LASER_0, FIL_IMG_LASER_0, FIL_IMG_LASER_1, FIL_IMG_LASER_1,
	FIL_IMG_LASER_1, FIL_IMG_LASER_1, FIL_IMG_LASER_1, FIL_IMG_LASER_2,
	FIL_IMG_LASER_2, FIL_IMG_LASER_2, FIL_IMG_LASER_3, FIL_IMG_LASER_3,
	FIL_IMG_LASER_3, FIL_IMG_LASER_3, FIL_IMG_LASER_2, FIL_IMG_LASER_2,
	FIL_IMG_LASER_1, FIL_IMG_LASER_1, FIL_IMG_LASER_0, FIL_IMG_LASER_0,
	FIL_IMG_LASER_0, FIL_IMG_LASER_0, FIL_IMG_LASER_0, FIL_IMG_LASER_0
};

static inline void free_laser(int position)
{
	if (filmat_obj[position].sign>=0)
	{
		free_obj(position);
		filmat_lasers--;
		filmat_obj[position].sign=-1;
	}
}

void filmat_laser_process(p_filmat_object this)
{
//    if (this->x > 0)
//    {
 	switch(this->direction)
	 {
			
			case 7:
				this->y+=this->vel;
				this->x-=this->vel;
				break;
			case 0:
				this->x-=this->vel;
				this->x-=this->vel;
			
				break;
			case 1:
				this->x-=this->vel;
				this->y-=this->vel;
				
				break;
			case 2:
				this->y-=this->vel;
				this->y-=this->vel;
				
				break;
			case 3:
				this->y-=this->vel;
				this->x+=this->vel;
			
				break;
			case 4:
				this->x+=this->vel;
				this->x+=this->vel;
				
				break;
			case 5:
				this->x+=this->vel;
				this->y+=this->vel;
				
				break;
			default:
				this->y+=this->vel;
				this->y+=this->vel;
				
	}

	this->sign--;

	register int draw_y=(this->x-this->y+filmat_global_pos);

	if ( (this->sign<0) || (this->x<=0) || (draw_y<=FIL_MIN_VISUAL) ||(draw_y>=FIL_MAX_VISUAL) )
	{
		this->sign=0;
		free_laser(this->position);
	}
	else
	{
		this->img=imag[this->sign];
	}
//   }
}

/* Incluido código para gestionar la explosión de los robots */
/* Fecha última modificación : 18/10/03 */

void filmat_laser_collision(p_filmat_object this, p_filmat_object other)
{

//if (this->position!=other->position) /*#*/
//{
	switch(other->obj_type)
	{
//Objetos en los que el laser no hace nada
		case FIL_TYPE_DUMMY:
		case FIL_TYPE_DUSTBIN:
		case FIL_TYPE_BRICK:
		case FIL_TYPE_BLOQ:
		case FIL_TYPE_MINE:
			other->push_x+=this->x - this->back_x;
			other->push_y+=this->y - this->back_y;
			free_laser(this->position);
			break;
		
//Objetos en los que el laser mata
		case FIL_TYPE_MOUTH:
		case FIL_TYPE_ROBOT:
		case FIL_TYPE_CYCLOP:
		case FIL_TYPE_DISC:
			other->process=(void *)&filmat_expl_process;
			play_sound(SND_HIT);
			other->rot=0;
			other->obj_type=FIL_TYPE_EXPL;
			other->img=FIL_IMG_EXPL0;  /* Le cambio la imagen */
			free_laser(this->position); /* Elimina disparo */
			break;

//Objeto especial espejo
		case FIL_TYPE_DEFL1:
			this->direction=(this->direction+1)&0x7;
			break;

//Objeto especial espejo con otro angulo
		case FIL_TYPE_DEFL2:
			if (this->direction<1)
				this->direction=7;
			else
				this->direction--;
			break;
	}
//}	
}

void filmat_laser_init(void)
{
	if (filmat_lasers>=filmat_spin_laser)
	{
		int index=search_free_obj();
	
		if (index>=0)
		{
			play_sound(SND_LASER);
			filmat_obj[index].x=filmat_hero.x+(filmat_hero_accel_x[filmat_hero.vel][filmat_hero.rot][0]);
			filmat_obj[index].y=filmat_hero.y+(filmat_hero_accel_y[filmat_hero.vel][filmat_hero.rot][0]);
			filmat_obj[index].direction=filmat_hero.rot;
			filmat_obj[index].vel=2;
			filmat_obj[index].state=1;
			filmat_obj[index].img=FIL_IMG_LASER_0;
			filmat_obj[index].back_x=filmat_obj[index].x;
			filmat_obj[index].back_y=filmat_obj[index].y;
			filmat_obj[index].process=(void *)&filmat_laser_process;
			filmat_obj[index].collision=(void *)&filmat_laser_collision;
			filmat_obj[index].size_x=FIL_TILE_SIZE;
			filmat_obj[index].size_y=FIL_TILE_SIZE;
			filmat_obj[index].position=index;
			filmat_obj[index].sign=FIL_HIT_TIME_LIFE;
			filmat_obj[index].obj_type=FIL_TYPE_LASER;
			filmat_lasers++;
			filmat_spin_laser=filmat_lasers<<1;
		}
	}
	else
		filmat_spin_laser--;
}
