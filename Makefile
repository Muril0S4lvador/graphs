all:
	gcc -g -o main main.c */*/*.c -lm

clean:
	rm -r main

imgs_clean:
	rm -r imgs/*.dot* imgs/arq_dots/*
