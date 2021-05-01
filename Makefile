defrag: a3.c
	cc -g -pthread -std=gnu11 -Wall -Werror -o defrag a3.c

clean: defrag
	rm defrag