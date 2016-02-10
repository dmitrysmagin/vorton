/*
playflic - play a FLIC file on the screen
Original public domain by Andre de Leiradella on 24-fev-2003.
Changed by Chui
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <SFont.h>
#include <SDL_flic.h>

#include "node_text.h"

enum{
	FLI_ERROR_FILE_NOT_FOUND,
	FLI_ERROR_READING,
	FLI_ERROR_CORRUPTED,
	FLI_ERROR_MEMORY
};

static char *fli_err[]=
{
	"FLIC file not found",
	"Error while reading FLIC",
	"FLIC file corrupted",
	"Out of memory"
};


extern void *FLI_SFont;
static SDL_Surface *___sdl_flic_default_sfont_surface=NULL;

void FLI_InitDefaultFont(void)
{
	if (___sdl_flic_default_sfont_surface==NULL)
	{
		___sdl_flic_default_sfont_surface=IMG_Load_RW(SDL_RWFromMem((void*)&FLI_SFont, 1024*1024),0);
		SFont_InitFont(___sdl_flic_default_sfont_surface);
	}
}

void FLI_FreeDefaultFont(void)
{
	if (___sdl_flic_default_sfont_surface!=NULL)
	{
		SDL_FreeSurface(___sdl_flic_default_sfont_surface);
		___sdl_flic_default_sfont_surface=NULL;
	}
}

int FLI_Play(SDL_Surface *screen, SDL_Rect *pos, char *flic_name, int mem, char *music_name, SDL_Surface *font, char *text, int loop)
{
	SDL_RWops	*rwops=NULL;
	void		*buffer=NULL;
	FLI_Animation	*flic;
	Mix_Music	*music=NULL;
	int		error;
	SDL_Rect	tmp_pos;
	SDL_Event	event;
	Uint32		ticks;
	char		*actual_text=NULL;
	Uint32		actual_frame=0;
	int		font_y,font_h=0;
#ifndef GP32
	int		need_erase=0;
#endif

	// Open the flic. 
	if (mem)
	{
		FILE *f=fopen(flic_name,"rb");
		unsigned long size;
		if (f==NULL)
		{
			SDL_SetError(fli_err[FLI_ERROR_FILE_NOT_FOUND]);
			return -1;
		}
		fseek(f,0,SEEK_END);
		size=ftell(f);
		fseek(f,0,SEEK_SET);
		if (size<1)
		{
			fclose(f);
			SDL_SetError(fli_err[FLI_ERROR_READING]);
			return -1;
		}
		buffer=malloc(size);
		if (buffer==NULL)
		{
			fclose(f);
			SDL_SetError(fli_err[FLI_ERROR_MEMORY]);
			return -1;
		}
		if (fread(buffer,1,size,f)!=size)
		{
			free(buffer);
			fclose(f);
			SDL_SetError(fli_err[FLI_ERROR_READING]);
			return -1;
		}
		fclose(f);
		rwops = SDL_RWFromMem(buffer,size);

	}
	else
		rwops = SDL_RWFromFile(flic_name, "rb");
	if (rwops == NULL) {
		if (buffer!=NULL)
			free(buffer);
		SDL_SetError(fli_err[FLI_ERROR_FILE_NOT_FOUND]);
		return -1;
	}
	if (text!=NULL)
	{
		node_text_read(text);
		if (font==NULL)
		{
			FLI_InitDefaultFont();
			font_h=___sdl_flic_default_sfont_surface->h;
		}
		else
		{
			SFont_InitFont(font);
			font_h=font->h;
		}
	}
	actual_frame=node_text_get_frame();
	actual_text=node_text_get_text();
	flic = FLI_Open(rwops, &error);
	if (error != FLI_OK)
		goto out;
	if (screen==NULL)
	{
		// Set the video with the size of the flic.
		screen = SDL_SetVideoMode(flic->width, flic->height, 0, SDL_SWSURFACE);
	}
	if (screen == NULL) {
		goto out;
	}
	// Play the flic. 
	tmp_pos.x = (screen->w-flic->width)>>1;
	tmp_pos.y = (screen->h-flic->height)>>1;
	if (pos==NULL)
		pos=&tmp_pos;
	font_y= screen->h - pos->y - font_h;
#ifndef GP32
	if ((pos->x>0)||(pos->y>0))
		need_erase=-1;
#endif
	if (music_name != NULL)
		music = Mix_LoadMUS(music_name);
	if (music != NULL)
		Mix_PlayMusic(music,-1);
	ticks = SDL_GetTicks();
	for (;;) {
		// Render the next frame.
		error = FLI_NextFrame(flic);
		if (error != FLI_OK)
			goto out;
#ifndef GP32
		if (need_erase)
			SDL_FillRect(screen,NULL,0);
#endif
		// Blit it.
		SDL_BlitSurface(flic->surface, NULL, screen, pos);
		if (actual_text!=NULL)
			SFont_XCenteredString(screen,font_y,actual_text);
		SDL_Flip(screen);
		// Exit program in case of any event.
		while (SDL_PollEvent(&event) == 1) {
			switch (event.type) {
				case SDL_JOYBUTTONUP:
				case SDL_MOUSEBUTTONUP:
				case SDL_KEYUP:
				case SDL_QUIT:
					goto out;
			}
		}
		if (actual_frame==flic->nextframe)
		{
			node_text_next();
			actual_text=node_text_get_text();
			actual_frame=node_text_get_frame();
		}
		if (flic->nextframe==1)
		{
			loop--;
			if (!loop)
				break;
		}
		// Delay between frames.
		ticks = SDL_GetTicks() - ticks;
		if (ticks < flic->delay)
			SDL_Delay(flic->delay - ticks);
		ticks = SDL_GetTicks();
	}
	out:
	// Describe the error.
	switch (error) {
		case FLI_READERROR:
			SDL_SetError(fli_err[FLI_ERROR_READING]);
			break;
		case FLI_CORRUPTEDFILE:
			SDL_SetError(fli_err[FLI_ERROR_CORRUPTED]);
			break;
		case FLI_OUTOFMEMORY:
			SDL_SetError(fli_err[FLI_ERROR_MEMORY]);
			break;
	}
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}
	FLI_Close(flic);
	if (buffer!=NULL)
		free(buffer);
	node_text_free_buffer();
	if (font==NULL)
		FLI_FreeDefaultFont();

	return error;
}
