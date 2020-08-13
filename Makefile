CC = gcc
OBJS = wird.o util.o

wird: $(OBJS)
	$(CC) $(OBJS) -o wird

wird.o: wird.c wird.h pages.h config.h
util.o: util.c util.h

clean: 
	rm *.o wird
