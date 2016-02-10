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

#include "ML_Element.h"
#include "ML_common_func.h"

/*************************************************************************

				Aqui trataremos los nodos elemento y su interfaz

**************************************************************************


/**************************************************************************
						Nodos de tipo elemento
***************************************************************************/

/*	El Constructor es lo unico que cambia por ahora.
	Tanto en el Destructor como la operacion Clone se utilizan la funciones
de la clase base nodo.
*/

void element_Constructor (p_com_obj obj)
{
	p_node this;
	this=(p_node)obj;

	node_Constructor(obj);
	this->type=NODE_Element;
}

/*   En los elementos las unicas interfaces disponibles son IUnknown, 
IMLNode y IMLElement.
*/

int element_QueryInterface (p_com_obj this, int IID, void **p_IObj)
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

	case IID_IMLElement:
		*p_IObj = NULL;
		*p_IObj = malloc (sizeof(struct MLElement));
		if (*p_IObj == NULL) 
		{
			/*Marcar error en sistema de definicion de errores*/
			return -3; /*No hay memoria disponible*/
		}
		
		mlelement_init((IMLElement)(*p_IObj),(p_node)this);
		
		return 0; /*Todo bien*/

	default:
		/* Marcar el error en el sistema de definicion de errores*/

		return -2; /*Interfaz no reconocida*/	
	}

	return -1;
}

int element_GetML (struct node *this, unsigned int *buf_size, char **out_buf)
{
	int i,j,num_attr;
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

	j=0; /*este sera el indice para la el codigo de cada hijo*/
	total_size=0;

	/*Primero hago una pasada en busca de los atributos, ya que los hijos no llevan un orden obligatorio*/
	for (i=0;i<this->num_childs;i++)
	{
		if (this->childs[i]->type==NODE_Attribute)
		{
			if ( -1 == ( this->childs[i]->GetML( this->childs[i], &child_size, &(buf_list[j]) ) ) )
			{
				for (i=0; i<j;i++)
				{
					if (buf_list[i]) free (buf_list[i]);
				}
				if (buf_list) free (buf_list);
				return -1;
			}
			j++;
			total_size+=child_size-1; /*para no tener en cuenta la terminacion \0 */
		}
	}

	num_attr=j;

	/*Añado los elementos que no son atributos, siguiendo el orden de aparicion*/
	for (i=0;i<this->num_childs;i++)
	{
		if (this->childs[i]->type!=NODE_Attribute)
		{
			if ( -1 == ( this->childs[i]->GetML( this->childs[i], &child_size, &(buf_list[j]) ) ) )
			{
				for (i=0; i<j;i++)
				{
					if (buf_list[i]) free (buf_list[i]);
				}
				if (buf_list) free (buf_list);
				return -1;
			}
			j++;
			total_size+=child_size-1;
		}
	}

	/*Ya tenemos el tamaño que debera tener el buffer de salida*/
	/*el tamaño del (nombre x2) + 5 o 3 para las etiquetas y num_attr es por los espacios entre atributos*/
	if (num_attr==j) 
		total_size+=strlen(this->name)+num_attr+4; 
	else 
		total_size+=(strlen(this->name)*2)+num_attr+6;

	*out_buf=(char *) malloc (total_size);
	if (*out_buf==NULL)
	{
		for (i=0; i<j;i++)
		{
			if (buf_list[i]) free (buf_list[i]);
		}
		if (buf_list) free (buf_list);
		return -1;
	}
	
	strcpy(*out_buf,"<");
	strcat(*out_buf,this->name);
	for (i=0;i<num_attr;i++)
	{
		strcat(*out_buf," ");
		strcat(*out_buf,buf_list[i]);
	}
	if (num_attr==j)
		strcat(*out_buf,"/>");
	else
		strcat(*out_buf,">");
	
	for (i;i<j;i++)
	{
		strcat(*out_buf,buf_list[i]);
	}
	if (num_attr!=j)
	{
		strcat(*out_buf,"</");
		strcat(*out_buf,this->name);
		strcat(*out_buf,">");
	}
	
	*buf_size=total_size;

	/*Hay que liberar la memoria utilizada en el proceso*/
	for (i=0; i<j;i++)
	{
		if (buf_list[i]) free (buf_list[i]);
	}
	if (buf_list) free (buf_list);

	return 0;
}

