all: merge_sort merge_sort2

merge_sort:merge_sort.c
	gcc -std=c99 -Wall -g -ggdb -O2 -o merge_sort merge_sort.c

merge_sort2:merge_sort2.c
	gcc -std=c99 -Wall -g -ggdb -O2 -o merge_sort2 merge_sort2.c

clean:
	rm -f *.o
	rm -f merge_sort
	rm -f merge_sort2
	rm -f core*
