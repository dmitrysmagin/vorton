/*
 * Vorton - Highway Encounter SDL Remake Game.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even  the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
----------------
- Objeto Robot -
----------------
*/


/* Definiciones generales */
#include "filmat.h"

#define FRAME_ROBOT 8
#define FRAME_ROBOT2 3
#define FIL_MAXITER 8
#define FREC_TRY 32
#define FIL_MAXANIM 5

/* Rutinas de Comprobación de Márgenes de Pista Genérica */

/* Fecha Creación: 	04/09/2003	*/
/* Fecha URev:		05/09/2003	*/
/* Parámetros: 		
 *		x:	Coordenada x del objeto		
 *		y:	Coordenada x del objeto		
 *
 * COMENTARIOS: Sin Optimizar		
 ***************************************************************/


/* Rutina de Cálculo de trayectorias de objetos móviles */

/* Fecha Creación: 	04/09/2003	*/
/* Fecha URev:		22/10/2003	*/


/* Parámetros: 		
			tipo : 	Tipo de Trayectoria
			0 Rectilínea N-O
			1 Rectilínea O
			2 Rectilínea S-O               N
			3 Rectilínea S               O + E
			4 Rectilínea S-E	       S
			5 Rectilínea E
			6 Rectilínea N-E
			7 Rectilínea N
			8 Rectilinea Persecución			 
			9 Aleatoria
		       10 Cambio Trayectoria cada cierto tiempo aleatorio
		       11 Cambio de trayectoria cada cierto tiempo fijo	
			
*		x:	Coordenada x del objeto		
*		y:	Coordenada x del objeto		
*		vel:	Velocidad del objeto		
*
* COMENTARIOS:	No Optimizada	
*		Esta rutina tiene la limitación que las trayectorias con memoria (aleatoria) son comunes
*		para todos los objetos que posean este tipo, es decir, cuando cambia la try , cambia para todos
*		los objetos de ese tipo.
*		Una solución consiste en incluir los vectores de dirección a cada objeto móvil en su estructura
*		Un diseño hábil de niveles puede ocultar este efecto.
***************************************************************/


//static inline void trayectoria(int obj_id, int *tipo,int *x,int *y,int vel)
static inline void trayectoria(p_filmat_object this)
{  /* Begin trayectoria */

	/* Selección y Cálculo de la trayectoria */
	int vel = this->vel;
		
	switch(this->move_type)
		{
			case 0:
				this->x-=vel;
				this->y+=vel;
				break;
			case 1:
				this->x-=vel;
				break;
			case 2:
				this->x-=vel;
				this->y-=vel;
				break;
			case 3:
				this->y-=vel;
				break;
			case 4:
				this->x+=vel;
				this->y-=vel;
				break;
			case 5:
				this->x+=vel;
				break;
			case 6:
				this->x+=vel;
				this->y+=vel;
				break;
			case 7:		
				this->y+=vel;
				break;
			case 8:		

				if (filmat_hero.x<this->x)
				  {
				  	this->x-=vel;
				  }
				 else if (filmat_hero.x!=this->x)
				  {
					this->x+=vel;
				  }
				if (filmat_hero.y<this->y)
				  {
				  	this->y-=vel;
				  }
				 else if (filmat_hero.y!=this->y)
				  {
					this->y+=vel;
				  } 	 	
				
				break;
			case 9:
				this->iter--;
				if (!this->iter)
				{
					this->vec_x=getRand()%2;
					this->vec_y=getRand()%2;
					this->sign=getRand()%2;
					this->iter=FREC_TRY;
				}
				if (!this->sign) vel=-vel;
				if (this->vec_x) this->x+=vel;	
				if (this->vec_y) this->y+=vel;
				break;
			case 10:

	 			this->iter--;
	 			if (!this->iter)
	 				{
	 					this->iter=1+(getRand()%FIL_MAXITER);
	 					this->vec_x=getRand()&0x1;
					        this->vec_y=getRand()&0x1;
					        this->sign=getRand()&0x1;
						this->vel=1+(getRand()&0x3);
	 				}	 				
				if (!this->sign) vel=-vel;
				if (this->vec_x) this->x+=vel;	
				if (this->vec_y) this->y+=vel;
	 			break;			
			case 11:
	 			this->iter--;
	 			if (!this->iter)
	 				{
	 					this->iter=FIL_MAXITER/4;
	 					this->vec_x=getRand()%2;
					        this->vec_y=getRand()%2;
					        this->sign=getRand()%2;
	 				}			
				if (!this->sign) vel=-vel;
				if (this->vec_x) this->x+=vel;	
				if (this->vec_y) this->y+=vel;
				break;
			default:
				break;
		}
		
		/* Comprobación límites de la pista */
		  limXY(this);	/* Comprobar coordendas */
		
} /* End trayectoria */


/* Rutina de Gestión del Comportamiento de los objetos móviles de la clase robot*/

/* Fecha Creación: 	14/09/2003	*/
/* Fecha URev:		18/09/2003	*/

