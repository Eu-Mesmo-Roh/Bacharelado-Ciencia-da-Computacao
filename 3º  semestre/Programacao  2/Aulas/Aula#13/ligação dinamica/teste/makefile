CC = gcc

CFLAGS = -Wall -fPIC

hello: main.o hello_pt.o hello_en.o hello_fr.o
	$(CC) $(CFLAGS) -o hello main.o hello_pt.o hello_en.o hello_fr.o

main.o: main.c hello.h
	$(CC) $(CFLAGS) -c main.c

hello_pt.o: hello_pt.c hello.h
	$(CC) $(CFLAGS) -c hello_pt.c

hello_en.o: hello_en.c hello.h
	$(CC) $(CFLAGS) -c hello_en.c

hello_fr.o: hello_fr.c hello.h
	$(CC) $(CFLAGS) -c hello_fr.c

clean:
	rm -f *.o hello main