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
- Objeto Fire  -
-----------------
*/


/* Definiciones generales */
#include "filmat.h"

void filmat_fire_process(p_filmat_object this)
{
	this->move_type=(this->move_type+1)&0x3;
	if (!this->move_type)
		this->rot=(this->rot+1)&0x3;
}

void filmat_fire_collision(p_filmat_object this, p_filmat_object other)
{
}

void filmat_fire_init(p_filmat_object this)
{
	this->img=FIL_IMG_FIRE_0;
	this->process=(void *)&filmat_fire_process;
	this->collision=(void *)&filmat_fire_collision;
	this->move_type=0;
}