/* Parámetros: 				
*			*this ; Puntero al objeto sobre el que se va
*			a procesar su comportamiento
*
*	LISTA DE COMPORTAMIENTOS
*			O: Trayectoria sin cambios. El objeto ejecuta la trayectoria
			   sin ningún tipo de regla adicional
*			1: OBJ_PROCESS: El objeto cambia el sentido de la trayectoria al alcanzar
			   los límites de la pista
*			2: OBJ_COLLISION: El objeto ejecuta una modificacion de acuerdo a su tipo
*
* COMENTARIOS:	No Optimizada	
*		Esta rutina sirve, para en base a trayectorias simples, construir
*		movimiento cíclicos.

***************************************************************/

static inline void Comportamiento(p_filmat_object this, int modifier)

{ /* Begin Comportamiento */
	switch(modifier)
	{ /* Begin switch->modifier */
		case 0:	
		 	break;
		case 1: /* obj_process */

			if (((this->x == FIL_MAX_X)||(this->x == FIL_MIN_X)) && ((this->move_type==1)||(this->move_type==5))) this->vel = -this->vel;
			if (((this->y == FIL_MAX_Y)||(this->y == FIL_MIN_Y)) && ((this->move_type==3)||(this->move_type==7))) this->vel = -this->vel;
			/* Faltan por implementar los rebotes en las 4 esquinas del mapa*/
			/* Una alternativa puede ser incluir en los 4 puntos un objeto fijo */
 
 			break;
		case 2: /* obj_collision */

			switch (this->obj_type)
			{ /* Begin switch tipo_obj */
			
				case FIL_TYPE_CYCLOP:
					this->move_type=9+(getRand()%3); /* Cambio aleatorio dirección*/
					break;
				case FIL_TYPE_DISC:
					/* Kamikace */
					break;
				case FIL_TYPE_ROBOT:
					this->vel = -this->vel;    /* Rebote al chocar*/
	 				break;
				case FIL_TYPE_MOUTH:
					this->vel = -this->vel;    /* Rebote al chocar*/
	 				break;
				case FIL_TYPE_MINE:
					this->vel = -this->vel;				
	 				break;
			 	default:
 					break;
			} /* Begin switch tipo_obj */
			
 		default:
 			break;
	} /* End switch this->modifier */ 	

} /* End Comportamiento */



/* Rutina de Proceso de objetos móviles */

/* Fecha Creación: 	02/09/2003	*/
/* Fecha URev:		08/09/2003	*/

/***************************************************************/

void filmat_robot_process(p_filmat_object this)

{  /* Begin obj_mov_process */
	
	if (!(frame&FRAME_ROBOT2))
	{
		this->rot++;
		if (this->rot>=8)
			this->rot=0;
		trayectoria(this);
		Comportamiento(this,1);
	}	
	
	
} /* End obj_mov_process */




/* Rutina de Proceso de explosiones objetos  */

/* Fecha Creación: 	04/10/2003	*/
/* Fecha URev:		21/10/2003	*/

/* Comentarios:		Como ahora los gráficos sólo tienen animación, realmente, la imagen
			que se escoge es la primera de cada archivo de explosiones existentes
			se ha tenido que elegir una animación en 11 fases para que se aprecie algo
			con 3 o menos animaciones ni se aprecia el efecto.
			La variable disparo es provisional
***************************************************************/

void filmat_expl_process(p_filmat_object this)
{  /* Begin expl_process */
	if (!(frame%3))
		this->rot++;	/* Aumentamos la fase de animación */
	if (this->rot==FIL_MAXANIM)
	{
		 this->rot=0;		    /* Limpiar valores temporales */
		 free_obj(this->position);  /* Eliminamos el objeto de la pantalla al terminar la explosión */
		 switch(this->obj_type)
		 {
		 	case FIL_TYPE_MINE:
		 		score+=15;		    /* Puntación de prueba */
		 	case FIL_TYPE_CYCLOP:
		 		score+=22;
		 	default:
		 		score+=33;
		 }		
	}	 

} /* End expl_process */





void filmat_robot_collision(p_filmat_object this, p_filmat_object other)
{
//	other->push_x+=this->x - this->back_x;
//	other->push_y+=this->y - this->back_y;
	if (other->obj_type!=FIL_TYPE_LASER &&
	    other->obj_type!=FIL_TYPE_PYRAMID &&
	    other->obj_type!=FIL_TYPE_NULL)
	{
		if (this->x != this->back_x && this->y != this->back_y)
		{
			this->x=this->back_x;
			this->y=this->back_y;
			Comportamiento(this,2);
		}
		else
		{
			Comportamiento(this,2);
			Comportamiento(this,1);
		}
	}
}



/* Rutina de Proceso de explosiones objetos  */

/* Fecha Creación: 	--/--/2003	*/
/* Fecha URev:		26/10/2003	*/

/* Comentarios:		
***************************************************************/



void filmat_robot_init(p_filmat_object this)
{
	switch (this->obj_type)
	{
		case FIL_TYPE_MINE:
			this->img=FIL_IMG_MINE_0;
			break;
		case FIL_TYPE_CYCLOP:
			this->img=FIL_IMG_CYCLOP_0;
			break;
		case FIL_TYPE_MOUTH:
			this->img=FIL_IMG_MOUTH_0;
			break;
		case FIL_TYPE_DISC:
			this->img=FIL_IMG_DISC_0;
			break;
		default:
			this->img=FIL_IMG_ROBOT_0;
	}		
	this->process=(void *)&filmat_robot_process;
	this->collision=(void *)&filmat_robot_collision;
}


