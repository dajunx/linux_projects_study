all:main.o shell41.o 
	gcc main.o shell41.o -o all
main.o:main.c
	gcc -g -c main.c -o main.o
shell41.o:shell41.c
	gcc -g -c shell41.c -o shell41.o
.PHONY:clean
clean:
	rm -rf *.o all
