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

#include "ML_leng_process.h"

/*
   Descripcion: comprueba si un caracter es o no reservado. 
   Datos de entrada: c.
*/

int is_reserv (char c)
{
	/*Puede que estos varien en futuras versiones*/
	if ( (c<0x30) || ((c>0x39)&&(c<0x41)) || ((c>0x5a)&&(c<0x61)&&(c!=0x5f)) || ((c>0x7a)&&(c<0x80)) )
		return 1;
	else 
		return 0;
}


/*
   Descripcion: salta todos los espacios y saltos de linea. 
   Datos de entrada: buf, size e index.
   Datos de salida: old_index
*/

int salta_espacio (char *buf, int size, int *index, int *old_index)
{
	*old_index=*index;

	 while ((*index<size)&&((buf[*index]==' ')||(buf[*index]=='\n'))) {(*index)++;}

	if (*index==size)
		return -1;
	else
		return 0;
}

/*
   Descripcion: salta todos los caracteres hasta encontrar un signo de menor. 
   Datos de entrada: buf, size e index.
   Datos de salida: old_index
*/

int busca_menor (char *buf, int size, int *index, int *old_index)
{
	*old_index=*index;

	while (*index<size)
	{
		if (buf[*index]=='<') break;
		(*index)++;
	}

	if (*index==size)
		return -1;
	else
		return 0;
}

/*
   Descripcion: salta todos los caracteres hasta encontrar un signo de mayor. 
   Datos de entrada: buf, size, index y nom_busq
   Datos de salida: old_index
*/

int busca_etiqueta_fin (char *buf, int size, int *index, int *old_index, char *nom_busq)
{
	int idx2,res=0;

	*old_index=*index;

	if (buf[*index]=='<')
	{
		if ((*index+1<size)&&(buf[*index+1]=='/'))
		{
			*index+=2;
		}
		else return -1;
	}
	else return -3;

	if (salta_espacio(buf, size, index, &idx2)==-1) return -1;

	switch (coger_nombre(buf, size, index, &idx2))
	{
	case -1:
		return -1;
	case -2:
	case 0:
		if (buf[*index]!='>') return -2;
		if (strncmp(&buf[idx2],nom_busq,*index-idx2)==0) res=1;
		break;
	}
		
	if (salta_espacio(buf, size, index, &idx2)==-1) return -1;

	if (buf[*index]=='>')
		if (res) {(*index)++; return 0;} else {(*index)++; return 1;}
	else
		if (res) return 2; else return -4;

}

/*
   Descripcion: recoge todos los caracteres validos para formar un nombre (* - reservados - ' ' - '\n'). 
   Datos de entrada: buf, size e index.
   Datos de salida: old_index
*/

int coger_nombre (char *buf, int size, int *index, int *old_index)
{
	*old_index=*index;

	 while (*index<size)
	{
		if ((buf[*index]==' ') || (buf[*index]=='\n')) return 0;
		if (is_reserv(buf[*index]) == 1) return -2;

		(*index)++;
	}

	return -1;
}


/*
   Descripcion: recoge los caracteres que determinan el valor de un atributo. 
   Datos de entrada: buf, size e index.
   Datos de salida: old_index
*/

int coger_atr_valor (char *buf, int size, int *index, int *old_index)
{
	if (*index>=size) return -1;

	if (buf[*index]!='"') return -3; else (*index)++; 

	*old_index=*index;

	 while (*index<size) 
	{
		if (buf[*index]=='"')
		{
			if (buf[*index-1]!='\\') 
			{
				(*index)++; 
				return 0;
			}
		}
		/*if ( (is_reserv(buf[*index]) != 0) ) return -2;*/

		(*index)++;
	}

	return -1;
}



/*funcion para saltar una palabra que utiliza el alfabeto dado*/
int salta_palabra_by_alf ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *alfabeto)
{
	if (strlen(alfabeto)==0) return -2;

	*old_index=*i;
	 while ((*i<*size)&&(strchr(alfabeto,buf[*i])==NULL)) {*i++;}

	if (*i==*size)
		return 0;
	else
		return 1;
}

/*funcion para buscar una palabra que utiliza el alfabeto dado*/
int busca_palabra_by_alf ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *alfabeto)
{
	if (strlen(alfabeto)==0) return -2;

	*old_index=*i;
	 while ((*i<*size)&&(strchr(alfabeto,buf[*i])!=NULL)) {(*i)++;}


	if (*i==*size)
		return 0;
	else
		return 1;
}

