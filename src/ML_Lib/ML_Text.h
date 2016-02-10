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

#if !defined(TEXT_H)
#define TEXT_H

#include "ML_Node.h"

struct MLText{

		p_node p_this;

		void (*AddRef) (struct MLText *this);
		void (*Release) (struct MLText *this);
		int (*QueryInterface) (struct MLText *this, int IID, void **p_IObj);

		int (*GetType) (struct MLText *this);

		const char * (*GetName) (struct MLText *this);
		int (*SetName) (struct MLText *this, char * name);

		int (*GetDataSize) (struct MLText *this);
		int (*GetData) (struct MLText *this, int *buffer_size, void **out_buffer);
		int (*SetData) (struct MLText *this, int buffer_size, void *in_buffer);
		int (*DeleteData) (struct MLText *this);

		int (*GetML) (struct MLText *this, unsigned int *buffer_size, char **out_buffer);
} ;


typedef struct MLText *IMLText;


/* Para iniciar los nodos de tipo Text*/
void text_init (p_node nodo);

/* Para iniciar las interfaces IMLText*/
void mltext_init (IMLText interfaz, p_node objeto);

#endif /* !defined( TEXT_H )*/