/*Sencilla inicializacion de un nodo tipo elemento*/
void element_init (p_node nodo)
{
	nodo->Constructor=element_Constructor;
	nodo->Destructor=node_Destructor;
	nodo->AddRef=node_AddRef;
	nodo->Release=node_Release;
	nodo->QueryInterface=element_QueryInterface;
	nodo->Copy=node_Clone;
	nodo->GetML=element_GetML;
	nodo->Constructor((p_com_obj)nodo);
}

/******************************************************************************
	************************* IMLElement *******************************

	Aqui comienza la interfaz IMLElement para objetos del tipo nodo_elemento
*******************************************************************************/

void mlelement_AddRef (IMLElement this)
{
	this->p_this->AddRef((p_com_obj)this->p_this);
}

void mlelement_Release (IMLElement this)
{
	this->p_this->Release((p_com_obj)this->p_this);
	 
	free (this);
}

int mlelement_QueryInterface (IMLElement this, int IID, void **p_IObj)
{
	return this->p_this->QueryInterface((p_com_obj)this->p_this,IID,p_IObj);
}

int mlelement_GetType (IMLElement this)
{
	return this->p_this->type;
}

const char *mlelement_GetName (IMLElement this)
{
	return this->p_this->name;
}

int mlelement_SetName (IMLElement this, char * name)
{
	strncpy(this->p_this->name,name,256);
	return 0;
}

int mlelement_GetNumChilds (IMLElement this)
{
	return this->p_this->num_childs;
}

int mlelement_GetSingleChild (IMLElement this, char * name, IMLNode *out, int index)
{
	return common_GetSingleChild (this->p_this, name, out, index);
}

int mlelement_AppendChild (IMLElement this, IMLNode child, IMLNode *ret)
{
	switch(child->p_this->type)
	{
	case NODE_Element:
	case NODE_Attribute:
	case NODE_Text:
	case NODE_Comment:
		break;

	default:
		return -2;/*tipo de nodo no permitido para colgar de un elemento*/
	}

	return common_AppendChild (this->p_this, child, ret);
}

int mlelement_DeleteChildByInterfaz (IMLElement this, IMLNode child)
{
	return common_DeleteChildByInterfaz (this->p_this, child);
}

int mlelement_DeleteChildByIndex (IMLElement this, int index)
{
	return common_DeleteChildByIndex (this->p_this, index);
}


int mlelement_GetAttribute (IMLElement this, char * name, IMLAttribute *out)
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

int mlelement_SetAttribute (IMLElement this, IMLAttribute in)
{
	int i;
	p_node tmp,*tmp1;

	for (i=0;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Attribute)&&(strcmp(this->p_this->childs[i]->name,in->p_this->name)==0))
		{
			if (this->p_this->childs[i]->size!= 0)
			{
				free (this->p_this->childs[i]->data);
			}
			this->p_this->childs[i]->size=in->p_this->size;
			this->p_this->childs[i]->data=malloc(this->p_this->childs[i]->size);
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
	free(this->p_this->childs);
	this->p_this->childs=tmp1;
	this->p_this->num_childs++;

	return i;
}

int mlelement_DeleteAttribute (IMLElement this, char *name)
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
			return 0;
		}
	}

	return -2;
}


/*Esto tendra que soportar XPath o utilizar trucos tipo index*/
int mlelement_GetSingleElement (IMLElement this, char * name, IMLElement *out, int index)
{
	int i=index;

	for (;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Element)&&(strcmp(this->p_this->childs[i]->name,name)==0))
		{
			*out=(IMLElement) malloc (sizeof(struct MLElement));
			if (*out==NULL) return -1;/*No hay memoria*/
			mlelement_init(*out,this->p_this->childs[i]);
			return i;
		}
	}

	return -2;
}

