all:
	gcc -g -o main main.c */*/*.c -lm

clean:
	rm -r main

imgs_clean:
	rm -r imgs/*.png imgs/arq_dots/*
