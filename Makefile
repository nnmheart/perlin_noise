includes = includes
lib = lib
out = app/perlin_noise.exe
c_files = src/main.c src/perlin.c

all : $(OBJS)
#-Wl,-subsystem,windows
	gcc $(c_files) -I$(includes) -L$(lib) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o $(out)