/*funcion para buscar una palabra que utiliza el alfabeto definido por su letra menor y mayor, se pueden excluir valores*/
/*es mas eficiente que la anterior en terminos generales*/
int busca_palabra_by_alf_limits ( FILE *fichero, char *buf, int *i, int *size, int *old_index, int alf_lo, int alf_hi, char *menos, char *mas)
{
	if (alf_lo>alf_hi) return -2;
	
	*old_index=*i;
	while ( (*i<*size) && (strchr(menos,buf[*i])==NULL) && ( ( (buf[*i]<=alf_hi) && (buf[*i]>=alf_lo) ) || ( strchr(mas,buf[*i]) != NULL ) ) ) {(*i)++;}

	if (*i==*size)
		return 0;
	else
		return 1;
}

/*devuelve error en forma de negativos, llega al final devuelve 0 y si encuantra un limite devuelve que limite es*/
int busca_limite ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *limite)
{
	int j,k,t,s;
	int fin=0, n_lim=0;
	char **lim;

	*old_index=*i;

	/*buscamos en la cadena limite los posibles fines de texto*/
	k=1;
	t=strlen(limite);
	if (t==0) return -2;


	for (j=0;j<t;j++) /*cuantos limites hay*/
	{
		if (limite[j] == ',')
		{
			if (j!=0)
			{
				if ((j+1 < t) && (limite[j+1]==',')) continue;
				else if (j+1!=t) k++;
			}
			else if (j+1==t) k=0;
		}
	}
	
	if (k==0) return -2;

	lim=(char **) malloc (sizeof(char *) * k);
	if (lim==NULL) return -1;

	for (k=s=j=0;j<t;j++) /*copiamos los limites en cadenas separadas*/
	{
		if (limite[j] == ',')
		{
			if (j!=0)
			{
				if ((j+1 < t) && (limite[j+1]==',')) continue;
				else if (j!=s)
					{
						lim[k] = (char *)malloc(sizeof(char)*(j-s));
						if (lim[k]==NULL) {for (s=0;s<k;s++) {free(lim[s]); free (lim); return -1;}}
						strncpy(lim[k++],&limite[s],j-s);
					}
			}
			s=j+1;
		}
	}

	if (s!=t);
	{
		lim[k] = (char *)malloc(sizeof(char)*(t-s));
		if (lim[k]==NULL) {for (s=0;s<k;s++) {free(lim[s]); free (lim); return -1;}}
		strncpy(lim[k++],&limite[s],t-s);
	}
	
	/*Buscamos en el texto hasta encontrar un limite*/
	while ( ((*i)<(*size)) && !fin)
	{
		for (s=0;s<k;s++)
		{
			n_lim=0;
			if (buf[*i]==*(lim[s]))
			{
				/*comprobar el resto del limite*/
				t=1;
				while ( ((*i+t)<(*size)) && (*(lim[s]+t) != '\0') )
				{
					if (buf[*i+t] != *(lim[s]+t)) {n_lim=0; break;}
					t++;
				}
				if (*(lim[s]+t) == '\0') n_lim=1;
			}
			if (n_lim) {fin=1; break;}
		}
		if (!fin) (*i)++;
	}
	
	if (fin) return s+1;
	else return 0;
}

int AnalizaT(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLText *tmp)
{
	int old_index;

	busca_menor ( buf, *size, i, &old_index);

	if (*i==old_index) return 1;

	if (doc->CreateText(doc,IID_IMLText,tmp)!=0) return -2;

	(*tmp)->p_this->data=malloc(*i-old_index);
	if ((*tmp)->p_this->data==NULL) {(*tmp)->Release(*tmp); return -1;}
	(*tmp)->p_this->size=(*i-old_index);
	memcpy((*tmp)->p_this->data,&buf[old_index],*i-old_index);

	return 0;
}

int AnalizaC(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLComment *tmp)
{
	int old_index;

	/*comprobamos el inicio de Comentario*/
	if (!((buf[*i]=='<')&&(buf[*i+1]=='!')&&(buf[*i+2]=='-')&&(buf[*i+3]=='-'))) return -3; /*habria que indicar el error*/

	*i+=3;

	old_index=*i;

	while (*i<*size)
	{
		if ((buf[*i]=='>')&&(buf[*i-1]=='-')&&(buf[*i-2]=='-')) {(*i)++; break;}
		(*i)++;
	}

	if (*i==*size) return -3;

	if (*i==old_index) return 0;

	if (doc->CreateComment(doc,IID_IMLComment,tmp)!=0) return -2;

	(*tmp)->p_this->data=malloc(*i-old_index);
	if ((*tmp)->p_this->data==NULL) {(*tmp)->Release(*tmp); return -1;}
	(*tmp)->p_this->size=(*i-old_index);
	memcpy((*tmp)->p_this->data,&buf[old_index],*i-old_index);

	return 1;
}

