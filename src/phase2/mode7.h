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




#ifndef MODE7_H
#define MODE7_H

#include "vorton.h"

typedef struct{
	sll x, y, dx, dy;
	int alt, state;
	SDL_Surface *img;
}mode7_object;

#define MODE7_MAP_X 512
#define MODE7_MAP_Y 512

#define MODE7_MAP_MASK_X (MODE7_MAP_X-1)
#define MODE7_MAP_MASK_Y (MODE7_MAP_Y-1)


#define MODE7_SCALE_X 200
#define MODE7_SCALE_Y 50

extern int mode7_z;
extern sll mode7_angle, mode7_x, mode7_y, mode7_sin, mode7_cos, mode7_z_scale, mode7_dx, mode7_dy;

extern unsigned char *mode7_tile_map;

extern unsigned int mode7_mul320[];

extern void (*mode7) (sll angle, sll cx, sll cy);
extern void (*mode7_stretch) (SDL_Surface *source, SDL_Rect *r);

int mode7_init(void);

void mode7_free(void);

void mode7_process(sll angle);

void mode7_draw_object(mode7_object *obj);

void mode7_stretch8(SDL_Surface *source, SDL_Rect *r);
void mode7_stretch16(SDL_Surface *source, SDL_Rect *r);
void mode7_stretch32(SDL_Surface *source, SDL_Rect *r);

#endif
