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

#if !defined(NODO_H)
#define NODO_H

#include "ML_COM_Sim.h"

struct node{
/*Aqui tenemos los componentes de cualquier objeto COM*/
		unsigned ref;
		int type;

		/*Con esto pretendo seguir las directrices de la OO en la medida de lo posible*/
		void (*Constructor) (p_com_obj this);
		void (*Destructor) (p_com_obj this);

		/*Esto es para aproximarme al COM*/
		void (*AddRef) (p_com_obj this);
		void (*Release) (p_com_obj this);
		int (*QueryInterface) (p_com_obj this, int IID, void **p_IObj);

/*Aqui comienzan los componentes del objeto nodo*/
		char name[256];
		int num_childs;
		struct node **childs;
		
		int size;
		void * data;

		/*Aqui van las funciones "privadas" de los objetos*/
		struct node * (*Copy) (struct node *this);
		int (*GetML) (struct node *this, unsigned int *buf_size, char **out_buf);

};

typedef struct node * p_node;

struct MLNode{

		p_node p_this;

		void (*AddRef) (struct MLNode *this);
		void (*Release) (struct MLNode *this);
		int (*QueryInterface) (struct MLNode *this, int IID, void **p_IObj);

		int (*GetType) (struct MLNode *this);

		const char *(*GetName) (struct MLNode *this);
		int (*SetName) (struct MLNode *this, char * name);
		
		int (*GetNumChilds) (struct MLNode *this);
		int (*GetSingleChild) (struct MLNode *this, char * name, struct MLNode **out, int index); 
		/*int (*GetChild) (struct MLNode *this, char * name, struct MLNode *out); */
		int (*AppendChild) (struct MLNode *this, struct MLNode *child, struct MLNode **ret);
		int (*DeleteChildByInterfaz) (struct MLNode *this, struct MLNode *child);
		int (*DeleteChildByIndex) (struct MLNode *this, int index);

		int (*GetML) (struct MLNode *this, unsigned int *buf_size, char **out_buf);
};

typedef struct MLNode * IMLNode;

/*********************************************************************************
		Funciones al uso para un objeto nodo sin datos referenciados
**********************************************************************************/
/*Constructor de clase base nodo*/
void node_Constructor (p_com_obj this);
/*Destructor de clase base nodo*/
void node_Destructor (p_com_obj this);

/*Funciones para manejar el componente, similares a las funciones de COM.Estas 
funcionan a nivel de objeto, no de interfaz*/
void node_AddRef (p_com_obj this);
void node_Release (p_com_obj this);
int node_QueryInterface (p_com_obj this, int IID, void **p_IObj);

/*Funcion de copia de objetos, se parece al operador de asignacion. Valida mientras
no haya en data datos referenciados, en ese caso hay que redefinirla.*/
p_node node_Clone (p_node this);



/*Es funcion inicia una interfaz (añade los metodos) y le asigna un objeto*/
void mlnode_init (IMLNode interfaz, p_node objeto);


#endif /*!defined(NODO_H)*/

