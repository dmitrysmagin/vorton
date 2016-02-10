#include "vorton.h"

#include "font_error.h"

static SDL_Surface *font_surface;

//static SFont_FontInfo font;

#define putstring(X,Y,Str) (SFont_PutStringInfo(screen,&font,X,Y,Str))

#define FADE_MIN 16 
#define FADE_MAX 64
#define FADE_STEP 2

static char error_title[]={ 150, ' ', 'E','R','R','O','R',' ','!',0 };

void init_display_error(void)
{
	font_surface=IMG_Load_RW(SDL_RWFromMem((void*)&font_png, FONT_PNG_SIZE),0);
	font.Surface=font_surface;
	SFont_InitFontInfo(&font);
}

static inline void draw_box(int x, int y, int w, int h, Uint32 col)
{
	int rx=(screen->w - w)+screen->w;

	switch(screen->format->BitsPerPixel)
	{
		case 8:
			{
				Uint8 *buf=(Uint8 *)screen->pixels;
				buf+=x+ (y*screen->w);
				for(y=0;y<h;y+=2,buf+=rx)
				{
					Uint8 *buf2=buf+1+screen->w;
					for(x=0;x<w;x+=2,buf+=2,buf2+=2)
					{
						*buf=col;
						*buf2=col;
						
					}
				}
				
			}
			break;
		case 16:
			{
				Uint16 *buf=(Uint16 *)screen->pixels;
				buf+=x+ (y*screen->w);
				for(y=0;y<h;y+=2,buf+=rx)
				{
					Uint16 *buf2=buf+(1+screen->w);
					for(x=0;x<w;x+=2,buf+=2,buf2+=2)
					{
						*buf=col;
						*buf2=col;
						
					}
				}
				
			}
			break;
		case 32:
			{
				Uint32 *buf=(Uint32 *)screen->pixels;
				buf+=x+ (y*screen->w);
				for(y=0;y<h;y+=2,buf+=rx)
				{
					Uint32 *buf2=buf+1+screen->w;
					for(x=0;x<w;x+=2,buf+=2,buf2+=2)
					{
						*buf=col;
						*buf2=col;
						
					}
				}
				
			}
			break;
		default:
			{
				SDL_Rect r;
				r.x=x;
				r.y=y;
				r.w=w;
				r.h=h;
				SDL_FillRect(screen,&r,col);
			}
	}
}


static inline void draw_window(int x, int y, int w, int h, Uint32 col0, Uint32 col1, char *title, char *text)
{
	SDL_Rect r;

	if (y-3-font.h<0)
		y=3+font.h;
	if (x-3<0)
		x=3;
	if (y+h+3>screen->h)
		h=screen->h - (y+3);
	if (x+w+3>screen->w)
		w=screen->w - (x+3);

	if (h<font.h+4)
		h=font.h+4;

	draw_box(x-2,y-2,2,h+4,col0);
	draw_box(x-2,y-2-font.h,w+4,2+font.h,col0);
	draw_box(x-2,y+h,w+4,2,col0);
	draw_box(x+w,y-2,2,h+4,col0);

	r.x=x-3;
	r.y=y-3-font.h;
	r.w=w+6;
	r.h=1;
	SDL_FillRect(screen,&r,col1);
	r.y+=h+5+font.h;
	SDL_FillRect(screen,&r,col1);
	r.y-=h+5+font.h;
	r.w=1;
	r.h=h+6+font.h;
	SDL_FillRect(screen,&r,col1);
	r.x+=w+5;
	SDL_FillRect(screen,&r,col1);

	putstring(x+1,y-font.h,title);

	draw_box(x,y,w,h,col1);

	r.x=x;
	r.y=y;
	r.w=w;
	r.h=1;
	SDL_FillRect(screen,&r,col1);
	r.y+=h-1;
	SDL_FillRect(screen,&r,col1);
	r.y-=h-1;
	r.w=1;
	r.h=h;
	SDL_FillRect(screen,&r,col1);
	r.x+=w-1;
	SDL_FillRect(screen,&r,col1);

	r.x=x;
	r.y=y;
	r.w=w;
	r.h=h;
	SDL_SetClipRect(screen,&r);

	if (SFont_TextWidthInfo(&font,text)<=w)
		putstring(x+2,y+((h-font.h)>>1),text);
	else
	{
		int i;
		char *tmp=calloc(1024,1);
		strncpy(tmp,text,1023);
		while(SFont_TextWidthInfo(&font,tmp)>w)
			tmp[strlen(tmp)-1]=0;
		i=strlen(tmp);
		h=(h-font.h)/3;
		putstring(x+2,y+h,tmp);
		strncpy(tmp,text,1023);
		putstring(x+2,y+h+2+font.h,tmp+i);
		free(tmp);
	}
	SDL_SetClipRect(screen,NULL);
}

void invert_screen_color(void)
{
	int i, total=(screen->w*screen->h);
	Uint32 *buf=(Uint32 *)screen->pixels;

	switch(screen->format->BitsPerPixel)
	{
		case 24:
			total/=3;
			break;
		case 16:
			total>>=1;
			break;
		default:
			total>>=2;
	}
	for(i=0;i<total;i++,buf++)
		*buf^=-1;
}

static inline void errormessage(int x, int y, char *msg)
{
	int i;
	int w=(screen->w)-(x<<1);
	int h=48; //(screen->h)-(y<<2);
	char *title=(char *)&error_title;
	Uint32 col1=SDL_MapRGB(screen->format,32,16,16);

	for(i=FADE_MIN;i<FADE_MAX;i+=FADE_STEP)
	{
		Uint32 col0=SDL_MapRGB(screen->format,i>>1,i>>1,i);
		draw_window(x,y,w,h, col0, col1, title,msg);
		SDL_Flip(screen);
		SDL_Delay(10);
	}
	for(i=FADE_MAX;i>FADE_MIN;i-=FADE_STEP)
	{
		Uint32 col0=SDL_MapRGB(screen->format,i>>1,i>>1,i);
		draw_window(x,y,w, h, col0, col1, title,msg);
		SDL_Flip(screen);
		SDL_Delay(10);
	}
}

void display_error(char *error_display)
{
	SDL_Event event;
	int i,theend=0;

	for(i=0;(i<2)&&(!theend);i++)
	{
		errormessage(8,48,error_display);
		while (SDL_PollEvent(&event)>0)
		{
			if (event.type == SDL_QUIT)
			{
				theend=1;
				break;
			}
		}
	}
	while(!theend)
	{
		if (SDL_PollEvent(&event)>0)
		{
			do{
				switch(event.type)
				{
					case SDL_JOYBUTTONUP:
					case SDL_QUIT:
						theend=1;
						break;
					case SDL_KEYUP:
						if ((event.key.keysym.sym==SDLK_ESCAPE)||(event.key.keysym.sym==SDLK_RETURN))
							theend=1;
						break;
				}
			}while(SDL_PollEvent(&event)>0);
		}
		else
			errormessage(8,48,error_display);
	}
}

void display_error_invert(char *error_display)
{
	invert_screen_color();
	display_error(error_display);
	invert_screen_color();
}
