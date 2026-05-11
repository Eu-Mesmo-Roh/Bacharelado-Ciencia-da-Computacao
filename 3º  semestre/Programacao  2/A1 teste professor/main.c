#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gbv.h"

#define CHAVE_SEGURANÇA "1234"

int main(int argc, char *argv[]) {
    //Sobe a quantidade de argumentos para 4
    if (argc < 4) {
        printf("Uso: %s <opção> <biblioteca> [documentos...]\n", argv[0]);
        return 1;
    }

    //testa se a chave tem 4 caracteres
    char *chave = argv[1];
    if (strlen(chave) != 4)
    {
        printf("A chave de segurança deve conter 4 caracteres");
        return 1;
    }

    if(strcmp(chave, CHAVE_SEGURANÇA) != 0)
    {
        printf("Chave de segurança incorreta");
        return 1;
    }

    const char *opcao = argv[2];
    const char *biblioteca = argv[3];

    Library lib;
    if (gbv_open(&lib, biblioteca) != 0) {
        printf("Erro ao abrir biblioteca %s\n", biblioteca);
        return 1;
    }

    if (strcmp(opcao, "-a") == 0) {
        for (int i = 4; i < argc; i++) {
            gbv_add(&lib, biblioteca, argv[i]);
        }
    } else if (strcmp(opcao, "-r") == 0) {
        for (int i = 4; i < argc; i++) {
            gbv_remove(&lib, argv[i]);
        }
    } else if (strcmp(opcao, "-l") == 0) {
        gbv_list(&lib);
    } else if (strcmp(opcao, "-v") == 0 && argc >= 5) {
        gbv_view(&lib, argv[4]);
    } else if (strcmp(opcao, "-o") == 0 && argc >= 5) {
        gbv_order(&lib, biblioteca, argv[4]);
    } else {
        printf("Opção inválida.\n");
    }   

    // Libera a memória dinâmica alocada para os documentos
    if (lib.docs != NULL) {
        free(lib.docs);
        lib.docs = NULL; // Zera o ponteiro
    }

    return 0;
}