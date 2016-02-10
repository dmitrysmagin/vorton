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

#if !defined(DOCUMENT_H)
#define DOCUMENT_H

#include "ML_COM_Sim.h"
#include "ML_Node.h"
#include "ML_Element.h"
#include "ML_Process.h"
#include "ML_Attribute.h"
#include "ML_Text.h"
#include "ML_Comment.h"

struct document{
		unsigned ref;
		int type;

		void (*Constructor) (p_com_obj this);
		void (*Destructor) (p_com_obj this);

		void (*AddRef) (p_com_obj this);
		void (*Release) (p_com_obj this);
		int (*QueryInterface) (p_com_obj this, int IID, void **p_IObj);

		char file_name[256];
		int num_childs;
		p_node *childs;

		int (*GetML) (struct document *this, unsigned int *buf_size, char **out_buf);
} ;

typedef struct document *p_document;

void document_init (p_document documento);



struct MLDocument{

		p_document p_this;

		void (*AddRef) (struct MLDocument *this);
		void (*Release) (struct MLDocument *this);
		int (*QueryInterface) (struct MLDocument *this, int IID, void **p_IObj);

		int (*GetType) (struct MLDocument *this);

		const char * (*GetFileName) (struct MLDocument *this);
		int (*SetFileName) (struct MLDocument *this, char * file_name);
		
		int (*GetNumChilds) (struct MLDocument *this);
		int (*GetSingleChild) (struct MLDocument *this, char * name, IMLNode *out, int index); 
		/*int (*GetChild) (struct MLDocument *this, char * name, IMLNode *out); */
		int (*AppendChild) (struct MLDocument *this, IMLNode child, IMLNode *ret);
		int (*DeleteChildByInterfaz) (struct MLDocument *this, IMLNode child);
		int (*DeleteChildByIndex) (struct MLDocument *this, int index);

		int (*AppendText) (struct MLDocument *this, IMLText child, IMLText *ret);
		int (*AppendComment) (struct MLDocument *this, IMLComment child, IMLComment *ret);

		int (*GetSingleElement) (struct MLDocument *this, char * name, IMLElement *out, int index); 
		int (*AppendElement) (struct MLDocument *this, IMLElement child, IMLElement *ret);
		int (*DeleteElement) (struct MLDocument *this, IMLElement child);

		int (*GetSingleProcessInstruction) (struct MLDocument *this, char * name, IMLProcessInstruction *out, int index); 
		int (*AppendProcessInstruction) (struct MLDocument *this, IMLProcessInstruction child, IMLProcessInstruction *ret);
		int (*DeleteProcessInstruction) (struct MLDocument *this, IMLProcessInstruction child);

	/*Aqui ponemos los metodos para crear objetos derivados, como elementos y atributos*/
		int (*CreateElement) (struct MLDocument *this, int IID, IMLElement *interfaz);
		int (*CreateAttribute) (struct MLDocument *this, int IID, IMLAttribute *interfaz);
		int (*CreateText) (struct MLDocument *this, int IID, IMLText *interfaz);
		int (*CreateComment) (struct MLDocument *this, int IID, IMLComment *interfaz);
		int (*CreateProcessInstruction) (struct MLDocument *this, int IID, IMLProcessInstruction *interfaz);
		
		int (*GetML) (struct MLDocument *this, unsigned int *buffer_size, char **out_buf);
		int (*SetML) (struct MLDocument *this, unsigned int buffer_size, char *in_buf);
		int (*Save) (struct MLDocument *this);
		int (*SaveFile) (struct MLDocument *this, const char *name);
		int (*LoadFile) (struct MLDocument *this, const char *name);
};


typedef struct MLDocument *IMLDocument;

void mldocument_init (IMLDocument interfaz, p_document objeto);

int DocumentFromCOM (int IID, IMLDocument *interfaz); /*PARCHE*/

#endif /*if !defined(DOCUMENT_H)*/

