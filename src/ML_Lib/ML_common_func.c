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

#include "ML_common_func.h"

int common_GetSingleChild (p_node this, char * name, IMLNode *out, int index)
{
	int i=index;

	for (i;i<this->num_childs;i++)
	{
		if (strcmp(this->childs[i]->name,name)==0)
		{
			*out=(IMLNode) malloc (sizeof(struct MLNode));
			mlnode_init(*out,this->childs[i]);
			return i;
		}
	}

	return -1;
}

int common_AppendChild (p_node this, IMLNode child, IMLNode *ret)
{
	int i=0;
	p_node tmp,*tmp1;

	tmp = child->p_this->Copy(child->p_this);
	if (tmp==NULL)
	{
		*ret=NULL;
		return -1;
	}

	tmp1 = (p_node *) malloc (sizeof(p_node)*(this->num_childs+1));

	if (tmp1==NULL) return -1; /*por si no hay memoria*/

	for (i;i<this->num_childs;i++)
	{
		tmp1[i]=this->childs[i];
	}

	tmp1[i]=tmp;
	tmp->AddRef((p_com_obj)tmp);
	free(this->childs);
	this->childs=tmp1;
	this->num_childs++;

	*ret=(IMLNode) malloc(sizeof(struct MLNode));
	if (*ret)
	{
		mlnode_init(*ret,tmp);
	}

	return i;
}

int common_DeleteChildByInterfaz (p_node this, IMLNode child)
{
	int i,j,index;
	p_node *tmp;

	for (index=0;index<this->num_childs;i++)
	{
		if (this->childs[index]==child->p_this)
			break;
	}

	if (index==this->num_childs)
		return 0;
	else
	{
		/*liberamos el enlace del objeto y si es el ultimo este se autodestruira*/
		this->childs[index]->Release((p_com_obj)this->childs[index]);

		/*Creamos la nueva lista de punteros de objeto*/
		tmp = (p_node *) malloc (sizeof(p_node)*(this->num_childs-1));


		if (tmp==NULL) return -1; /*por si no hay memoria*/

		j=0;
		for (i=0;i<this->num_childs;i++)
		{
			if (i!=index)
				tmp[j++]=this->childs[i];
		}

		free (this->childs);/*borramos la lista vieja*/
		this->childs=tmp;/*ponemos la lista nueva*/
		this->num_childs--;
		return 1;
	}
}


int common_DeleteChildByIndex (p_node this, int index)
{
	int i,j;
	p_node *tmp;

	if ((index>=this->num_childs)||(index<0))
		return 0;
	else
	{
		/*liberamos el enlace del objeto y si es el ultimo este se autodestruira*/
		this->childs[index]->Release((p_com_obj)this->childs[index]);

		/*Creamos la nueva lista de punteros de objeto*/
		tmp = (p_node *) malloc (sizeof(p_node)*(this->num_childs-1));


		if (tmp==NULL) return -1; /*por si no hay memoria*/

		j=0;
		for (i=0;i<this->num_childs;i++)
		{
			if (i!=index)
				tmp[j++]=this->childs[i];
		}

		free (this->childs);/*borramos la lista vieja*/
		this->childs=tmp;/*ponemos la lista nueva*/
		this->num_childs--;
		return 1;
	}
}


int common_GetData (p_node this, int *buffer_size, void **out_buffer)
{
	if (this->size)
	{
		*buffer_size=this->size;
		*out_buffer=malloc(this->size+1);
		if (*out_buffer==NULL) return -1; /*no hay memoria*/
		memcpy(*out_buffer,this->data,this->size);
		((char *)*out_buffer)[this->size]='\0'; /*poco ortodoxo pero efectivo ;P*/
		return 0;
	}
	return -2;
}


int common_SetData (p_node this, int buffer_size, void *in_buffer)
{
	if (this->size)
	{
		free(this->data);
	}

	this->size=buffer_size;
	this->data=malloc(this->size);
	if (this->data==NULL) {this->size=0; return -1;} /*no hay memoria*/
	memcpy(this->data,in_buffer,buffer_size);
	return 0;
}
