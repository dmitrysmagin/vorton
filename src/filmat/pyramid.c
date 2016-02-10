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
-------------------
- Objeto Piramide -
-------------------
*/


/* Definiciones generales */
#include "filmat.h"

void filmat_pyramid_collision(p_filmat_object this, p_filmat_object other)
{
	other->push_y+=this->y - this->back_y;
	this->y=this->back_y;
}

void filmat_pyramid_process(p_filmat_object this)
{
	register int f=(frame%45) / 15;
	this->img=FIL_IMG_PYRAMID0+f; //(f/15);
	if (filmat_obj[1].obj_type==FIL_TYPE_DUMMY)
	{
		if (!(frame&0x1))
		{
			register int y=this->y;
			register int hy=filmat_hero.y;
			if ((hy>y-FIL_PYRAMID_VIEW)&&(hy<y+FIL_PYRAMID_VIEW))
				this->y++;
		}
	}
	else
		this->y+=this->push_y;
	if (this->y>FIL_MAX_Y)
	{
		filmat_hero.process=(void *)&filmat_bloq_process;
		if (this->y>FIL_MAX_Y+32)
		{
			this->x=-2000;
			filmat_global_pos=4096;
			completed=1;
			theend=1;
		}
		else
			this->y+=2;
		if (this->y<4096)
			filmat_global_pos=this->y;
	}

}

void filmat_pyramid_init(p_filmat_object this)
{
	this->x=64;
	this->y=64;
	this->back_x=64;
	this->back_y=64;
	this->push_x=0;
	this->push_y=0;
	this->position=0;
	this->obj_type=FIL_TYPE_PYRAMID;
	this->img=FIL_IMG_PYRAMID0;
	this->size_x=FIL_TILE_SIZE;
	this->size_y=FIL_TILE_SIZE;
	this->process=(void *)&filmat_pyramid_process;
	this->collision=(void *)&filmat_pyramid_collision;
}