int mlelement_AppendElement (IMLElement this, IMLElement in, IMLElement *ret)
{
	int i;
	p_node tmp,*tmp1;


	tmp = in->p_this->Copy(in->p_this);
	if (tmp==NULL)
	{
		*ret=NULL;
		return -1;
	}

	tmp1 = (p_node *) malloc (sizeof(p_node)*(this->p_this->num_childs+1));

	if (tmp1==NULL) return -1; /*por si no hay memoria*/

	for (i=0;i<this->p_this->num_childs;i++)
	{
		tmp1[i]=this->p_this->childs[i];
	}

	tmp1[i]=tmp;
	tmp->AddRef((p_com_obj)tmp);
	free(this->p_this->childs);
	this->p_this->childs=tmp1;
	this->p_this->num_childs++;

	*ret=(IMLElement) malloc(sizeof(struct MLElement));
	if (*ret)
	{
		mlelement_init(*ret,tmp);
	}

	return i;
}

/*Esto tendra que soportar tambien XPath para hacer mas facil todo. Para otra version*/
int mlelement_DeleteElement (IMLElement this, IMLElement in)
{
	int i,j,k;
	p_node *tmp;

	for (i=0;i<this->p_this->num_childs;i++)
	{
		if ((this->p_this->childs[i]->type==NODE_Element)&&(strcmp(this->p_this->childs[i]->name,in->p_this->name)==0))
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

int mlelement_AppendText (IMLElement this, IMLText in, IMLText *ret)
{
	int i;
	p_node tmp,*tmp1;


	tmp = in->p_this->Copy(in->p_this);
	if (tmp==NULL)
	{
		*ret=NULL;
		return -1;
	}

	tmp1 = (p_node *) malloc (sizeof(p_node)*(this->p_this->num_childs+1));

	if (tmp1==NULL) /*por si no hay memoria*/
	{
		tmp->Release((p_com_obj)tmp);
		*ret=NULL;
		return -1;
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

	*ret=(IMLText) malloc(sizeof(struct MLText));
	if (*ret)
	{
		mltext_init(*ret,tmp);
	}

	return i;
}

int mlelement_AppendComment (IMLElement this, IMLComment in, IMLComment *ret)
{
	int i;
	p_node tmp,*tmp1;


	tmp = in->p_this->Copy(in->p_this);
	if (tmp==NULL)
	{
		*ret=NULL;
		return -1;
	}

	tmp1 = (p_node *) malloc (sizeof(p_node)*(this->p_this->num_childs+1));

	if (tmp1==NULL) /*por si no hay memoria*/
	{
		tmp->Release((p_com_obj)tmp);
		*ret=NULL;
		return -1;
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

	*ret=(IMLComment) malloc(sizeof(struct MLComment));
	if (*ret)
	{
		mlcomment_init(*ret,tmp);
	}

	return i;
}

int mlelement_GetML (IMLElement this, unsigned int *buffer_size, char **out_buffer)
{
	return this->p_this->GetML(this->p_this,buffer_size,out_buffer);
}


/*Funcion de inicializacion de interfaz*/
void mlelement_init (IMLElement interfaz, p_node objeto)
{
	interfaz->p_this = objeto;
	objeto->AddRef((p_com_obj)objeto);
	interfaz->AddRef = mlelement_AddRef;
	interfaz->Release = mlelement_Release;
	interfaz->QueryInterface = mlelement_QueryInterface;
	interfaz->GetType = mlelement_GetType;
	interfaz->GetName = mlelement_GetName;
	interfaz->SetName = mlelement_SetName;
	interfaz->GetNumChilds = mlelement_GetNumChilds;
	interfaz->GetSingleChild = mlelement_GetSingleChild;
	interfaz->AppendChild = mlelement_AppendChild;
	interfaz->DeleteChildByInterfaz = mlelement_DeleteChildByInterfaz;
	interfaz->DeleteChildByIndex = mlelement_DeleteChildByIndex;
	interfaz->GetAttribute = mlelement_GetAttribute;
	interfaz->SetAttribute = mlelement_SetAttribute;
	interfaz->DeleteAttribute = mlelement_DeleteAttribute;
	interfaz->GetSingleElement = mlelement_GetSingleElement;
	interfaz->AppendElement = mlelement_AppendElement;
	interfaz->DeleteElement = mlelement_DeleteElement;
	interfaz->AppendText = mlelement_AppendText;
	interfaz->AppendComment = mlelement_AppendComment;
	interfaz->GetML = mlelement_GetML;
}



