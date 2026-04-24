CC:= gcc
CFLAGS:= -Werror -Wextra -Wall -std=c11
INCLUDES := -I../includes
CLIBS:= ../bin/qlog.so

.phony: example_1 clean

example_1:
	$(CC) $(CFLAGS) $@.c $(INCLUDES) $(CLIBS) -o $@.o
	./$@.o

clean:
	rm -rf *.o
