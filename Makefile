main: main.o cities.o
	gcc main.o cities.o -o main

main.o: main.c weather.h
	gcc -c main.c

cities.o: cities.c weather.h  
	gcc -c cities.c

clean:
	@echo "Removing everything except source files..."
	rm *.o main