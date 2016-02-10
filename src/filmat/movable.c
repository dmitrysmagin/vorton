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

void filmat_movable_collision(p_filmat_object this, p_filmat_object other)
{
	other->push_x+=this->x - this->back_x;
	other->push_y+=this->y - this->back_y;
	this->x=this->back_x;
	this->y=this->back_y;
}

void filmat_movable_process(p_filmat_object this)
{
	this->x+=this->push_x;
	this->y+=this->push_y;
	limXY(this);
}

void filmat_movable_init(p_filmat_object this)
{
	switch(this->obj_type)
	{
		case FIL_TYPE_DUSTBIN:
			this->img=FIL_IMG_DUSTBIN;
			break;
	}
	this->process=(void *)&filmat_movable_process;
	this->collision=(void *)&filmat_movable_collision;
}
