all:
	gcc -g -o main *.c */*.c -lm

clean:
	rm -r main