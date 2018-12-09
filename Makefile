


all: *.c
	gcc -o QUIKE -O3 $^ -lm -lGL -lGLU -lglut

	
run: all
	./QUIKE map.bmp