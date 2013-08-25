OBJ		=	minicron.o entry.o 
CFLAGS	=	-g -Wall

all:	clean minicron 
minicron:$(OBJ) 
		gcc $(CFLAGS) -o minicron	$(OBJ)

clean:
		rm -f minicron
		rm -f *.o

