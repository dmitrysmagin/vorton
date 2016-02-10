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

#include "ML_Node.h"
#include "ML_common_func.h"


/*************************************************************************

				 Ejemplo de funcion de inicio de clase

**************************************************************************

void node_init (p_node nodo)
{
	nodo->Constructor=node_Constructor;
	nodo->Destructor=node_Destructor;
	nodo->AddRef=node_AddRef;
	nodo->Release=node_Release;
	nodo->QueryInterface=node_QueryInterface;
	nodo->Copy=node_Clone;
	nodo->Constructor(nodo);
}


El proceso a seguir para crear una instancia de una clase de nodo:

	- Crear la estructura nodo en memoria (ej. node = malloc(struct node);)
	- Llamar a node_init (ej. element_init(node);)
	- Crear la estructura de la interfaz en memoria 
			(ej. i_element = malloc (sizeof(struct MLElement));)
	- Llamar a interfaz_init (ej. IMLElement_init(i_element, node);
	- Devolver interfaz a usuario (ej. return i_element;)

Ejemplo:
int CreateElement (IMLDocument this, int IID, void **interfaz)
{
	p_node node;
	
	switch (IID)
	{
	case IID_IMLElement:
		node = malloc (sizeof(struct node));
		element_init(node);
		*interfaz = malloc (sizeof(struct MLElement));
		IMLElement_init(*interfaz, node);
		return 0;

	default:
		return -2;
	}

	return -1;
}

*************************************************************************/

/*
		Estas funciones pueden utilizarse en como constructores o 
	destructores de base llamandolas desde los constructores y destructores 
	de los nodos elemento o atributo...
*/

void node_Constructor (p_com_obj obj)
{
	p_node this;
/*Hago una conversion y asi el codigo es mas facil, a parte de no cambiar desde la version 0.1*/
	this= (p_node)obj;

	this->ref=0;
	this->type=NODE_0;

	strcpy(this->name,"");
	this->num_childs=0;
	this->childs=NULL;

	this->size=0;
	this->data=NULL;
}


void node_Destructor (p_com_obj obj)
{
	int i;
	p_node this; 
/*Hago una conversion y asi el codigo es mas facil, a parte de no cambiar desde la version 0.1*/
	this= (p_node)obj; 

	for (i=0;i<((p_node)this)->num_childs;i++)
	{
		this->childs[i]->Release((p_com_obj)this->childs[i]);
	}

	if (this->childs) free(this->childs);

	if (this->size!=0) /*quizas esto sea redundante*/
	{
		if (this->data!=NULL) free (this->data);
	}

	free(this);
	
}

/*    Estas funciones pueden ser definidas de forma diferente en cada tipo de nodo.
	Aunque no es necesario ya que la liberacion de recursos se hace en el destructor.
*/

void node_AddRef (p_com_obj this)
{
	this->ref++;
}

void node_Release (p_com_obj this)
{
	this->ref--;
	if (this->ref<=0) this->Destructor(this);
}


/*   Esta funcion se define en cada tipo de nodo y determina las interfaces disponibles
solo la pongo a modo ejemplo, pero es lo mas basico que puede haber en un objeto COM*/

int node_QueryInterface (p_com_obj this, int IID, void **p_IObj)
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

	default:
		/* Marcar el error en el sistema de definicion de errores*/

		return -2; /*Interfaz no reconocida*/	
	}

	return -1;
}


