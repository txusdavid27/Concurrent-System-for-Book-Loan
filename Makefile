programas : solicitante.o receptor.o
	gcc solicitante.o -o solicitante -lpthread
	gcc receptor.o -o receptor -lpthread

	
solicitante : solicitante.o
	gcc solicitante.o -o solicitante
	
receptor : receptor.o
	gcc receptor.o -o receptor

solicitante.o : solicitante.c
	gcc -c solicitante.c
	
receptor.o : receptor.c
	gcc -c receptor.c
	
leerReporte.o : leerReporte.c
	gcc -c leerReporte.c
	
clean:
	rm *.o solicitante receptor 
