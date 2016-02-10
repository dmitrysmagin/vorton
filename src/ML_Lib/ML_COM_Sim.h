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

#if !defined(COM_SIM_H)
#define COM_SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {NODE_0,DOCUMENT_0, DOCUMENT_1,NODE_Element,NODE_Attribute,NODE_Text,NODE_ProcessInstruction,NODE_Comment};
enum {IID_IUnknown,IID_IMLDocument,IID_IMLNode,IID_IMLElement,IID_IMLAttribute,IID_IMLText,IID_IMLProcessInstruction,IID_IMLComment};

struct com_obj{
	unsigned ref;
	int type;

	/*Con esto pretendo seguir las directrices de la OO en la medida de lo posible*/
	void (*Constructor) (struct com_obj *this);
	void (*Destructor) (struct com_obj *this);

	/*Esto es para aproximarme al COM*/
	void (*AddRef) (struct com_obj *this);
	void (*Release) (struct com_obj *this);
	int (*QueryInterface) (struct com_obj *this, int IID, void **p_IObj);
};

typedef struct com_obj *p_com_obj;

struct  Unknown {

		p_com_obj p_this;

		void (*AddRef) (struct  Unknown *this);
		void (*Release) (struct  Unknown *this);
		int (*QueryInterface) (struct  Unknown *this, int IID, void **p_IObj);

};

typedef struct Unknown * IUnknown;


/*Es funcion inicia una interfaz (añade los metodos) y le asigna un objeto*/
void unknown_init (IUnknown interfaz, p_com_obj objeto);

#endif /*if !defined(COM_SIM_H)*/


