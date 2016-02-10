/*
 * Vorton - Highway Encounter SDL Remake Game.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef FILMAT_H
#define FILMAT_H

#include "vorton.h"

typedef void* _p_filmat_object;

typedef struct {
	int x, y;		/* Coordenadas Cartesianas */
	int position;           /* Posicion en la lista */
	int direction;          /*La uso de momento pq si meto rot, la imagen del laser cambia */
	int back_x, back_y;	/* Coordenadas anteriores al proceso */
	int push_x, push_y; 	/* Empuje */ 
	int draw_x, draw_y; 	/* Coordenadas de visualizacion */ 
	int size_x, size_y;	/* longitud x e y del objecto */
	int rot;		/* Rotacion sobre su eje */
	int vel;		/* Velocidad */
	int state;		/* Estado */
	int img;		/* Imagen base asociada */
	int move_type;		/* Tipo de movimiento */
	int obj_type;		/* Tipo de Robot/objeto */
	int iter;
	int vec_x;		/* movimiento en trayectorias aleatorias     */
	int vec_y;
	int sign;
	void (*process)(_p_filmat_object this);		/* Proceso */
	void (*collision)(_p_filmat_object this, _p_filmat_object other);	/* Choque */
	_p_filmat_object back;	/* Anterior objecto para visualizar */
	_p_filmat_object next;	/* Siguiente objeto para visualizar */
}filmat_object;

typedef filmat_object * p_filmat_object;

#define FIL_DATA_COMMON DATA_PREFIX "filmat" FILE_SEPARATOR

/* Enumeracion para la carga de imagenes */
enum {
	FIL_IMG_HERO_0_0,
	FIL_IMG_HERO_1_0,
	FIL_IMG_HERO_2_0,
	FIL_IMG_HERO_3_0,
	FIL_IMG_HERO_4_0,
	FIL_IMG_HERO_5_0,
	FIL_IMG_HERO_6_0,
	FIL_IMG_HERO_7_0,
	FIL_IMG_HERO_0_1,
	FIL_IMG_HERO_1_1,
	FIL_IMG_HERO_2_1,
	FIL_IMG_HERO_3_1,
	FIL_IMG_HERO_4_1,
	FIL_IMG_HERO_5_1,
	FIL_IMG_HERO_6_1,
	FIL_IMG_HERO_7_1,
	FIL_IMG_HERO_0_2,
	FIL_IMG_HERO_1_2,
	FIL_IMG_HERO_2_2,
	FIL_IMG_HERO_3_2,
	FIL_IMG_HERO_4_2,
	FIL_IMG_HERO_5_2,
	FIL_IMG_HERO_6_2,
	FIL_IMG_HERO_7_2,
	FIL_IMG_HERO_HIT_0,
	FIL_IMG_HERO_HIT_1,
	FIL_IMG_HERO_HIT_2,
	FIL_IMG_HERO_HIT_3,
	FIL_IMG_DUMMY_0,
	FIL_IMG_DUMMY_1,
	FIL_IMG_DUMMY_2,
	FIL_IMG_LASER_0,
	FIL_IMG_LASER_1,
	FIL_IMG_LASER_2,
	FIL_IMG_LASER_3,
	FIL_IMG_EXPL0,	
	FIL_IMG_EXPL1,
	FIL_IMG_EXPL2,
	FIL_IMG_EXPL3,
	FIL_IMG_EXPL4,
	FIL_IMG_ENERGY_0,
	FIL_IMG_ENERGY_1,
	FIL_IMG_ENERGY_2,
	FIL_IMG_ENERGY_3,
	FIL_IMG_ENERGY_4,
	FIL_IMG_ENERGY_5,
	FIL_IMG_ENERGY_6,
	FIL_IMG_ENERGY_7,
	FIL_IMG_ENERGY_8,
	FIL_IMG_PYRAMID0,
	FIL_IMG_PYRAMID1,
	FIL_IMG_PYRAMID2,
	FIL_IMG_SHIP,
	FIL_IMG_BLOCK_0,
	FIL_IMG_ROBOT_0,
	FIL_IMG_ROBOT_1,
	FIL_IMG_ROBOT_2,
	FIL_IMG_ROBOT_3,
	FIL_IMG_ROBOT_4,
	FIL_IMG_ROBOT_5,
	FIL_IMG_ROBOT_6,
	FIL_IMG_ROBOT_7,
	FIL_IMG_CYCLOP_0,
	FIL_IMG_CYCLOP_1,
	FIL_IMG_CYCLOP_2,
	FIL_IMG_CYCLOP_3,
	FIL_IMG_CYCLOP_4,
	FIL_IMG_CYCLOP_5,
	FIL_IMG_CYCLOP_6,
	FIL_IMG_CYCLOP_7,
	FIL_IMG_MINE_0,
	FIL_IMG_MINE_1,
	FIL_IMG_MINE_2,
	FIL_IMG_MINE_3,
	FIL_IMG_MINE_4,
	FIL_IMG_MINE_5,
	FIL_IMG_MINE_6,
	FIL_IMG_MINE_7,
	FIL_IMG_TILE,
	FIL_IMG_BRICK_0,
	FIL_IMG_BRICK_1,
	FIL_IMG_BRICK_2,
	FIL_IMG_BRICK_3,
	FIL_IMG_FIRE_0,
	FIL_IMG_FIRE_1,
	FIL_IMG_FIRE_2,
	FIL_IMG_FIRE_3,
	FIL_IMG_MOUTH_0,
	FIL_IMG_MOUTH_1,
	FIL_IMG_MOUTH_2,
	FIL_IMG_MOUTH_3,
	FIL_IMG_MOUTH_4,
	FIL_IMG_MOUTH_5,
	FIL_IMG_MOUTH_6,
	FIL_IMG_MOUTH_7,
	FIL_IMG_DISC_0,
	FIL_IMG_DISC_1,
	FIL_IMG_DISC_2,
	FIL_IMG_DISC_3,
	FIL_IMG_DISC_4,
	FIL_IMG_DISC_5,
	FIL_IMG_DISC_6,
	FIL_IMG_DISC_7,
	FIL_IMG_DUSTBIN,
	FIL_IMG_FLAME,
	FIL_IMG_DEFL1,
	FIL_IMG_DEFL2,
	FIL_IMG_FLOOR_0,
	FIL_IMG_FLOOR_1,
	FIL_IMG_FLOOR_2,
	FIL_IMG_FLOOR_3,
	FIL_IMG_FLOOR_4,
	FIL_IMG_FLOOR_5,
	FIL_IMG_FLOOR_6,
	FIL_IMG_FLOOR_7,
	FIL_NUM_IMAGES
};

