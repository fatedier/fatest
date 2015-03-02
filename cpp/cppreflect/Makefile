dd:dd.o reflect.o
	g++ -o dd dd.o reflect.o

dd.o:dd.cpp
	g++ -c -o dd.o dd.cpp

reflect.o:reflect.cpp
	g++ -c -o reflect.o reflect.cpp

clean:
	rm -f *.o
	rm -f core*
	rm dd
