CC = gcc
OBJS = wird.o util.o

wird: $(OBJS)
	$(CC) $(OBJS) -o wird

wird.o: wird.c wird.h
util.o: util.c util.h

clean: 
	rm *.o wird
