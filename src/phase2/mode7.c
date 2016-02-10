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

unsigned char *mode7_tile_map;

void (*mode7) (sll angle, sll cx, sll cy);
void (*mode7_stretch) (SDL_Surface *source, SDL_Rect *r);

int mode7_z;
sll mode7_angle, mode7_x, mode7_y, mode7_cos, mode7_sin, mode7_z_scale, mode7_dx, mode7_dy;

static Uint8 *tile8[8];
static Uint16 *tile16[8];
static Uint32 *tile32[8];

static unsigned int *mulMap;

unsigned int mode7_mul320[]=
	{
		0, 320, 640, 960, 1280, 1600, 1920, 2240, 2560, 2880, 3200, 3520, 3840, 4160, 4480, 4800, 
		5120, 5440, 5760, 6080, 6400, 6720, 7040, 7360, 7680, 8000, 8320, 8640, 8960, 9280, 9600, 9920, 
		10240, 10560, 10880, 11200, 11520, 11840, 12160, 12480, 12800, 13120, 13440, 13760, 14080, 14400, 14720, 15040, 
		15360, 15680, 16000, 16320, 16640, 16960, 17280, 17600, 17920, 18240, 18560, 18880, 19200, 19520, 19840, 20160, 
		20480, 20800, 21120, 21440, 21760, 22080, 22400, 22720, 23040, 23360, 23680, 24000, 24320, 24640, 24960, 25280, 
		25600, 25920, 26240, 26560, 26880, 27200, 27520, 27840, 28160, 28480, 28800, 29120, 29440, 29760, 30080, 30400, 
		30720, 31040, 31360, 31680, 32000, 32320, 32640, 32960, 33280, 33600, 33920, 34240, 34560, 34880, 35200, 35520, 
		35840, 36160, 36480, 36800, 37120, 37440, 37760, 38080, 38400, 38720, 39040, 39360, 39680, 40000, 40320, 40640, 
		40960, 41280, 41600, 41920, 42240, 42560, 42880, 43200, 43520, 43840, 44160, 44480, 44800, 45120, 45440, 45760, 
		46080, 46400, 46720, 47040, 47360, 47680, 48000, 48320, 48640, 48960, 49280, 49600, 49920, 50240, 50560, 50880, 
		51200, 51520, 51840, 52160, 52480, 52800, 53120, 53440, 53760, 54080, 54400, 54720, 55040, 55360, 55680, 56000, 
		56320, 56640, 56960, 57280, 57600, 57920, 58240, 58560, 58880, 59200, 59520, 59840, 60160, 60480, 60800, 61120, 
		61440, 61760, 62080, 62400, 62720, 63040, 63360, 63680, 64000, 64320, 64640, 64960, 65280, 65600, 65920, 66240, 
		66560, 66880, 67200, 67520, 67840, 68160, 68480, 68800, 69120, 69440, 69760, 70080, 70400, 70720, 71040, 71360, 
		71680, 72000, 72320, 72640, 72960, 73280, 73600, 73920, 74240, 74560, 74880, 75200, 75520, 75840, 76160, 76480, 
	};


#define MODE7_SCALE_X_SLL int2sll(MODE7_SCALE_X)



#define MODE7_PROCESS	\
    int screen_x, screen_y;	\
    sll distance, horizontal_scale;	\
    sll line_dx, line_dy;		\
    sll space_x, space_y;		\
    int init_y=(mode7_z>>4) + 19;	\
    for (screen_y = init_y; screen_y<240; screen_y++)	\
    {					\
	distance=int2sll((mode7_z << 5)/screen_y);	\
        horizontal_scale = slldiv (distance, MODE7_SCALE_X_SLL);	\
        line_dx = sllmul (sllneg(sllsin(angle)), horizontal_scale);	\
        line_dy = sllmul (sllcos(angle), horizontal_scale);	\
        space_x = sllsub(slladd(cx, sllmul (distance, sllcos(angle))), sllmul(int2sll(160), line_dx));	\
        space_y = sllsub(slladd(cy, sllmul (distance, sllsin(angle))), sllmul(int2sll(160), line_dy));	\
        for (screen_x = 0; screen_x<320; screen_x++)	\
        {						\
	    register int x=sll2int(space_x);		\
	    register int y=sll2int(space_y);		\
	    _scr[screen_x+mode7_mul320[screen_y]]=	\
		_tile[					\
			mode7_tile_map[			\
					((x>>4)&MODE7_MAP_MASK_X) +	\
					mulMap[(y>>4)&MODE7_MAP_MASK_Y]	\
				]			\
		     ] [(x&0xF)+((y&0xF)<<4)];		\
            space_x=slladd(space_x,line_dx);		\
            space_y=slladd(space_y,line_dy);		\
        }		\
    }			

