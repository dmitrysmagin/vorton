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


#include "ML_Attribute.h"
#include "ML_common_func.h"

/*************************************************************************

				Aqui trataremos los nodos Atributo y su interfaz

**************************************************************************

El proceso a seguir para crear una instancia de un objeto Atributo:

	- Crear la estructura nodo en memoria (ej. node = malloc(struct node);)
	- Llamar a node_init (ej. attribute_init(node);)
	- Crear la estructura de la interfaz en memoria 
			(ej. i_attribute = malloc (sizeof(struct MLAttribute));)
	- Llamar a interfaz_init (ej. mlattribute_init(i_attribute, node);
	- Devolver interfaz a usuario (ej. return i_attribute;)

Ejemplo (esta funcion sera el constructor de clase, que incluira el 
objeto MLDocument):

int CreateAttribute (IMLDocument this, int IID, void **interfaz)
{
	p_node node;
	
	switch (IID)
	{
	case IID_IMLAttribute:
		node = malloc (sizeof(struct node));
		attribute_init(node);
		*interfaz = malloc (sizeof(struct MLAttribute));
		IMLAttribute_init(*interfaz, node);
		return 0;

	default:
		return -2;
	}

	return -1;
}

*************************************************************************/

/**************************************************************************

						Nodos de tipo Atributo

***************************************************************************/



/*	El Constructor es lo unico que cambia por ahora.
	Tanto en el Destructor como la operacion Clone se utilizan la funciones
de la clase base nodo.
*/

void attribute_Constructor (p_com_obj this)
{
	node_Constructor((p_com_obj)this);
	this->type=NODE_Attribute;
}

/*   En los Atributos las unicas interfaces disponibles son IUnknown, 
IMLNode y IMLAttribute.
*/

int attribute_QueryInterface (p_com_obj this, int IID, void **p_IObj)
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

	case IID_IMLAttribute:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct MLAttribute));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		mlattribute_init((IMLAttribute)(*p_IObj),(p_node)this);
		
		return 0; /*Todo bien*/

	default:
		/* Marcar el error en el sistema de definicion de errores*/

		return -2; /*Interfaz no reconocida*/	
	}

	return -1;
}

int attribute_GetML (struct node *this, unsigned int *buf_size, char **out_buf)
{
	int i,num_txt;
	unsigned int total_size, child_size;
	char **buf_list;

	total_size=0;

	if (this->size==0)  /*tiene preferencia el campo data sobre los nodos hijos*/
	{
		if (this->num_childs!=0)
		{
			buf_list=(char **) malloc (this->num_childs*sizeof(char *));
			if (buf_list==NULL)
			{
				return -1;
			}
		}
		else buf_list=NULL;
	
		num_txt=0; /*este sera el indice para la el codigo de cada hijo*/
	
		/*Recogemos los trozos de texto, no puede haber otra cosa*/
		for (i=0;i<this->num_childs;i++)
		{
			if (this->childs[i]->type==NODE_Text)
			{
				if ( -1 == ( this->childs[i]->GetML( this->childs[i], &child_size, &(buf_list[num_txt]) ) ) )
				{
					for (i=0; i<num_txt;i++)
					{
						free (buf_list[i]);
					}
					if (buf_list) free (buf_list);
					return -1;
				}
				num_txt++;
				total_size+=child_size;
			}
		}
	
		/*Ya tenemos el tamaño que debera tener el buffer de salida*/
		/*el tamaño del nombre + 3 para la etiqueta y num_txt es por los espacios entre atributos*/
		total_size+=strlen(this->name)+3+num_txt;	
	
		*out_buf=(char *) malloc (total_size);
		if (*out_buf==NULL)
		{
			for (i=0; i<num_txt;i++)
			{
				free (buf_list[i]);
			}
			if (buf_list) free (buf_list);
			return -1;
		}
		
		strcpy(*out_buf,this->name);
		strcat(*out_buf,"=\"");
		for (i=0;i<num_txt;i++)
		{
			strcat(*out_buf,buf_list[i]);
		}
		strcat(*out_buf,"\"");
		
		*buf_size=total_size;

		/*Hay que liberar la memoria utilizada en el proceso*/
		for (i=0; i<num_txt;i++)
		{
			free (buf_list[i]);
		}
		if (buf_list) free (buf_list);
	}
	else
	{
		/*el tamaño del nombre + 3 para la etiqueta*/
		total_size+=this->size;
		total_size+=strlen(this->name)+4;	
		*out_buf=(char *) malloc (total_size);
		if (*out_buf==NULL) return -1;
		*buf_size=total_size;

		strcpy(*out_buf,this->name);
		strcat(*out_buf,"=\"");
		memcpy(*out_buf+(strlen(*out_buf)*sizeof(char)),this->data, this->size); /*no es una cadena lo que copiamos*/
		(*out_buf)[total_size-2]='"';
		(*out_buf)[total_size-1]='\0';
	}

	return 0;
}

