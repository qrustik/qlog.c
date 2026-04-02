CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra
DEBUG_FLAGS = -g -O0

TARGET = log.a

$(TARGET): log.c log.h
	$(CC) $(CFLAGS) -c log.c -o log.o
	ar rcs $(TARGET) log.o

test: $(TARGET)
	$(CC) $(CFLAGS) test.c -l:log.a