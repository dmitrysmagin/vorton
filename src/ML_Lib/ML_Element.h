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

#if !defined(ELEMENT_H)
#define ELEMENT_H

#include "ML_Node.h"
#include "ML_Attribute.h"
#include "ML_Text.h"
#include "ML_Comment.h"


struct MLElement{

		p_node p_this;

		void (*AddRef) (struct MLElement *this);
		void (*Release) (struct MLElement *this);
		int (*QueryInterface) (struct MLElement *this, int IID, void **p_IObj);

		int (*GetType) (struct MLElement *this);

		const char * (*GetName) (struct MLElement *this);
		int (*SetName) (struct MLElement *this, char * name);

		int (*GetNumChilds) (struct MLElement *this);
		int (*GetSingleChild) (struct MLElement *this, char * name, IMLNode *out, int index); 
		/*int (*GetChild) (IMLElement this, char * name, IMLNode *out); */
		int (*AppendChild) (struct MLElement *this, IMLNode child, IMLNode *ret);
		int (*DeleteChildByInterfaz) (struct MLElement *this, IMLNode child);
		int (*DeleteChildByIndex) (struct MLElement *this, int index);

/*		unsigned int (*GetDataSize) (struct MLElement *this);
		int (*GetData) (struct MLElement *this, unsigned int buffer_size, void *out_buffer);
		int (*SetData) (struct MLElement *this, unsigned int buffer_size, void *in_buffer);
		int (*DeleteData) (struct MLElement *this);
*/
		int (*GetAttribute) (struct MLElement *this, char * name, IMLAttribute *out);
		int (*SetAttribute) (struct MLElement *this, IMLAttribute in);
		int (*DeleteAttribute) (struct MLElement *this, char *name);

		int (*GetSingleElement) (struct MLElement *this, char * name, struct MLElement **out, int index); 
		int (*AppendElement) (struct MLElement *this, struct MLElement *in, struct MLElement **ret);
		int (*DeleteElement) (struct MLElement *this, struct MLElement *in);

		int (*AppendText) (struct MLElement *this, IMLText in, IMLText *ret);
		int (*AppendComment) (struct MLElement *this, IMLComment in, IMLComment *ret);

		int (*GetML) (struct MLElement *this, unsigned int *buffer_size, char **out_buffer);
};


typedef struct MLElement *IMLElement;


/* Para iniciar los nodos de tipo elemento*/
void element_init (p_node nodo);

/* Para iniciar las interfaces IMLElement*/
void mlelement_init (IMLElement interfaz, p_node objeto);

#endif /* !defined( ELEMENT_H )*/


