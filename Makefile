CC = gcc

CFLAGS=  -Wall

RM = rm -f

all: rmdup.o listdir.o 

rmdup.o: rmdup.c 
		$(CC) $(CFLAGS) -o bin/rmdup rmdup.c
		
listdir.o: listdir.c 
		 $(CC) $(CFLAGS) -o bin/listdir listdir.c
		 
clean:  
		$(RM) bin/*