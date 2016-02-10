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
---------------------
- Objeto Robot Nulo -
---------------------
*/


int filmat_dummy_img[4];
int filmat_dummy_color=0;

/* Definiciones generales */
#include "filmat.h"

#define VEL_COLOR 0x7

void filmat_dummy_collision(p_filmat_object this, p_filmat_object other)
{
	if (other->position<this->position)
		this->y=this->back_y;
	if (!this->iter)
		if (other->obj_type==FIL_TYPE_CYCLOP ||
		    other->obj_type==FIL_TYPE_MOUTH ||
		    other->obj_type==FIL_TYPE_MINE ||
		    other->obj_type==FIL_TYPE_DISC ||
		    other->obj_type==FIL_TYPE_ROBOT )
		{
			play_sound(SND_DUMMY);
		 	this->iter=1;
		}
}

void filmat_dummy_process(p_filmat_object this)
{
	static int first=0;
	int y;

	if (this->iter)
	{
		this->iter++;
		if (this->iter<50)
			this->img=filmat_dummy_img[this->iter&0x3];
		else
		{
			if (this->state<1)
			{
				if (this->iter==51)
					play_sound(SND_EXPL);
				else if (this->iter==55)
					this->img=FIL_IMG_EXPL2;
				else if (this->iter==60)
					this->img=FIL_IMG_EXPL1;
				else if (this->iter==65)
					this->img=FIL_IMG_EXPL4;
				else if (this->iter==70)
					this->img=FIL_IMG_EXPL0;
				else if (this->iter>75)
				{
					for(y=this->position+1;filmat_obj[y].obj_type==FIL_TYPE_DUMMY;y++)
					{
						filmat_obj[y-1].iter=0;
						filmat_obj[y-1].y=filmat_obj[y].y;
						filmat_obj[y-1].state=filmat_obj[y].state;
					}
					free_obj(y-1);
				}
			}
			else
			{
				if (!filmat_hero_hiting)
				{
					this->state--;
					this->iter=0;
				}
			}
		}
	}
	else
	{
		if (!(frame & VEL_COLOR))
		{
			if (first)
			{
				filmat_dummy_color=(filmat_dummy_color+1)&0x3;
				first=0;
			}
			this->img=filmat_dummy_img[filmat_dummy_color];
		}
		else
			first=1;
	}
	y=filmat_obj[this->position-1].y-FIL_TILE_SIZE;
	if (y<FIL_MAX_Y)
		this->y=y;
}

void filmat_dummy_init(p_filmat_object this, int y)
{
	this->x=64;
	this->y=filmat_obj[0].y-y;
	this->back_x=64;
	this->back_y=filmat_obj[0].y-y;
	this->obj_type=FIL_TYPE_DUMMY;
	this->img=FIL_IMG_DUMMY_0;
	this->size_x=FIL_TILE_SIZE;
	this->size_y=FIL_TILE_SIZE;
	this->process=(void *)&filmat_dummy_process;
	this->collision=(void *)&filmat_dummy_collision;
	this->state=8;
	this->iter=0;

	filmat_dummy_img[0]=FIL_IMG_DUMMY_0;
	filmat_dummy_img[1]=FIL_IMG_DUMMY_1;
	filmat_dummy_img[2]=FIL_IMG_DUMMY_2;
	filmat_dummy_img[3]=FIL_IMG_DUMMY_1;
}

