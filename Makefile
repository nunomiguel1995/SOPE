CC = gcc

CFLAGS=  -Wall

RM = rm -f

all: rmdup.o lsdir.o 

rmdup.o: rmdup.c 
		$(CC) $(CFLAGS) -o bin/rmdup rmdup.c
		
lsdir.o: lsdir.c 
		 $(CC) $(CFLAGS) -o bin/lsdir lsdir.c
		 
clean:  
		$(RM) bin/*