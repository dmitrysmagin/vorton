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

#if !defined(PROCESS_H)
#define PROCESS_H

#include "ML_Node.h"
#include "ML_Attribute.h"


struct MLProcessInstruction{

		p_node p_this;

		void (*AddRef) (struct MLProcessInstruction *this);
		void (*Release) (struct MLProcessInstruction *this);
		int (*QueryInterface) (struct MLProcessInstruction *this, int IID, void **p_IObj);

		int (*GetType) (struct MLProcessInstruction *this);

		const char * (*GetName) (struct MLProcessInstruction *this);
		int (*SetName) (struct MLProcessInstruction *this, char * name);

		int (*GetNumChilds) (struct MLProcessInstruction *this);
		int (*GetSingleChild) (struct MLProcessInstruction *this, char * name, IMLNode *out, int index); 
		/*int (*GetChild) (IMLProcessInstruction this, char * name, IMLNode *out); */
		int (*AppendChild) (struct MLProcessInstruction *this, IMLNode child, IMLNode *ret);
		int (*DeleteChildByInterfaz) (struct MLProcessInstruction *this, IMLNode child);
		int (*DeleteChildByIndex) (struct MLProcessInstruction *this, int index);

		unsigned int (*GetDataSize) (struct MLProcessInstruction *this);
		int (*GetData) (struct MLProcessInstruction *this, unsigned int *buffer_size, void **out_buffer);
		int (*SetData) (struct MLProcessInstruction *this, unsigned int buffer_size, void *in_buffer);
		int (*DeleteData) (struct MLProcessInstruction *this);

		int (*GetAttribute) (struct MLProcessInstruction *this, char * name, IMLAttribute *out);
		int (*SetAttribute) (struct MLProcessInstruction *this, IMLAttribute in);
		int (*DeleteAttribute) (struct MLProcessInstruction *this, char *name);

		int (*GetML) (struct MLProcessInstruction *this, unsigned int *buffer_size, char **out_buffer);
};


typedef struct MLProcessInstruction *IMLProcessInstruction;


/* Para iniciar los nodos de tipo processo*/
void ML_process_init (p_node nodo);

/* Para iniciar las interfaces IMLProcessInstruction*/
void mlprocess_init (IMLProcessInstruction interfaz, p_node objeto);

#endif /* !defined( ELEMENT_H )*/