void mode7_8bits(sll angle, sll cx, sll cy)
{
    Uint8 *_scr=screen->pixels;
    Uint8 **_tile=(Uint8 **)&tile8;
    MODE7_PROCESS
}

#ifndef GP32

void mode7_16bits(sll angle, sll cx, sll cy)
{
    Uint16 *_scr=screen->pixels;
    Uint16 **_tile=(Uint16 **)&tile16;
    MODE7_PROCESS
}

#ifndef DREAMCAST

void mode7_32bits(sll angle, sll cx, sll cy)
{
    Uint32 *_scr=screen->pixels;
    Uint32 **_tile=(Uint32 **)&tile32;
    MODE7_PROCESS
}

#endif

#endif

static inline void putMap(int x, int y,int value)
{
	mode7_tile_map[x + MODE7_MAP_X*y]=value;
}


int mode7_init(void)
{
    int i;

    mode7_tile_map=(unsigned char*)calloc(MODE7_MAP_X,MODE7_MAP_Y);
    if (mode7_tile_map==NULL)
	return -1;

    mulMap=(unsigned int*)calloc(MODE7_MAP_Y,sizeof(unsigned int));
    if (mulMap==NULL)
    {
	free(mode7_tile_map);
	mode7_tile_map=NULL;
	return -2;
    }
    for(i=0;i<MODE7_MAP_Y;i++)
	mulMap[i]=(MODE7_MAP_X)*i;

    for(i=0;i<MODE7_MAP_X*(MODE7_MAP_Y>>5);i++)
	putMap( (getRand()&MODE7_MAP_MASK_X), (getRand()&MODE7_MAP_MASK_Y), 6);
    for(i=0;i<MODE7_MAP_Y*(MODE7_MAP_X>>5);i++)
	putMap( (getRand()&MODE7_MAP_MASK_X), (getRand()&MODE7_MAP_MASK_Y), 7);
    for(i=0;i<MODE7_MAP_X;i++)
    {
	int ca_x=(getRand()&(MODE7_MAP_MASK_X>>1));
	int ca_y=(getRand()&(MODE7_MAP_MASK_Y>>1));
	ca_x<<=1;
	ca_y<<=1;
	putMap(ca_x, ca_y, 2);
	putMap(ca_x, ca_y+1, 3);
	putMap(ca_x+1, ca_y, 5);
	putMap(ca_x+1, ca_y+1, 4);
    }

    for(i=0;i<MODE7_MAP_Y>>3;i++)
    {
	int ca_x=(getRand()&(MODE7_MAP_MASK_X>>3));
	int ca_y=(getRand()&(MODE7_MAP_MASK_Y>>3));
	ca_x<<=3;
	ca_y<<=3;
        putMap(ca_x,   ca_y, 0);
        putMap(ca_x+1, ca_y, 0);
        putMap(ca_x+2, ca_y, 0);
        putMap(ca_x+3, ca_y, 0);
        putMap(ca_x+4, ca_y, 0);
        putMap(ca_x+5, ca_y, 0);
        putMap(ca_x+6, ca_y, 0);
        putMap(ca_x+7, ca_y, 0);
	
        putMap(ca_x,   ca_y+1, 0);
        putMap(ca_x+1, ca_y+1, 2);
        putMap(ca_x+2, ca_y+1, 1);
        putMap(ca_x+3, ca_y+1, 1);
        putMap(ca_x+4, ca_y+1, 1);
        putMap(ca_x+5, ca_y+1, 1);
        putMap(ca_x+6, ca_y+1, 5);
        putMap(ca_x+7, ca_y+1, 0);
	
        putMap(ca_x,   ca_y+2, 0);
        putMap(ca_x+1, ca_y+2, 1);
        putMap(ca_x+2, ca_y+2, 1);
        putMap(ca_x+3, ca_y+2, 1);
        putMap(ca_x+4, ca_y+2, 1);
        putMap(ca_x+5, ca_y+2, 1);
        putMap(ca_x+6, ca_y+2, 1);
        putMap(ca_x+7, ca_y+2, 0);
	
        putMap(ca_x,   ca_y+3, 0);
        putMap(ca_x+1, ca_y+3, 1);
        putMap(ca_x+2, ca_y+3, 1);
        putMap(ca_x+3, ca_y+3, 1);
        putMap(ca_x+4, ca_y+3, 1);
        putMap(ca_x+5, ca_y+3, 1);
        putMap(ca_x+6, ca_y+3, 1);
        putMap(ca_x+7, ca_y+3, 0);
	
        putMap(ca_x,   ca_y+4, 0);
        putMap(ca_x+1, ca_y+4, 1);
        putMap(ca_x+2, ca_y+4, 1);
        putMap(ca_x+3, ca_y+4, 1);
        putMap(ca_x+4, ca_y+4, 1);
        putMap(ca_x+5, ca_y+4, 1);
        putMap(ca_x+6, ca_y+4, 1);
        putMap(ca_x+7, ca_y+4, 0);
	
        putMap(ca_x,   ca_y+5, 0);
        putMap(ca_x+1, ca_y+5, 1);
        putMap(ca_x+2, ca_y+5, 1);
        putMap(ca_x+3, ca_y+5, 1);
        putMap(ca_x+4, ca_y+5, 1);
        putMap(ca_x+5, ca_y+5, 1);
        putMap(ca_x+6, ca_y+5, 1);
        putMap(ca_x+7, ca_y+5, 0);
	
        putMap(ca_x,   ca_y+6, 0);
        putMap(ca_x+1, ca_y+6, 3);
        putMap(ca_x+2, ca_y+6, 1);
        putMap(ca_x+3, ca_y+6, 1);
        putMap(ca_x+4, ca_y+6, 1);
        putMap(ca_x+5, ca_y+6, 1);
        putMap(ca_x+6, ca_y+6, 4);
        putMap(ca_x+7, ca_y+6, 0);
	
        putMap(ca_x,   ca_y+7, 0);
        putMap(ca_x+1, ca_y+7, 0);
        putMap(ca_x+2, ca_y+7, 0);
        putMap(ca_x+3, ca_y+7, 0);
        putMap(ca_x+4, ca_y+7, 0);
        putMap(ca_x+5, ca_y+7, 0);
        putMap(ca_x+6, ca_y+7, 0);
        putMap(ca_x+7, ca_y+7, 0);
    }
    for(i=0;i<8;i++)
    {
	tile8[i]=phase2_image[PH2_IMG_BACKGROUND_0+i]->pixels;
	tile16[i]=phase2_image[PH2_IMG_BACKGROUND_0+i]->pixels;
	tile32[i]=phase2_image[PH2_IMG_BACKGROUND_0+i]->pixels;
    }
    switch(screen->format->BitsPerPixel)
    {
#ifndef GP32
#ifndef DREAMCAST
	case 32:
    		mode7=&mode7_32bits;
 		mode7_stretch=&mode7_stretch32;
		break;
#endif
	case 16:
    		mode7=&mode7_16bits;
 		mode7_stretch=&mode7_stretch16;
		break;
#endif
	default:
    		mode7=&mode7_8bits;
 		mode7_stretch=&mode7_stretch8;
    }
    mode7_angle = int2sll(2);
    mode7_x = 0; mode7_y = 0;
    mode7_z = 384;
    return 0;
}

void mode7_free(void)
{
	if (mode7_tile_map!=NULL)
		free(mode7_tile_map);
	if (mulMap!=NULL)
		free(mulMap);
	mode7_tile_map=NULL;
	mulMap=NULL;
}


void mode7_process(sll angle)
{
	mode7_angle = angle;
	mode7_cos= sllcos(mode7_angle);
	mode7_sin= sllsin(mode7_angle);
	mode7_dx=sllrotl(mode7_cos,4);
	mode7_dy=sllrotl(mode7_sin,4);
	mode7_x = slladd (mode7_x, mode7_dx);
	mode7_y = slladd (mode7_y, mode7_dy);
	mode7_z_scale = int2sll(mode7_z<<5);
}
