ifeq ($(build_conf),linear)
	SRClib=linear
	DIRlib=libs/linear_lib/
else
	ifeq ($(build_conf),parallel)
		SRClib=parallel
		DIRlib=libs/parallel_lib/
endif
endif

IZ2: vector.o system.o main.o
	gcc main.o system.o vector.o -L$(DIRlib) -l$(SRClib) -o IZ2_$(SRClib)

system.o: src/system/sys_info.c src/system/sys_info.h
	gcc -c src/system/sys_info.c -o system.o

vector.o: src/vector/vector.h src/vector/vector.c
	gcc -c src/vector/vector.c -o vector.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f *.o
