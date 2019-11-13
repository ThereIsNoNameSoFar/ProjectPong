#Wykonał Michał Warzecha I rok Informatyka EAIIB grupa 6b
CC = gcc  

LIBS = -lm -lncursesw -ltinfo

FLAGS = -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw
FLAGS += -Wall -pedantic -Wextra -Werror

all: main.o pong.o
	$(CC) main.o pong.o $(LIBS) $(FLAGS) -o ProjectPong.exe

main.o: main.c
	$(CC) main.c $(LIBS) $(FLAGS) -c -o main.o
	
pong.o: pong.c pong.h
	$(CC) pong.c $(LIBS) $(FLAGS) -c -o pong.o