/*Sencilla inicializacion de un nodo tipo Attributeo*/
void attribute_init (p_node nodo)
{
	nodo->Constructor=attribute_Constructor;
	nodo->Destructor=node_Destructor;
	nodo->AddRef=node_AddRef;
	nodo->Release=node_Release;
	nodo->QueryInterface=attribute_QueryInterface;
	nodo->Copy=node_Clone;
	nodo->GetML=attribute_GetML;
	nodo->Constructor((p_com_obj)nodo);
}

/******************************************************************************
	************************* IMLAttribute *******************************

	Aqui comienza la interfaz IMLAttribute para objetos del tipo nodo_Attributeo
*******************************************************************************/

void mlattribute_AddRef (IMLAttribute this)
{
	this->p_this->AddRef((p_com_obj)this->p_this);
}

void mlattribute_Release (IMLAttribute this)
{
	this->p_this->Release((p_com_obj)this->p_this);
	 
	free (this);
}

int mlattribute_QueryInterface (IMLAttribute this, int IID, void **p_IObj)
{
	return this->p_this->QueryInterface((p_com_obj)this->p_this,IID,p_IObj);
}

int mlattribute_GetType (IMLAttribute this)
{
	return this->p_this->type;
}

const char *mlattribute_GetName (IMLAttribute this)
{
	return this->p_this->name;
}

int mlattribute_SetName (IMLAttribute this, char * name)
{
	strncpy(this->p_this->name,name,256);
	return 0;
}

int mlattribute_GetNumChilds (IMLAttribute this)
{
	return this->p_this->num_childs;
}

int mlattribute_GetSingleChild (IMLAttribute this, char * name, IMLNode *out, int index)
{
	return common_GetSingleChild (this->p_this, name, out, index);
}

int mlattribute_AppendChild (IMLAttribute this, IMLNode child, IMLNode *ret)
{
	if (child->p_this->type != NODE_Text)
	{
		return -2;/*tipo de nodo no permitido para colgar de un atributo*/
	}

	return common_AppendChild (this->p_this, child, ret);
}

int mlattribute_DeleteChildByInterfaz (IMLAttribute this, IMLNode child)
{
	return common_DeleteChildByInterfaz (this->p_this, child);
}

int mlattribute_DeleteChildByIndex (IMLAttribute this, int index)
{
	return common_DeleteChildByIndex (this->p_this, index);
}


int mlattribute_GetDataSize (IMLAttribute this)
{
	return this->p_this->size;
}

int mlattribute_GetData (IMLAttribute this, int *buffer_size, void **out_buffer)
{
	return common_GetData (this->p_this, buffer_size, out_buffer);
}

int mlattribute_SetData (IMLAttribute this, int buffer_size, void *in_buffer)
{
	return common_SetData (this->p_this, buffer_size, in_buffer);
}

int mlattribute_DeleteData (IMLAttribute this)
{
	if (this->p_this->size)
	{
		free(this->p_this->data);
		this->p_this->size=0;
	}
	else return -1;


	return 0;
}

int mlattribute_GetML (IMLAttribute this, unsigned int *buffer_size, char **out_buffer)
{
	return this->p_this->GetML(this->p_this,buffer_size,out_buffer);
}

/*Funcion de inicializacion de interfaz*/
void mlattribute_init (IMLAttribute interfaz, p_node objeto)
{
	interfaz->p_this = objeto;
	objeto->AddRef((p_com_obj)objeto);
	interfaz->AddRef = mlattribute_AddRef;
	interfaz->Release = mlattribute_Release;
	interfaz->QueryInterface = mlattribute_QueryInterface;
	interfaz->GetType = mlattribute_GetType;
	interfaz->GetName = mlattribute_GetName;
	interfaz->SetName = mlattribute_SetName;
	interfaz->GetNumChilds = mlattribute_GetNumChilds;
	interfaz->GetSingleChild = mlattribute_GetSingleChild;
	interfaz->AppendChild = mlattribute_AppendChild;
	interfaz->DeleteChildByInterfaz = mlattribute_DeleteChildByInterfaz;
	interfaz->DeleteChildByIndex = mlattribute_DeleteChildByIndex;
	interfaz->GetDataSize = mlattribute_GetDataSize;
	interfaz->GetData = mlattribute_GetData;
	interfaz->SetData = mlattribute_SetData;
	interfaz->DeleteData = mlattribute_DeleteData;
	interfaz->GetML = mlattribute_GetML;
}





