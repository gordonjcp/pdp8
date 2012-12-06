LIBS = -lncurses 
CFLAGS = -c -g -Wall
CC = gcc
STD = _GNU_SOURCE
OBJS = df32.o tty.o cpu.o rim.o preload.o pdp8.o
DEPAL_OBJS = depal.o
.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $<

all: pdp8 depal pal

depal:  $(DEPAL_OBJS)
	$(CC) $(DEPAL_OBJS) -o depal
pal:
	$(CC) pal.c -o pal

pdp8: $(OBJS) 
	$(CC) $(OBJS) -o pdp8 $(LIBS)

clean: 
	rm -f *.o core
clobber: clean
	rm pdp8 pal depal

