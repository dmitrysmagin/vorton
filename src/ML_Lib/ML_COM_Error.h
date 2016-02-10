/*---------------------------------------------------------------------------

	                  GNU Lesser General Public License

------------------------------------------------------------------------------
    Markup Languaje Library ML_Lib version 0.1
    Copyright (C) 2004  Diego Blazquez Garcia, alias (Mortimor)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

------------------------------------------------------------------------------*/

#if !defined(ML_ERROR_H)
#define ML_ERROR_H

#define MAX_ML_FUNC_NAME 30

static unsigned int ml_func_id=0; /*Aquellas funciones que pongan su nombre lo indican con un 1 en el primer bit*/
static char ml_func_name [MAX_ML_FUNC_NAME];
static int ml_err_cod=0;
static char ml_err_desc[200];

#endif /*if !defined(ML_ERROR_H)*/