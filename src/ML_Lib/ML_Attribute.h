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

#if !defined(ATTRIBUTE_H)
#define ATTRIBUTE_H

#include "ML_Node.h"

struct MLAttribute{

		p_node p_this;

		void (*AddRef) (struct MLAttribute *this);
		void (*Release) (struct MLAttribute *this);
		int (*QueryInterface) (struct MLAttribute *this, int IID, void **p_IObj);

		int (*GetType) (struct MLAttribute *this);

		const char *(*GetName) (struct MLAttribute *this);
		int (*SetName) (struct MLAttribute *this, char * name);

		int (*GetNumChilds) (struct MLAttribute *this);
		int (*GetSingleChild) (struct MLAttribute *this, char * name, IMLNode *out, int index); 
		/*int (*GetChild) (struct MLAttribute *this, char * name, IMLNode *out); */
		int (*AppendChild) (struct MLAttribute *this, IMLNode child, IMLNode *ret);
		int (*DeleteChildByInterfaz) (struct MLAttribute *this, IMLNode child);
		int (*DeleteChildByIndex) (struct MLAttribute *this, int index);

		int (*GetDataSize) (struct MLAttribute *this);
		int (*GetData) (struct MLAttribute *this, int *buffer_size, void **out_buffer);
		int (*SetData) (struct MLAttribute *this, int buffer_size, void *in_buffer);
		int (*DeleteData) (struct MLAttribute *this);

		int (*GetML) (struct MLAttribute *this, unsigned int *buffer_size, char **out_buffer);
};

typedef struct MLAttribute * IMLAttribute;


/* Para iniciar los nodos de tipo Attribute*/
void attribute_init (p_node nodo);

/* Para iniciar las interfaces IMLAttribute*/
void mlattribute_init (IMLAttribute interfaz, p_node objeto);

#endif /*!define(ATTRIBUTE_H)*/

