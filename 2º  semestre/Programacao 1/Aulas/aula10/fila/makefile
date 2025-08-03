# makefile generico para os TADs
# Carlos Maziero - DINF/UFPR

CC     = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LIB    = fila
BIN    = teste

# gera o arquivo executável
$(BIN):   $(BIN).o $(LIB).o

# gera os arquivos-objeto
$(BIN).o: $(BIN).c $(LIB).h
$(LIB).o: $(LIB).c $(LIB).h

# limpeza
clean:
	rm -f *.o *~ $(BIN)