/* Maximos objetos */
/* FixMe: Debe ser variable */
#define FIL_MAX_OBJ 512

/* Coordenada minima X cartesiana */
#define FIL_MIN_X 32

/* Coordenada maxima X cartesiana */
#define FIL_MAX_X 96

/* Longitud de cada baldosa */
#define FIL_TILE_SIZE (FIL_MAX_X-FIL_MIN_X)/8

#define FIL_PYRAMID_VIEW (FIL_TILE_SIZE * 16)

/* Tiempo de vida de un disparo */
#define FIL_HIT_TIME_LIFE 24

/* Coordenada minima Y cartesiana */
#define FIL_MIN_Y 10

/* Coordenada maxima Y cartesiana */
#define FIL_MAX_Y (4096-16)

/* Velocidad Maxima del Heroe */
#define FIL_MAX_VEL 9

/* Posicion del Heroe minima en pantalla para scroll */
#define FIL_SCROLL_MIN -10

/* Posicion del Heroe maxima en pantalla para scroll */
#define FIL_SCROLL_MAX 10

/* Posicion minima y maxima de un objeto para ser visualizado */
#define FIL_MIN_VISUAL -96
#define FIL_MAX_VISUAL 240

/* Número máximo de disparos*/
#define FIL_MAX_LASER 3

/* Imagenes por segundo */
/* FIXME: En GP32 aun no depurado: desactivado */
#define FIL_FPS (1000 / 50)

/* Tipo de objetos móviles previstos en el juego */
/* incluido dentro de la categoría robot */

/* Ciclopes: Enemigos, Moviles, Descuentan vida, Cambian de direccion al colisionar con un objeto*/
/* Trayectoria asignada cualquiera ( 1-11) */
/* Ojo: Enemigos, Moviles, Descuentan vida, Cambian de direccion aleatoriamente cada cierto tiempo tambien aleatorio*/
/* Trayectoria asignada = 9 */
/* Bocas: Enemigos, Moviles, Descuentan vida, Cambian de direccion cada poco rato */
/* Trayectoria asignada = 11 */
/* Minas: Enemigos, Moviles, Descuentan vida, Trayectorias fijas normalmente de izquierda a derecha */
/*Discos: Enemigos, Moviles, Descuentan vida, Van hacia el heroe */
/* Trayectoria por defecto = 8 */
enum{
	FIL_TYPE_NULL,
	FIL_TYPE_HERO,
	FIL_TYPE_PYRAMID,
	FIL_TYPE_DUMMY,
	FIL_TYPE_DUSTBIN,
	FIL_TYPE_FLAME,
	FIL_TYPE_DEFL1,
	FIL_TYPE_DEFL2,
	FIL_TYPE_BLOQ,
	FIL_TYPE_CYCLOP,
	FIL_TYPE_MOUTH,
	FIL_TYPE_MINE,
	FIL_TYPE_DISC,
	FIL_TYPE_ROBOT,
	FIL_TYPE_LASER,
	FIL_TYPE_BARREL,
	FIL_TYPE_EXPL,
	FIL_TYPE_ELECTRIC,
	FIL_TYPE_BRICK,
	FIL_TYPE_FIRE,
	FIL_TYPE_TILE
};

