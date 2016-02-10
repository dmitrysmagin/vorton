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
- Objeto Bloque -
-----------------
*/


/* Definiciones generales */
#include "filmat.h"

void filmat_bloq_process(p_filmat_object this)
{
}

void filmat_bloq_collision(p_filmat_object this, p_filmat_object other)
{
}

void filmat_bloq_init(p_filmat_object this)
{
	switch(this->obj_type)
	{
		case FIL_TYPE_BLOQ:
			this->img=FIL_IMG_BLOCK_0;
			break;
		case FIL_TYPE_TILE:
			this->img=FIL_IMG_TILE;
			break;
	}
	this->process=(void *)&filmat_bloq_process;
	this->collision=(void *)&filmat_bloq_collision;
}
