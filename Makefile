#Compiles the three files using the pthread library
# 'make newshell' compiles the shell
# 'make run' runs the executable
# 'make clean' removes the executable
CC=gcc
CFLAGS=-g
newshell: shell.c shell.h functions.c
	$(CC) -o newshell $(CFLAGS) shell.c functions.c shell.h
clean:
	$(RM) newshell
run: newshell
	./newshell
