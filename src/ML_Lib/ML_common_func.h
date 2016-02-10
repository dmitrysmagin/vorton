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

#if !defined(COMMON_FUNC_H)
#define COMMON_FUNC_H

#include "ML_Node.h"
#include "ML_Element.h"
#include "ML_Process.h"
#include "ML_Attribute.h"
#include "ML_Text.h"
#include "ML_Comment.h"

int common_GetSingleChild (p_node this, char * name, IMLNode *out, int index);

int common_AppendChild (p_node this, IMLNode child, IMLNode *ret);

int common_DeleteChildByInterfaz (p_node this, IMLNode child);

int common_DeleteChildByIndex (p_node this, int index);

int common_GetData (p_node this, int *buffer_size, void **out_buffer);

int common_SetData (p_node this, int buffer_size, void *in_buffer);


#endif /*!define(COMMON_FUNC_H)*/

