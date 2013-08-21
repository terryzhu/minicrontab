OBJ		=	minicron.o entry.o 
CFLAGS	=	-g

all:	clean minicron 
minicron:$(OBJ) 
		gcc $(CFLAGS) -o minicron	$(OBJ)

clean:
		rm -f minicron
		rm -f *.o

