dd:dd.o avl_tree.o
	g++ -Wall -g -ggdb -O2 -o dd dd.o avl_tree.o

dd.o:dd.cpp
	g++ -Wall -g -ggdb -O2 -c -o dd.o dd.cpp

avl_tree.o:avl_tree.cpp
	g++ -Wall -g -ggdb -O2 -c -o avl_tree.o avl_tree.cpp

clean:
	rm -f *.o
	rm -f dd
	rm -f core*