int AnalizaPI(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLProcessInstruction *tmp)
{
	IMLAttribute atr=NULL;
	int old_index;

	/*comprobamos el inicio de PI*/
	if (!((buf[*i]=='<')&&(buf[*i+1]=='?'))) return -3; /*habria que indicar el error*/

	*i+=2;
	
	if (salta_espacio(buf, *size, i, &old_index)==-1) /*Se acabo el buffer. Aqui se podria recargar, pero por ahora no*/
	{
		return -2;
	}

	switch (coger_nombre(buf, *size, i, &old_index))
	{
	case -1:
		return -2;

	case 0:
		break;

	case -2:
		if ((buf[*i]=='?')&&(buf[*i+1]=='>'))
			break;
		else
			return -4;
	}

	if (doc->CreateProcessInstruction(doc,IID_IMLProcessInstruction,tmp)!=0) return -5;

	if (*i-old_index>255) {(*tmp)->Release(*tmp); return -1;}
	else 
	{
		memcpy((*tmp)->p_this->name,&buf[old_index],*i-old_index);
		(*tmp)->p_this->name[*i-old_index]='\0';
	}

	/*ahora hay que buscar atributos o el final*/
	while (*i<*size) 
	{
		if (salta_espacio(buf, *size, i, &old_index)==-1)
		{
			return 1;
		}
	
		if (buf[*i]=='?')
		{
			if (*i+1>=*size) {if (atr) atr->Release(atr); return 0; }
			else if (buf[*i+1]=='>') {if (atr) atr->Release(atr); *i+=2; return 0; }
			else {if (atr) atr->Release(atr); return 0; }
		}
		else if (is_reserv(buf[*i])) 
			{/*Si encuentra un caracter reservado aqui lo considera un error, destruye la PI y devuelve el control*/
				if (atr) atr->Release(atr); 
				if (*tmp) (*tmp)->Release(*tmp); 
				return -6;
			}

		switch (coger_nombre(buf, *size, i, &old_index))
		{
		case -1:
			return 0;
	
		case 0:
		case -2:
			if (doc->CreateAttribute(doc,IID_IMLAttribute,&atr)!=0)
			{
				return 0;
			}

			if (*i-old_index>255) 
			{
				memcpy(atr->p_this->name,&buf[old_index],255);
				atr->p_this->name[255]='\0';
			}
			else 
			{
				memcpy(atr->p_this->name,&buf[old_index],*i-old_index);
				atr->p_this->name[*i-old_index]='\0';
			}
			break;
		}

		if (salta_espacio(buf, *size, i, &old_index)==-1)
		{
			if (atr) atr->Release(atr);
			return 0;
		}

		if (buf[*i] == '=')
		{
			
			(*i)++;
	
			if (salta_espacio(buf, *size, i, &old_index)==-1)
			{
				if (atr) atr->Release(atr);
				return 0;
			}
	
			switch (coger_atr_valor(buf, *size, i, &old_index))
			{
			case -1:
				return 0;
		
			case 0:
				atr->p_this->data=malloc(*i-old_index-1);
				atr->p_this->size=(*i-old_index-1);
				memcpy(atr->p_this->data,&buf[old_index],*i-old_index-1);
				break;
		
			case -3: /*Aqui deberia dar error, pero por ahora dejaremos que sea flexible en este aspecto*/
				break;
			}
		}

		/*Añadimos el atributo si lo hemos encontramos*/
		if ((*tmp)->SetAttribute(*tmp,atr)==-1) 
		{
			if (atr) atr->Release(atr);
			return 0;
		}

		if (atr) atr->Release(atr);
		atr=NULL;
	}
	
	/*Se acabo el buffer, mientras no lo recargemos aqui devolveremos el control.*/
	if (atr) atr->Release(atr);
	return 0;	
}

