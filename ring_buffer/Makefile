all:test0 test1 test2 test3
.PHONY:all

test0:
	gcc -o test0 -g test0.c -lpthread
	rm -f *.o

test1:
	gcc -o test1 -g test1.c -lpthread
	rm -f *.o

test2:
	gcc -o test2 -g test2.c -lpthread
	rm -f *.o

test3:
	gcc -o test3 -g test3.c -lpthread
	rm -f *.o

clean:
	rm -rf test0
	rm -rf test1
	rm -rf test2
	rm -rf test3
	rm -rf *.o
