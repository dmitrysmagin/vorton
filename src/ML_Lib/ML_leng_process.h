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

#include "ML_Document.h"
#include "ML_Element.h"
#include "ML_Process.h"
#include "ML_Text.h"

/*leng_process.h funciones para facilitar el procesamiento de los ficheros*/

int is_reserv (char c);
int salta_espacio (char *buf, int size, int *index, int *old_index);
int busca_menor (char *buf, int size, int *index, int *old_index);
int busca_etiqueta_fin (char *buf, int size, int *index, int *old_index, char *nom_busq);
int coger_nombre (char *buf, int size, int *index, int *old_index);
int coger_atr_valor (char *buf, int size, int *index, int *old_index);


/*funcion para saltar una palabra que utiliza el alfabeto dado*/
int salta_palabra_by_alf ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *alfabeto);

/*funcion para buscar una palabra que utiliza el alfabeto dado*/
int busca_palabra_by_alf ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *alfabeto);

/*funcion para buscar una palabra que utiliza el alfabeto definido por su letra menor y mayor, se pueden excluir valores*/
/*es mas eficiente que la anterior en terminos generales*/
int busca_palabra_by_alf_limits ( FILE *fichero, char *buf, int *i, int *size, int *old_index, int alf_lo, int alf_hi, char *menos, char *mas);

int busca_limite ( FILE *fichero, char *buf, int *i, int *size, int *old_index, char *limite);


int AnalizaE(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLElement *tmp);
int AnalizaPI(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLProcessInstruction *tmp);
int AnalizaT(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLText *tmp);
int AnalizaC(IMLDocument doc, FILE *fichero, char *buf, int *i, int *size, IMLComment *tmp);