int AnalizaE(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLElement *tmp)
{
	IMLElement pe=NULL,ppe=NULL;
	IMLText pt=NULL,ppt=NULL;
	IMLAttribute atr=NULL;
	int old_index;

	/*comprobamos el inicio del Ele*/
	if (buf[*i]!='<') return -3; /*habria que indicar el error*/

	(*i)++;
	
	if (salta_espacio(buf, *size, i, &old_index)==-1) /*Se acabo el buffer. Aqui se podria recargar, pero por ahora no*/
	{
		return -2;
	}

	switch (coger_nombre(buf, *size, i, &old_index))
	{
	case -1:
		return -2;

	case 0:
		if (*i-old_index<1) return -2;
		break;

	case -2:
		if ( ((buf[*i]=='/')&&(buf[*i+1]=='>')) || (buf[*i]=='>') )
			break;
		else
			return -4;
	}

	if (doc->CreateElement(doc,IID_IMLElement,tmp)!=0) return -5;

	if (*i-old_index>255) {(*tmp)->Release(*tmp); return -1;}
	else 
	{
		memcpy((*tmp)->p_this->name,&buf[old_index],*i-old_index);
		(*tmp)->p_this->name[*i-old_index]='\0';
	}

	/*ahora hay que buscar atributos o el final de la etiqueta*/
	while (*i<*size) 
	{
		if (salta_espacio(buf, *size, i, &old_index)==-1)
		{
			return 0;
		}
	
		if (buf[*i]=='/') /*posibles finales de elemento, para elementos vacios*/
		{
			if (*i+1>=*size) {if (atr) atr->Release(atr); return 0; }
			else if (buf[*i+1]=='>') {if (atr) atr->Release(atr); (*i)+=2; return 0; }
			else {if (atr) atr->Release(atr); (*i)++; return 0; } /*por ahora dejare que pase*/
		}

		if (buf[*i]!='>') /*se admite todo menos fin de buffer y falta de memoria*/
		{
			if (is_reserv(buf[*i])) 
			{/*Si encuentra un caracter reservado aqui lo considera un error, destruye el Ele y devuelve el control*/
				if (atr) atr->Release(atr); 
				if (tmp) (*tmp)->Release(*tmp); 
				return -6;
			}
	
			switch (coger_nombre(buf, *size, i, &old_index))
			{
			case -1:
				return 0;
		
			case 0:
			case -2:
				if (doc->CreateAttribute(doc,IID_IMLAttribute,&atr)!=0)
				{
					return 0;
				}
	
				if (*i-old_index>255) 
				{
					memcpy(atr->p_this->name,&buf[old_index],255);
					atr->p_this->name[255]='\0';
				}
				else 
				{
					memcpy(atr->p_this->name,&buf[old_index],*i-old_index);
					atr->p_this->name[*i-old_index]='\0';
				}
				break;
			}
	
			if (salta_espacio(buf, *size, i, &old_index)==-1)
			{
				if (atr) atr->Release(atr);
				return 0;
			}
	
			if (buf[*i] == '=')
			{
				
				(*i)++;
		
				if (salta_espacio(buf, *size, i, &old_index)==-1)
				{
					if (atr) atr->Release(atr);
					return 0;
				}
		
				switch (coger_atr_valor(buf, *size, i, &old_index))
				{
				case -1:
					return 0;
			
				case 0:
					atr->p_this->data=malloc(*i-old_index-1);
					atr->p_this->size=(*i-old_index-1);
					memcpy(atr->p_this->data,&buf[old_index],*i-old_index-1);
					break;
			
				case -3: /*Aqui deberia dar error, pero por ahora dejaremos que sea flexible en este aspecto*/
					break;
				}
			}
	
			/*Añadimos el atributo si lo hemos encontramos*/
			if ((*tmp)->SetAttribute(*tmp,atr)==-1) 
			{
				if (atr) atr->Release(atr);
				return 0;
			}
	
			if (atr) atr->Release(atr);
			atr=NULL;

		}
		else 
		{
			(*i)++;
			break; /*si encuentra el final de la etiqueta '>' salimos de este bucle*/
		}
	}
	
	while (*i<*size-1)
	{
		switch(buf[*i])
		{
		case '<':
			if (buf[*i+1]=='/')
			{/*etiqueta de fin de elemento*/
				switch (busca_etiqueta_fin(buf, *size, i, &old_index, (*tmp)->p_this->name))
				{
				case 0:/*hay una etiqueta de fin y pertenece a nuestro elemento*/
					return 0;
				case 2:/*no es un fin de etiqueta valido pero aparece el nombre que buscamos*/
					return 0;
				
				case 1: /*hay una etiqueta de fin, pero no es de nuestro elemento*/
					break;
				case -4: /*no es un fin de etiqueta valido y no aparece el nombre que buscamos*/
					break;
				}
			}
			else/*subelemento*/
			{
				if ( (AnalizaE(doc,fichero,buf,i,size, &pe)) == 0)
				{
					/*añadimos pe al elemento tmp*/
					if ((*tmp)->AppendElement(*tmp,pe,&ppe)!=-1)
					{
						if (ppe) {ppe->Release(ppe); ppe=NULL;}
					}
					if (pe) {pe->Release(pe); pe=NULL;}
				}
			}
			break;

		default:
			if ( (AnalizaT(doc,fichero,buf,i,size, &pt)) == 0)
			{
				/*añadimos pt al elemento*/
				if ((*tmp)->AppendText(*tmp,pt,&ppt)!=-1)
				{
					if (ppt) {ppt->Release(ppt); ppt=NULL;}
				}
				if (pt) {pt->Release(pt); pt=NULL;}
			}
			break;
		}
	}

	/*Se acabo el buffer, mientras no lo recargemos aqui devolveremos el control.*/
	if (atr) atr->Release(atr);
	if (pt) pt->Release(pt);
	if (pe) pe->Release(pe);
	return 0;	
}

