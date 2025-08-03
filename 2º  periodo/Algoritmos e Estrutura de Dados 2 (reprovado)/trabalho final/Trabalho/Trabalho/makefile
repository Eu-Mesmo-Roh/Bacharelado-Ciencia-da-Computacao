# Nome do executável
EXEC = programa

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -Werror

# Arquivos-fonte
SRCS = main.c ordena.c intercala.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Regra padrão: compilar o programa
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Regra para executar o programa
run: $(EXEC)
	./$(EXEC)

# Limpar os arquivos objeto e o executável
clean:
	rm -f $(EXEC) $(OBJS)
	rm -rf runs intermediarios
	rm saida.txt