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




#include "phase2.h"

#define STRECTH_INIT	\
	int x=r->x,y=r->y,h=r->h,w=r->w;	\
	sll ox,oy;	\
	if (x<0)	\
	{		\
		ox=int2sll(-x);	\
		w+=x;		\
		x=0;		\
	}			\
	else			\
		ox=0;		\
	if (x < 320)		\
	{			\
		if (x+w > 320)	\
			w=320 - x; 	\
		if (y<0)	\
		{		\
			oy=int2sll(-y);	\
			h+=y;		\
			y=0;		\
		} else			\
			oy=0;		\
		if (y < 240)		\
		{			\
			if (y+h > 240)	\
				h=240 - y;	\
			sll dx=slldiv(int2sll(source->w),int2sll(r->w));	\
			sll dy=slldiv(int2sll(source->h),int2sll(r->h));	\
			ox=sllmul(dx,ox);	\
			oy=sllmul(dy,oy);	

#define STRETCH_LOOP	\
			for(y=0;y<h;y++,dst+=320)	\
			{		\
				register int tmp_x;	\
				register sll oox=ox;	

#define STRETCH_LOOP2	\
				for(tmp_x=0;tmp_x<w;tmp_x++)	\
				{

#define STRETCH_END	\
					if(tmp)		\
						dst[tmp_x]=tmp;	\
					oox=slladd(oox,dx);	\
				}				\
				oy=slladd(oy,dy);		\
			}		\
		}			\
	}


void mode7_stretch8(SDL_Surface *source, SDL_Rect *r)
{			
	STRECTH_INIT
	Uint8 *dst=screen->pixels + mode7_mul320[y] + x;
	STRETCH_LOOP
	register Uint8 *src=source->pixels + (source->w * sll2int(oy));
	STRETCH_LOOP2
	register Uint8 tmp=src[sll2int(oox)];
	STRETCH_END
}

#ifndef GP32


void mode7_stretch16(SDL_Surface *source, SDL_Rect *r)
{			
	STRECTH_INIT
	Uint16 *dst=screen->pixels + ((mode7_mul320[y] + x)<<1);
	STRETCH_LOOP
	register Uint16 *src=source->pixels + ((source->w * sll2int(oy))<<1);
	STRETCH_LOOP2
	register Uint16 tmp=src[sll2int(oox)];
	STRETCH_END
}

#ifndef DREAMCAST

void mode7_stretch32(SDL_Surface *source, SDL_Rect *r)
{			
	STRECTH_INIT
	Uint32 *dst=screen->pixels + ((mode7_mul320[y] + x)<<2);
	STRETCH_LOOP
	register Uint32 *src=source->pixels + ((source->w * sll2int(oy))<<2);
	STRETCH_LOOP2
	register Uint32 tmp=src[sll2int(oox)];
	STRETCH_END
}

#endif
#endif


#define MODE7_SCALE_X_SLL int2sll(MODE7_SCALE_X)
#define MODE7_SCALE_Y_SLL int2sll(MODE7_SCALE_Y)


void mode7_draw_object(mode7_object *obj)
{
    SDL_Rect r;

printf("draw_object w=%i, h=%i\n",obj->img->w,obj->img->h);
    // The object in this case is at a fixed position of (160, 100).
    // Calculate the position relative to the camera.
    sll obj_x = sllsub(obj->x, mode7_x);
    sll obj_y = sllsub(obj->y, mode7_y);
printf("mode7_x=%f, mode7_y=%f\n",sll2dbl(mode7_x),sll2dbl(mode7_y));
printf("obj_x=%f, obj_y=%f, cos=%f, sin=%f\n",sll2dbl(obj_x),sll2dbl(obj_y),sll2dbl(mode7_cos),sll2dbl(mode7_sin));

    // use a rotation transformation to rotate the object by the camera
    // angle
    sll space_x = slladd(
			sllmul (obj_x, mode7_cos),
			sllmul (obj_y, mode7_sin)
		);
    if (space_x>0)
    {
    	sll space_y = sllsub(
			sllmul (obj_y, mode7_cos),
			sllmul (obj_x, mode7_sin)
			);
    // calculate the screen coordinates that go with these space coordinates
    // by dividing everything by space_x (the distance)
    	r.x = 160 + sll2int (sllmul (slldiv (MODE7_SCALE_X_SLL, space_x), space_y));
    	r.y = sll2int (slldiv (mode7_z_scale, space_x));

printf("space_x=%f, division=%f\n",sll2dbl(space_x),obj->img->w/sll2dbl(space_x));
	space_x=sllinv(sllrotl(space_x,7));
printf("-space_x=%f\n",sll2dbl(space_x));
    // the size of the object has to be scaled according to the distance
    	r.w = sll2int(sllmul(int2sll(obj->img->w),space_x));
    	r.h = sll2int(sllmul(int2sll(obj->img->h),space_x));
    	r.x = r.x - (r.w>>1);
    	r.y -=  (r.h - obj->alt);  
    // draw the object
printf("draw obj x=%i, y=%i, w=%i, h=%i\n",r.x,r.y,r.w,r.h);
    	mode7_stretch(obj->img, &r);
    }
}


