all:markov

markov.o:markov.c markov.h
	gcc markov.c -g -c markov.c
	
main.o:main.c markov.h
	gcc main.c -g -c main.c

markov:main.o markov.o
	gcc main.o markov.o -o markov

clean:
	-rm *.o markov
