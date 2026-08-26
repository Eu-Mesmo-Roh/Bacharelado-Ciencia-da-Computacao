#Definindo o compilador
CC = gcc

#Definindo as flags de compilação
CFLAGS = -Wall  -Wextra -g

#Nome do executavel
TARGET = programa_arvore

# Listagem dos arquivos fonte e objetos
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

#
all: $(TARGET)

# Regra para criar o executável a partir dos objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra genérica: ensina como transformar qualquer .c em um .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar a "sujeira" (arquivos compilados)
clean:
	rm -f *.o $(TARGET)
