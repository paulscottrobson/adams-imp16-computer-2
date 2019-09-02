

CC      = gcc
EXE	= 
CFLAGS  = -g -O2 -DCOLOUR -DDEBUGGABLE -D_THREAD_SAFE -I/usr/local/include/SDL2 -I/usr/X11R6/include  -I/usr/X11/include -DHAVE_OPENGL -g
LIBS	=  -lSDL2_test -L/usr/local/lib -lSDL2 

TARGETS = imp16$(EXE)

all: $(TARGETS)

imp16$(EXE): main.c system.c core.c debugger.c core_sdl.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

