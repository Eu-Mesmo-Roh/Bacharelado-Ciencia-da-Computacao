#include "arvoreB.h"

int main (){
    struct arvoreB *arvore;
    char continua, acao_arvore;
    int32_t aux_t, chave;

    // teste segundo arquivo base trabalho 
    printf ("\nCriando arvore com t = 2 e inserção de 1 a 12\n");
    arvore = criarArvoreB (2);
    for (int32_t i = 1; i <= 12; i++)
        inserirArvoreB (arvore, i);
    printf("\n");
    imprimirArvoreB (arvore);
    printf("\n");
    imprimirEmOrdem (arvore);
    deletarArvore (arvore);

   do {
        printf ("\nDigite 'c' para iniciar ou 's' para sair: ");
        if (scanf (" %c", &continua) != 1) 
            break;

        if (continua == 'c') {
            printf ("Grau (t): ");
            scanf ("%d", &aux_t);
            arvore = criarArvoreB (aux_t);

            do {
                printf ("\n[i] Inserir, [b] Buscar, [p] Parar: ");
                scanf (" %c", &acao_arvore);

                if (acao_arvore == 'i') {
                    printf("Chave: ");
                    scanf ("%d", &chave);
                    inserirArvoreB (arvore, chave);
                } 
                else if (acao_arvore == 'b') {
                    struct nodo *res;
                    int32_t idx;
                    printf("Chave: ");
                    scanf ("%d", &chave);
                    res = buscarArvoreB (arvore, chave, &idx);
                    if (res) 
                        printf ("Chave encontrada no indice %d\n", idx);
                    else 
                        printf ("Chave nao encontrada (idx: %d)\n", idx);
                }
            } while (acao_arvore == 'i' || acao_arvore == 'b');

            printf("\n");
            imprimirArvoreB (arvore);
            imprimirEmOrdem (arvore);
            deletarArvore (arvore);
        }
    } while (continua == 'c');

    return 0;
}