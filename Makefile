all:
	gcc -g -o main main.c */*/*.c -lm

teste:
	gcc -g -o main $(ARG1) */*/*.c -lm

clean:
	rm -r main

imgs_clean:
	rm -r imgs/*.dot* imgs/arq_dots/*
