all:
	gcc -g -c rbtree.c
	gcc -g -c test.c
	gcc -o exe rbtree.o test.o

clean:
	rm -rf *.o exe