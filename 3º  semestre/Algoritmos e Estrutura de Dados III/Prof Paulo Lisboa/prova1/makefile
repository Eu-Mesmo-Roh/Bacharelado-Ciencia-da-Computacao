# makefile Arvore b
# Alunos: Victor Eduardo e Ronaldo Pereira

GRR = 20203937_20240514

# Detectar se possui gcc 14 na maquina 
CC      = $(shell command -v gcc-14 2> /dev/null || echo gcc)
CFLAGS  = -Wall -Wextra -Werror -g -std=c23
LDLIBS  = -lm

TARGET  = prova1_$(GRR)
DIR     = prova1_$(GRR)

# Garante que o comando make compile tudo 
all: $(TARGET)

# binário final
$(TARGET): arvoreB.o test.o fila.o
	$(CC) $(CFLAGS) arvoreB.o test.o fila.o -o $(TARGET) $(LDLIBS)

# Compilação dos módulos individuais [cite: 14]
test.o: test.c arvoreB.h fila.h
	$(CC) $(CFLAGS) -c test.c

arvoreB.o: arvoreB.c arvoreB.h 
	$(CC) $(CFLAGS) -c arvoreB.c

fila.o: fila.c fila.h
	$(CC) $(CFLAGS) -c fila.c

# execução
run: all
	./$(TARGET)

# teste com valgrind 
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# gerar arquivo TGZ para entregar
tgz: clean
	mkdir -p $(DIR)
	cp *.c *.h makefile integrantes.txt $(DIR)
	tar czvf $(DIR).tar.gz $(DIR)
	rm -rf $(DIR)
	@echo "Arquivo $(DIR).tar.gz criado para entrega"

# limpar arquivos temporários
clean:
	rm -f *.o $(TARGET) $(DIR).tar.gz