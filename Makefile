


all: *.c
	gcc -o QUIKE $^ -lm -lGL -lGLU -lglut

	
run: all
	./QUIKE