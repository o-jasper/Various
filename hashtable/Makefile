default: all

clean:
	rm hash4str_test cons_test

all: hash4str_test cons_test

#Binaries.
hash4str_test: hash4str.c lookup3.c test/hash4str_test.c
	gcc test/hash4str_test.c -o hash4str_test 

cons_test: cons.c symbol.c hash4str.c lookup3.c test/cons_test.c
	gcc test/cons_test.c -o cons_test
