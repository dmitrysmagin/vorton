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


#include "ML_Text.h"
#include "ML_common_func.h"

/*************************************************************************

				Aqui trataremos los nodos Texto y su interfaz

**************************************************************************

	La estructura de este fichero es similar a la del resto de nodos.

*************************************************************************/

/**************************************************************************

						Nodos de tipo Texto

***************************************************************************/



/*	El Constructor es lo unico que cambia por ahora.
	Tanto en el Destructor como la operacion Clone se utilizan la funciones
de la clase base nodo.
*/

void text_Constructor (p_com_obj this)
{
	node_Constructor(this);
	this->type=NODE_Text;
}

/*   En los Atributos las unicas interfaces disponibles son IUnknown, 
IMLNode y IMLText.
*/

int text_QueryInterface (p_com_obj this, int IID, void **p_IObj)
{
	switch(IID)
	{
	case IID_IUnknown:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct Unknown));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		unknown_init((IUnknown)(*p_IObj),this);
		
		return 0; /*Todo bien*/

	case IID_IMLNode:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct MLNode));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		mlnode_init((IMLNode)(*p_IObj),(p_node)this);
		
		return 0; /*Todo bien*/

	case IID_IMLText:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct MLText));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		mltext_init((IMLText)(*p_IObj),(p_node)this);
		
		return 0; /*Todo bien*/

	default:
		/* Marcar el error en el sistema de definicion de errores*/

		return -2; /*Interfaz no reconocida*/	
	}

	return -1;
}

int text_GetML (struct node *this, unsigned int *buf_size, char **out_buf)
{
	if (this->size==0)  /*tiene preferencia el campo data sobre los nodos hijos*/
	{
		return -2; /*Habra que estudiar la posibilidad de mandar un buffer vacio*/
	}
	else
	{
		*out_buf=(char *) malloc (this->size+1);
		if (*out_buf==NULL) return -1;
		*buf_size=this->size+1;

		memcpy(*out_buf,this->data, this->size); /*no es una cadena lo que copiamos*/
		memcpy(*out_buf+this->size,"\0",1 );
	}

	return 0;
}


/*Sencilla inicializacion de un nodo tipo Texto*/
void text_init (p_node nodo)
{
	nodo->Constructor=text_Constructor;
	nodo->Destructor=node_Destructor;
	nodo->AddRef=node_AddRef;
	nodo->Release=node_Release;
	nodo->QueryInterface=text_QueryInterface;
	nodo->Copy=node_Clone;
	nodo->GetML=text_GetML;
	nodo->Constructor((p_com_obj)nodo);
}

/******************************************************************************
	************************* IMLText *******************************

	Aqui comienza la interfaz IMLText para objetos del tipo nodo_Texto
*******************************************************************************/

void mltext_AddRef (IMLText this)
{
	this->p_this->AddRef((p_com_obj)this->p_this);
}

void mltext_Release (IMLText this)
{
	this->p_this->Release((p_com_obj)this->p_this);
	 
	free (this);
}

int mltext_QueryInterface (IMLText this, int IID, void **p_IObj)
{
	return this->p_this->QueryInterface((p_com_obj)this->p_this,IID,p_IObj);
}

int mltext_GetType (IMLText this)
{
	return this->p_this->type;
}

const char *mltext_GetName (IMLText this)
{
	return this->p_this->name;
}

int mltext_SetName (IMLText this, char * name)
{
	strncpy(this->p_this->name,name,256);
	return 0;
}

int mltext_GetDataSize (IMLText this)
{
	return this->p_this->size;
}

int mltext_GetData (IMLText this, int *buffer_size, void **out_buffer)
{
	if (this->p_this->size)
	{
		*buffer_size=this->p_this->size;
		*out_buffer=malloc(this->p_this->size);
		if (*out_buffer==NULL) return -2; /*no hay memoria*/
		memcpy(*out_buffer,this->p_this->data,this->p_this->size);
		return 1;
	}
	return -1;
}

int mltext_SetData (IMLText this, int buffer_size, void *in_buffer)
{
	if (this->p_this->size)
	{
		free(this->p_this->data);
	}

	this->p_this->size=buffer_size;
	this->p_this->data=malloc(this->p_this->size);
	if (this->p_this->data==NULL) {this->p_this->size=0; return -2;}/*no hay memoria*/
	memcpy(this->p_this->data,in_buffer,buffer_size);
	return 1;
}

int mltext_DeleteData (IMLText this)
{
	if (this->p_this->size)
	{
		free(this->p_this->data);
		this->p_this->size=0;
	}
	else return -1;


	return 0;
}

int mltext_GetML (IMLText this, unsigned int *buffer_size, char **out_buffer)
{
	return this->p_this->GetML(this->p_this,buffer_size,out_buffer);
}

/*Funcion de inicializacion de interfaz*/
void mltext_init (IMLText interfaz, p_node objeto)
{
	interfaz->p_this = objeto;
	objeto->AddRef((p_com_obj)objeto);
	interfaz->AddRef = mltext_AddRef;
	interfaz->Release = mltext_Release;
	interfaz->QueryInterface = mltext_QueryInterface;
	interfaz->GetType = mltext_GetType;
	interfaz->GetName = mltext_GetName;
	interfaz->SetName = mltext_SetName;
	interfaz->GetDataSize = mltext_GetDataSize;
	interfaz->GetData = mltext_GetData;
	interfaz->SetData = mltext_SetData;
	interfaz->DeleteData = mltext_DeleteData;
	interfaz->GetML = mltext_GetML;
}


