CC = gcc -g
OBJS = wird.o util.o print.o

wird: $(OBJS)
	$(CC) $(OBJS) -o wird

wird.o: wird.c wird.h pages.h config.h
print.o: print.c print.h
util.o: util.c util.h

clean: 
	rm *.o wird