extern int filmat_lasers;
extern int filmat_spin_laser;

extern int filmat_completed;

/* Array de objetos moviles y no moviles */
extern p_filmat_object filmat_obj; 

/* Posicion global de la pantalla (zona de visualizacion) */
extern int filmat_global_pos;

/* Objeto Heroe */
extern filmat_object filmat_hero;

extern int filmat_hero_accel_x[10][8][2];
extern int filmat_hero_accel_y[10][8][2];
extern int filmat_hero_hiting;

extern int filmat_ship_x;
extern int filmat_ship_y;

extern char *filmat_video_msg;

/* Primer objeto de la lista de visualizacion */
extern p_filmat_object filmat_first;

/* Cantidad actual de imagenes mostradas */
extern Uint32 filmat_frame;

/* Superficies */
extern SDL_Surface *filmat_image[FIL_NUM_IMAGES];

/* Iniciacion de video */
extern int filmat_load_images(void);

extern void filmat_init_frame_time(void);
extern void filmat_delay_frame_time(void);

/* Iniciacion de procesos */
extern int filmat_process_init(void);

/* Proceso padre */
extern void filmat_process();

extern void filmat_pauseScreen(void);

/* Realizar visualizacion */
extern void filmat_draw();

/* Realizar visualizacion para inicio de nivel */
extern void filmat_draw_init_level(void);
extern void filmat_draw_end_level(void);
extern void filmat_draw_init_level_presentation(int i, char *str);
extern void filmat_draw_end_level_presentation(int i, char *str);

/* Funciones de proceso y choque del objecto Heroe */
void filmat_hero_init(int x, int y, int state);
void filmat_hero_process(p_filmat_object this);
void filmat_hero_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Robot */
void filmat_robot_init(p_filmat_object this);
void filmat_robot_process(p_filmat_object this);
void filmat_robot_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Bloque */
void filmat_bloq_init(p_filmat_object this);
void filmat_bloq_process(p_filmat_object this);
void filmat_bloq_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Brick */
void filmat_brick_init(p_filmat_object this);
void filmat_brick_process(p_filmat_object this);
void filmat_brick_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Fire */
void filmat_fire_init(p_filmat_object this);
void filmat_fire_process(p_filmat_object this);
void filmat_fire_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Laser */
void filmat_laser_process(p_filmat_object this);
void filmat_laser_collision(p_filmat_object this, p_filmat_object other);
void filmat_laser_init(void);

/* Funciones de proceso y choque del objecto Piramide */
void filmat_pyramid_init(p_filmat_object this);
void filmat_pyramid_process(p_filmat_object this);
void filmat_pyramid_collision(p_filmat_object this, p_filmat_object other);

/* Funciones de proceso y choque del objecto Robot Nulo */
void filmat_dummy_init(p_filmat_object this, int y);
void filmat_dummy_process(p_filmat_object this);
void filmat_dummy_collision(p_filmat_object this, p_filmat_object other);


/* Función de explosión de objetos */
void filmat_expl_process(p_filmat_object this);

/* Funciones de proceso y choque de objetos movibles */
void filmat_movable_init(p_filmat_object this);
void filmat_movable_process(p_filmat_object this);
void filmat_movable_collision(p_filmat_object this, p_filmat_object other);

/* Carga el mapa e inicializa los objetos */
int filmat_parse_map(void);
void filmat_unload(void);
void filmat_process_exit(void);

/* Funcion de busqueda de un hueco libre dentro del array de objetos */
static inline int search_free_obj(void)
{
	int i;
	for(i=1;i<FIL_MAX_OBJ;i++)
		if (filmat_obj[i].x<0)
			return i;
	return -1;
}

/* Funcion que elimina un objto del array de objetos */
static inline void free_obj(int index)
{
	filmat_obj[index].x=-12345;
	filmat_obj[index].y=-12345;
	filmat_obj[index].collision=(void *)&filmat_bloq_collision;
	filmat_obj[index].process=(void *)&filmat_bloq_process;
	filmat_obj[index].obj_type=FIL_TYPE_NULL;
	filmat_obj[index].next=NULL;
}

static inline void free_all_obj(void)
{
	int i;

	for(i=0;i<FIL_MAX_OBJ;i++)
	{
		free_obj(i);
		filmat_obj[i].position=i;
	}
}

/* Comprobacion de Márgenes generico  */
static inline void limXY(p_filmat_object this)
{
	if (this->x>FIL_MAX_X) this->x=FIL_MAX_X;
	else if (this->x<FIL_MIN_X) this->x=FIL_MIN_X;
	if (this->y>FIL_MAX_Y) this->y=FIL_MAX_Y;
	else if (this->y<FIL_MIN_Y) this->y=FIL_MIN_Y;
}


#endif
