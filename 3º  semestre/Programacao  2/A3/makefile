# Configurações do compilador e bibliotecas
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lm
TARGET = Game

# Listagem dos arquivos fonte e objetos

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Regras de compilação

all: $(TARGET)


$(TARGET): $(OBJS)	# Regra para criar o executável a partir dos objetos
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c # Regra para compilar arquivos .c em .o
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean