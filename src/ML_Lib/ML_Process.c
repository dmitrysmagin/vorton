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

#include "ML_Process.h"
#include "ML_common_func.h"

/*************************************************************************

				Aqui trataremos los nodos proceso y su interfaz

**************************************************************************

El proceso a seguir para crear una instancia de un objeto proceso:

	- Crear la estructura nodo en memoria (ej. node = malloc(struct node);)
	- Llamar a node_init (ej. ML_process_init(node);)
	- Crear la estructura de la interfaz en memoria 
			(ej. i_process = malloc (sizeof(struct MLProcessInstruction));)
	- Llamar a interfaz_init (ej. mlprocess_init(i_process, node);
	- Devolver interfaz a usuario (ej. return i_process;)

Ejemplo (esta funcion sera el constructor de clase, que incluira el 
objeto MLDocument):

int CreateProcessInstruction (IMLDocument this, int IID, void **interfaz)
{
	p_node node;
	
	switch (IID)
	{
	case IID_IMLProcessInstruction:
		node = malloc (sizeof(struct node));
		ML_process_init(node);
		*interfaz = malloc (sizeof(struct MLProcessInstruction));
		IMLProcessInstruction_init(*interfaz, node);
		return 0;

	default:
		return -2;
	}

	return -1;
}

*************************************************************************/

/**************************************************************************

						Nodos de tipo proceso

***************************************************************************/



/*	El Constructor es lo unico que cambia por ahora.
	Tanto en el Destructor como la operacion Clone se utilizan la funciones
de la clase base nodo.
*/

void process_Constructor (p_com_obj this)
{
	node_Constructor(this);
	this->type=NODE_ProcessInstruction;
}

/*   En los processos las unicas interfaces disponibles son IUnknown, 
IMLNode y IMLProcessInstruction.
*/

int process_QueryInterface (p_com_obj this, int IID, void **p_IObj)
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

	case IID_IMLProcessInstruction:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct MLProcessInstruction));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		mlprocess_init((IMLProcessInstruction)(*p_IObj),(p_node)this);
		
		return 0; /*Todo bien*/

	default:
		/* Marcar el error en el sistema de definicion de errores*/

		return -2; /*Interfaz no reconocida*/	
	}

	return -1;
}

int process_GetML (struct node *this, unsigned int *buf_size, char **out_buf)
{
	int i,num_attr;
	unsigned int total_size, child_size;
	char **buf_list;

	if (this->num_childs!=0)
	{
		buf_list=(char **) malloc (this->num_childs*sizeof(char *));
		if (buf_list==NULL)
		{
			return -1;
		}
	}
	else buf_list=NULL;

	num_attr=0; /*este sera el indice para la el codigo de cada hijo*/
	total_size=0;

	/*Recogemos los atributos, no puede haber otra cosa*/
	for (i=0;i<this->num_childs;i++)
	{
		if (this->childs[i]->type==NODE_Attribute)
		{
			if ( -1 == ( this->childs[i]->GetML( this->childs[i], &child_size, &(buf_list[num_attr]) ) ) )
			{
				for (i=0; i<num_attr;i++)
				{
					if (buf_list[i]) free (buf_list[i]);
				}
				free (buf_list);
				return -1;
			}
			num_attr++;
			total_size+=child_size-1;
		}
	}

	/*Ya tenemos el tamaño que debera tener el buffer de salida*/
	/*el tamaño del nombre + 5 para la etiqueta y num_attr es por los espacios entre atributos*/
	total_size+=strlen(this->name)+5+num_attr;	

	*out_buf=(char *) malloc (total_size);
	if (*out_buf==NULL)
	{
		for (i=0; i<num_attr;i++)
		{
			if (buf_list[i]) free (buf_list[i]);
		}
		if (buf_list) free (buf_list);
		return -1;
	}
	
	strcpy(*out_buf,"<?");
	strcat(*out_buf,this->name);
	for (i=0;i<num_attr;i++)
	{
		strcat(*out_buf," ");
		strcat(*out_buf,buf_list[i]);
	}
	strcat(*out_buf,"?>");
	
	*buf_size=total_size;

	/*Hay que liberar la memoria utilizada en el proceso*/
	for (i=0; i<num_attr;i++)
	{
		if (buf_list[i]) free (buf_list[i]);
	}
	if (buf_list) free (buf_list);

	return 0;
}

/*Sencilla inicializacion de un nodo tipo processo*/
void ML_process_init (p_node nodo)
{
	nodo->Constructor=process_Constructor;
	nodo->Destructor=node_Destructor;
	nodo->AddRef=node_AddRef;
	nodo->Release=node_Release;
	nodo->QueryInterface=process_QueryInterface;
	nodo->Copy=node_Clone;
	nodo->GetML=process_GetML;
	nodo->Constructor((p_com_obj)nodo);
}

/******************************************************************************
	************************* IMLProcessInstruction *******************************

	Aqui comienza la interfaz IMLProcessInstruction para objetos del tipo nodo_processo
*******************************************************************************/

void mlprocess_AddRef (IMLProcessInstruction this)
{
	this->p_this->AddRef((p_com_obj)this->p_this);
}

void mlprocess_Release (IMLProcessInstruction this)
{
	this->p_this->Release((p_com_obj)this->p_this);
	 
	free (this);
}

int mlprocess_QueryInterface (IMLProcessInstruction this, int IID, void **p_IObj)
{
	return this->p_this->QueryInterface((p_com_obj)this->p_this,IID,p_IObj);
}

int mlprocess_GetType (IMLProcessInstruction this)
{
	return this->p_this->type;
}

const char *mlprocess_GetName (IMLProcessInstruction this)
{
	return this->p_this->name;
}

int mlprocess_SetName (IMLProcessInstruction this, char * name)
{
	strncpy(this->p_this->name,name,256);
	return 0;
}

int mlprocess_GetNumChilds (IMLProcessInstruction this)
{
	return this->p_this->num_childs;
}

int mlprocess_GetSingleChild (IMLProcessInstruction this, char * name, IMLNode *out, int index)
{
	return common_GetSingleChild (this->p_this, name, out, index);
}

int mlprocess_AppendChild (IMLProcessInstruction this, IMLNode child, IMLNode *ret)
{
	switch(child->p_this->type)
	{
	case NODE_Attribute:
	case NODE_Text:
		break;

	default:
		return -2;/*tipo de nodo no permitido para colgar de un processo*/
	}

	return common_AppendChild (this->p_this, child, ret);
}

int mlprocess_DeleteChildByInterfaz (IMLProcessInstruction this, IMLNode child)
{
	return common_DeleteChildByInterfaz (this->p_this, child);
}

int mlprocess_DeleteChildByIndex (IMLProcessInstruction this, int index)
{
	return common_DeleteChildByIndex (this->p_this, index);
}


int mlprocess_GetAttribute (IMLProcessInstruction this, char * name, IMLAttribute *out)
{
	int i;

	for (i=0;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Attribute)&&(strcmp(this->p_this->childs[i]->name,name)==0))
		{
			*out=(IMLAttribute) malloc (sizeof(struct MLAttribute));
			if (*out==NULL) return -1;/*No hay memoria*/
			mlattribute_init(*out,this->p_this->childs[i]);
			return i;
		}
	}

	return -2;
}

int mlprocess_SetAttribute (IMLProcessInstruction this, IMLAttribute in)
{
	int i;
	void *p_tmp;
	p_node tmp,*tmp1;

	for (i=0;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Attribute)&&(strcmp(this->p_this->childs[i]->name,in->p_this->name)==0))
		{
			p_tmp=malloc(in->p_this->size); /*Nos aseguramos de en caso de error poder recuperarnos*/
			if (p_tmp==NULL) return -1;
			
			if (this->p_this->childs[i]->size!= 0)
			{
				free (this->p_this->childs[i]->data);
			}
			this->p_this->childs[i]->data=p_tmp;
			this->p_this->childs[i]->size=in->p_this->size;
			memcpy(this->p_this->childs[i]->data,in->p_this->data,in->p_this->size);
			return i;
		}
	}

	tmp = in->p_this->Copy(in->p_this);
	if (tmp==NULL)
	{
		return -1;
	}

	tmp1 = (p_node *) malloc (sizeof(p_node)*(this->p_this->num_childs+1));

	if (tmp1==NULL) 
	{
		tmp->Release((p_com_obj)tmp);
		return -1; /*por si no hay memoria*/
	}

	for (i=0;i<this->p_this->num_childs;i++)
	{
		tmp1[i]=this->p_this->childs[i];
	}

	tmp1[i]=tmp;
	tmp->AddRef((p_com_obj)tmp);
	if (this->p_this->childs) free(this->p_this->childs);
	this->p_this->childs=tmp1;
	this->p_this->num_childs++;

	return i;
}

int mlprocess_DeleteAttribute (IMLProcessInstruction this, char *name)
{
	int i,j,k;
	p_node *tmp;

	for (i=0;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Attribute)&&(strcmp(this->p_this->childs[i]->name,name)==0))
		{
			/*liberamos el enlace del objeto y si es el ultimo este se autodestruira*/
			this->p_this->childs[i]->Release((p_com_obj)this->p_this->childs[i]);

			/*Creamos la nueva lista de punteros de objeto*/
			tmp = (p_node *) malloc (sizeof(p_node)*(this->p_this->num_childs-1));


			if (tmp==NULL) return -1; /*por si no hay memoria*/

			j=0;
			for (k=0;k<this->p_this->num_childs;k++)
			{
				if (k!=i)
					tmp[j++]=this->p_this->childs[k];
			}

			free (this->p_this->childs);/*borramos la lista vieja*/
			this->p_this->childs=tmp;/*ponemos la lista nueva*/
			this->p_this->num_childs--;
			return 1;
		}
	}

	return -2;
}

/*todavia no se si utilizare el campo data*/
unsigned int mlprocess_GetDataSize (IMLProcessInstruction this)
{
	return this->p_this->size;
}

int mlprocess_GetData (IMLProcessInstruction this, unsigned int *buffer_size, void **out_buffer)
{
	if (this->p_this->size)
	{
		*buffer_size=this->p_this->size;
		*out_buffer=malloc(this->p_this->size);
		if (*out_buffer==NULL) return -1; /*no hay memoria*/
		memcpy(*out_buffer,this->p_this->data,this->p_this->size);
		return 0;
	}
	else
	{
		*buffer_size=0;
		*out_buffer=NULL;
		return -2;
	}
}

int mlprocess_SetData (IMLProcessInstruction this, unsigned int buffer_size, void *in_buffer)
{
	if (this->p_this->size)
	{
		free(this->p_this->data);
	}

	this->p_this->size=buffer_size;
	this->p_this->data=malloc(this->p_this->size);
	if (this->p_this->data==NULL) return -1; /*no hay memoria*/
	memcpy(this->p_this->data,in_buffer,buffer_size);
	return 0;
}

int mlprocess_DeleteData (IMLProcessInstruction this)
{
	if (this->p_this->size)
	{
		free(this->p_this->data);
		this->p_this->size=0;
	}
	else return -2;


	return 0;
}

int mlprocess_GetML (IMLProcessInstruction this, unsigned int *buffer_size, char **out_buffer)
{
	return this->p_this->GetML(this->p_this,buffer_size,out_buffer);
}

/*Funcion de inicializacion de interfaz*/
void mlprocess_init (IMLProcessInstruction interfaz, p_node objeto)
{
	interfaz->p_this = objeto;
	objeto->AddRef((p_com_obj)objeto);
	interfaz->AddRef = mlprocess_AddRef;
	interfaz->Release = mlprocess_Release;
	interfaz->QueryInterface = mlprocess_QueryInterface;
	interfaz->GetType = mlprocess_GetType;
	interfaz->GetName = mlprocess_GetName;
	interfaz->SetName = mlprocess_SetName;
	interfaz->GetNumChilds = mlprocess_GetNumChilds;
	interfaz->GetSingleChild = mlprocess_GetSingleChild;
	interfaz->AppendChild = mlprocess_AppendChild;
	interfaz->DeleteChildByInterfaz = mlprocess_DeleteChildByInterfaz;
	interfaz->DeleteChildByIndex = mlprocess_DeleteChildByIndex;
	interfaz->GetAttribute = mlprocess_GetAttribute;
	interfaz->SetAttribute = mlprocess_SetAttribute;
	interfaz->DeleteAttribute = mlprocess_DeleteAttribute;
	interfaz->GetDataSize = mlprocess_GetDataSize;
	interfaz->GetData = mlprocess_GetData;
	interfaz->SetData = mlprocess_SetData;
	interfaz->DeleteData = mlprocess_DeleteData;
	interfaz->GetML = mlprocess_GetML;
}



