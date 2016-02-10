TARGET	=	vorton


CC =		gcc
STRIP=		strip


CFLAGS =	-Wall -Wno-switch -DDEBUG \
		-DDATA_PREFIX=\"data/\" -DFILE_SEPARATOR=\"/\" -DARGS \
		-Isrc -Isrc/SDL_flic -Isrc/SFont `sdl-config --cflags` \
		-ffast-math -fno-common -ffreestanding -fno-builtin \
		-fno-exceptions -fstrict-aliasing -O2 \
		-Isrc/filmat -Isrc/phase2 -Isrc/ML_Lib \


#CFLAGS+=-DUSE_FIXED_POINT


LDFLAGS =	`sdl-config --libs` -lSDL_mixer -lSDL_image -lm


.CFILES	=	src/vorton.c \
		src/video.c \
		src/intro.c \
		src/audio.c \
		src/menu.c \
		src/exit.c \
		src/language.c \
		src/records.c \
		src/error.c \
		src/rand.c \
		src/filmat/laser.c \
		src/filmat/process.c \
		src/filmat/hero.c \
		src/filmat/bloq.c \
		src/filmat/brick.c \
		src/filmat/fire.c \
		src/filmat/robot.c \
		src/filmat/movable.c \
		src/filmat/parse.c \
		src/filmat/pyramid.c \
		src/filmat/dummy.c \
		src/filmat/filmat.c \
		src/filmat/video.c \
		src/phase2/video.c \
		src/phase2/process.c \
		src/phase2/mode7.c \
		src/phase2/stretch.c \
		src/SDL_flic/SDL_flic.c \
		src/SDL_flic/SDL_playflic.c \
		src/SDL_flic/node_text.c \
		src/SDL_flic/font-arial-12.c \
		src/SFont/SFont.c \
		src/ML_Lib/ML_Attribute.c \
		src/ML_Lib/ML_Comment.c \
		src/ML_Lib/ML_common_func.c \
		src/ML_Lib/ML_COM_Sim.c \
		src/ML_Lib/ML_Document.c \
		src/ML_Lib/ML_Element.c \
		src/ML_Lib/ML_leng_process.c \
		src/ML_Lib/ML_Node.c \
		src/ML_Lib/ML_Process.c \
		src/ML_Lib/ML_Text.c \


all: $(TARGET)


OBJS =		$(.CFILES:.c=.o)


$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(TARGET)


run:		$(TARGET)

#	./$(TARGET) -nosound
#	./$(TARGET) -nosound -8bits
	./$(TARGET) -windowscreen -nosound -8bits
#	./$(TARGET) -windowscreen -8bits

clean:
	rm -f $(OBJS) $(TARGET)
