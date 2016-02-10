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
#include "mode7.h"

int phase2_ship_x, phase2_ship_y;
int phase2_global_x, phase2_global_y;

static Uint32 next_time=0;

static inline void init_frame_time(void)
{
	next_time=SDL_GetTicks()+PH2_FPS;
}


static inline void delay_frame_time(void)
{
	Uint32 t=SDL_GetTicks();

	if (t<next_time)
	{
		SDL_Delay(next_time-t);
		next_time+=PH2_FPS;
	}
	else
		next_time=t+PH2_FPS;
}


static inline void getKey(void)
{
	SDL_Event event;

	while(SDL_PollEvent( &event ))
	{
		int key=0;
		int hat=0;
		int set=-1;
		switch(event.type)
		{
			case SDL_QUIT:
				theend=1;
				break;
#ifndef GP32
			case SDL_JOYHATMOTION:
#ifdef DREAMCAST
			    	hat=15^SDL_JoystickGetHat(joy,event.jhat.hat);
#else
			    	hat=SDL_JoystickGetHat(joy,event.jhat.hat);
#endif
				if (hat & SDL_HAT_UP)
					up_pressed=-1;
				else {
					up_pressed=0;
					if (hat & SDL_HAT_DOWN)
						down_pressed=-1;
					else
						down_pressed=0;
				}
				if (hat & SDL_HAT_RIGHT)
					right_pressed=-1;
				else {
					right_pressed=0;
					if (hat & SDL_HAT_LEFT)
						left_pressed=-1;
					else
						left_pressed=0;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				set=0;
			case SDL_JOYBUTTONUP:
				switch (event.jbutton.button)
				{
					case 1:
						theend=-1;
						break;
					default:
						hit1_pressed=set;
				}
				break;
#endif

			case SDL_KEYDOWN:
				key=1;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_RETURN:
					case SDLK_ESCAPE:
						theend=1; break;
					case SDLK_UP:
						up_pressed=key; break;
					case SDLK_DOWN:
						down_pressed=key; break;
					case SDLK_LEFT:
						left_pressed=key; break;
					case SDLK_RIGHT:
						right_pressed=key; break;
					case SDLK_LCTRL:
						hit1_pressed=key; break;
				}
				break;
		}
	}
}

void phase2_process(void)
{
    sll inc=dbl2sll(0.0022);

    phase2_ship_x=128; phase2_ship_y=147;
    phase2_shoot.x=0; phase2_shoot.y=0; phase2_shoot.state=100;
    phase2_bad.x=0; phase2_bad.y=300; phase2_bad.state=105;
    phase2_global_x=0; phase2_global_y=0;

    music_play_fade(MUSIC_LEVEL);
    init_frame_time();

    theend=0;
    while (!theend)
    {
	getKey();
	if (hit1_pressed)
		if (phase2_shoot.state>=25)
		{

			sll angle=sllsub(mode7_angle,sllmul(inc,int2sll((128-phase2_ship_x)<<1)));
			sll multi=int2sll((256-phase2_ship_y)/24);
			phase2_shoot.dx=sllmul(sllrotl(sllcos(angle),3),SLL_CONST_PI);
			phase2_shoot.dy=sllmul(sllrotl(sllsin(angle),3),SLL_CONST_PI);
			phase2_shoot.x=slladd(mode7_x,sllmul(phase2_shoot.dx,multi));
			phase2_shoot.y=slladd(mode7_y,sllmul(phase2_shoot.dy,multi));
			angle=sllsub(mode7_angle,sllmul(inc,int2sll((128-phase2_ship_x)<<2)));
			phase2_shoot.dx=sllmul(sllrotl(sllcos(angle),3),SLL_CONST_PI);
			phase2_shoot.dy=sllmul(sllrotl(sllsin(angle),3),SLL_CONST_PI);
			phase2_shoot.alt=(phase2_ship_y-128)>>2;
			phase2_shoot.state=24;
			play_sound(SND_SHIP_LASER);
		}

	if (left_pressed)
		phase2_ship_x-=8;
	else if (right_pressed)
		phase2_ship_x+=8;
	else if (phase2_ship_x<128)
		phase2_ship_x+=4;
	else if (phase2_ship_x>128)
		phase2_ship_x-=4;

	if (phase2_ship_x>=192)
		phase2_ship_x-=8;
	else if (phase2_ship_x<=64)
		phase2_ship_x+=8;

	if (up_pressed)
	{
		if (mode7_z<512)
		{
			mode7_z += 32;
			phase2_ship_y-=8;
		}
	}
	else if (down_pressed)
	{
		if (mode7_z>256)
		{
			mode7_z -= 32;
			phase2_ship_y+=8;
		}

	}
	else
	if (mode7_z>400)
	{
		mode7_z-=16;
		phase2_ship_y+=4;
	}
	else
	if (mode7_z<368)
	{
		mode7_z+=16;
		phase2_ship_y-=4;
	}

	mode7_process(sllsub(mode7_angle,sllmul(inc,int2sll((128-phase2_ship_x)>>1))));
	phase2_global_x= sll2int(mode7_x);
	phase2_global_y= sll2int(mode7_y);

	phase2_draw();
	delay_frame_time();
    }
}


int phase2_process_init(void)
{
	mode7_init();
        up_pressed=0; down_pressed=0; left_pressed=0; right_pressed=0; hit1_pressed=0;
	return 0;
}
