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




#ifndef PHASE2_H
#define PHASE2_H

#include "vorton.h"
#include "math-sll.h"
#include "mode7.h"

#define PH2_FPS (1000 / 33)

#define PH2_DATA_COMMON DATA_PREFIX "phase2" FILE_SEPARATOR

enum {
        PH2_IMG_SHIP_0,
        PH2_IMG_SHIP_1,
        PH2_IMG_SHIP_2,
        PH2_IMG_SHIP_3,
        PH2_IMG_SHIP_4,
        PH2_IMG_SHADOW_0,
        PH2_IMG_SHADOW_1,
        PH2_IMG_SHADOW_2,
        PH2_IMG_SHOOT,
        PH2_IMG_BAD,
        PH2_IMG_BACKGROUND_0,
        PH2_IMG_BACKGROUND_1,
        PH2_IMG_BACKGROUND_2,
        PH2_IMG_BACKGROUND_3,
        PH2_IMG_BACKGROUND_4,
        PH2_IMG_BACKGROUND_5,
        PH2_IMG_BACKGROUND_6,
        PH2_IMG_BACKGROUND_7,
        PH2_IMG_SKY,
	PH2_NUM_IMAGES,
};

#define PH2_SKY_WIDTH 64
#define PH2_SKY_HEIGHT 52
#define PH2_SKY_MASK (PH2_SKY_WIDTH-1)
#define PH2_SKY_ITER (320 / PH2_SKY_WIDTH)

extern SDL_Surface *phase2_image[PH2_NUM_IMAGES];

extern mode7_object phase2_shoot, phase2_bad;

extern int phase2_ship_x, phase2_ship_y;
extern int phase2_global_x, phase2_global_y;

extern void phase2_draw(void);
extern void phase2_process(void);
extern int phase2_process_init(void);
extern int phase2_load_images(void);
extern void phase2_unload(void);

#endif
