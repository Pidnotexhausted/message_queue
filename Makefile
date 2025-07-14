.PHONY : all clean

all:
	gcc -o main main.c -g -lrt

clean:
	rm -rf main