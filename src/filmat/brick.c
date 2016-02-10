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
-----------------
- Objeto Brick  -
-----------------
*/


/* Definiciones generales */
#include "filmat.h"

void filmat_brick_process(p_filmat_object this)
{
}

void filmat_brick_collision(p_filmat_object this, p_filmat_object other)
{
	if ((other->obj_type==FIL_TYPE_LASER)||(other->obj_type==FIL_TYPE_NULL))
	{
		this->move_type--;
		if (!this->move_type)
		{
			free_obj(this->position);
		}
		else
			this->img++;
	}
}

void filmat_brick_init(p_filmat_object this)
{
	this->img=FIL_IMG_BRICK_0;
	this->process=(void *)&filmat_brick_process;
	this->collision=(void *)&filmat_brick_collision;
	this->move_type=4;
}