/***************************************************************************
		Esta funcion es necesario reescribirla en las subclases en las que el
	campo data del nodo contenga algun dato referenciado (p.e. lista enlazada).
****************************************************************************/
p_node node_Clone (p_node this)
{
	int i,j;
	p_node tmp;

	tmp = (p_node) malloc (sizeof(struct node));
	if (tmp==NULL) return NULL;

	tmp->ref=0;
	tmp->type=this->type;

	strcpy(tmp->name,this->name);
	tmp->size=this->size;
	if (tmp->size!=0)
	{
		tmp->data=malloc(this->size);
		if (tmp->data==NULL)
		{
			free(tmp);
			return NULL;
		}
		memcpy(tmp->data,this->data,this->size);
	}
	else tmp->data=NULL;

	tmp->num_childs=this->num_childs;
	if (this->num_childs>0)
	{
		tmp->childs=(p_node *) malloc (this->num_childs*sizeof(p_node));
		if (tmp->childs==NULL)
		{
			if (tmp->data) free (tmp->data);
			free(tmp);
			return NULL;
		}
	
		for (i=0;i<this->num_childs;i++)
		{
			tmp->childs[i]=this->childs[i]->Copy(this->childs[i]);
			if (tmp->childs[i]==NULL) /*limpiar la memoria es fundamental*/
			{
				if (tmp->data) free (tmp->data);
				for (j=0;j<i;j++)
				{
					tmp->childs[j]->Release((p_com_obj)tmp->childs[j]);
				}
				if (tmp->childs) free (tmp->childs);
				if (tmp) free(tmp);
				return NULL;
			}
			tmp->childs[i]->AddRef((p_com_obj)tmp->childs[i]);
		}
	}
	else tmp->childs=NULL;

	tmp->Constructor=this->Constructor;
	tmp->Destructor=this->Destructor;
	tmp->AddRef=this->AddRef;
	tmp->Release=this->Release;
	tmp->QueryInterface=this->QueryInterface;
	tmp->Copy=this->Copy;
	tmp->GetML=this->GetML;

	return tmp;
}



/******************************************************************************
		Aqui comienza la interfaz IMLNode para objetos del tipo nodo
*******************************************************************************/
/*Funciones COM*/

void mlnode_AddRef (IMLNode this)
{
	this->p_this->AddRef((p_com_obj)this->p_this);
}

void mlnode_Release (IMLNode this)
{
	this->p_this->Release((p_com_obj)this->p_this);
	 
	free (this);
}

int mlnode_QueryInterface (IMLNode this, int IID, void **p_IObj)
{
	return this->p_this->QueryInterface((p_com_obj)this->p_this, IID, p_IObj);
}

/*Para conocer el tipo de nodo con el que tratamos, asi podriamos solicitar
la interfaz adecuada con QueryInterface para obtener mayor fucnionalidad*/
int mlnode_GetType (IMLNode this)
{
	return this->p_this->type;
}

/*Para controlar el nombre de los nodos*/
const char *mlnode_GetName (IMLNode this)
{
	return this->p_this->name;
}

int mlnode_SetName (IMLNode this, char * name)
{
	strncpy(this->p_this->name,name,256);
	return 0;
}

int mlnode_GetNumChilds (IMLNode this)
{
	return this->p_this->num_childs;
}

/*Para manejar los nodos descendientes. En futuras versiones se añadira
soporte para XPath en funciones tipo GetChild()*/
int mlnode_GetSingleChild (IMLNode this, char * name, IMLNode *out, int index)
{
	return common_GetSingleChild (this->p_this, name, out, index);
}

int mlnode_AppendChild (IMLNode this, IMLNode child, IMLNode *ret)
{
	return common_AppendChild (this->p_this, child, ret);
}

int mlnode_DeleteChildByInterfaz (IMLNode this, IMLNode child)
{
	return common_DeleteChildByInterfaz (this->p_this, child);
}

int mlnode_DeleteChildByIndex (IMLNode this, int index)
{
	return common_DeleteChildByIndex (this->p_this, index);
}

int mlnode_GetML (IMLNode this, unsigned int *buffer_size, char **out_buffer)
{
	return this->p_this->GetML(this->p_this,buffer_size,out_buffer);
}

void mlnode_init (IMLNode interfaz, p_node objeto)
{
	interfaz->p_this = objeto;
	objeto->AddRef((p_com_obj)objeto);
	interfaz->AddRef = mlnode_AddRef;
	interfaz->Release = mlnode_Release;
	interfaz->QueryInterface = mlnode_QueryInterface;
	interfaz->GetType = mlnode_GetType;
	interfaz->GetName = mlnode_GetName;
	interfaz->SetName = mlnode_SetName;
	interfaz->GetNumChilds = mlnode_GetNumChilds;
	interfaz->GetSingleChild = mlnode_GetSingleChild;
	interfaz->AppendChild = mlnode_AppendChild;
	interfaz->DeleteChildByInterfaz = mlnode_DeleteChildByInterfaz;
	interfaz->DeleteChildByIndex = mlnode_DeleteChildByIndex;
	interfaz->GetML=mlnode_GetML;
}

