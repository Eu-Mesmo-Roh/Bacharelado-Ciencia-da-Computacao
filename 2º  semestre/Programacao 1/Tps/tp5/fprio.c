// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Função que cria e inicializa um novo nodo da fila de prioridade
struct fpnodo_t *fpnodo_cria(int tipo, int prio, void *item)
{
    struct fpnodo_t *nodo;

    // Aloca memória para um novo nodo
    nodo = malloc(sizeof(struct fpnodo_t));
    if (nodo == NULL)
        return NULL;

    // Inicializa os campos do nodo
    nodo->tipo = tipo;
    nodo->prio = prio;
    nodo->item = item;
    nodo->prox = NULL;

    return nodo;
}

// Função que libera a memória de um nodo
void fpnodo_destroi(struct fpnodo_t *nodo)
{
    if (nodo != NULL) // Verifica se o nodo não é NULL
        free(nodo);

    nodo = NULL;
}

// Função que cria e inicializa a fila de prioridade
struct fprio_t *fprio_cria()
{
    struct fprio_t *fprio;

    fprio = malloc(sizeof(struct fprio_t));

    if (fprio == NULL) // Verifica se a alocação falhou
        return NULL;

    fprio->num = 0;     // Inicializa o número de elementos na fila como 0
    fprio->prim = NULL; // Inicializa a fila como vazia (primeiro nodo é NULL)

    return fprio;
}

// Função que destrói a fila de prioridade e libera a memória de todos os seus nodos
struct fprio_t *fprio_destroi(struct fprio_t *f)
{
    struct fpnodo_t *aux, *aux2;

    if (f == NULL)
        return NULL;

    aux = f->prim;

    // Percorre a lista de nodos e destrói cada nodo
    while (aux != NULL)
    {
        aux2 = aux;
        aux = aux->prox;

        free(aux2->item); // Libera o item, se necessário
        fpnodo_destroi(aux2);
    }

    free(f); // Libera a memória da estrutura da fila

    return NULL;
}

// Função que compara dois nodos, verificando se eles têm o mesmo tipo e prioridade
int compara_nodo(struct fpnodo_t *novo_nodo, struct fpnodo_t *nodo)
{
    if (novo_nodo == NULL || nodo == NULL)
        return -1;

    // Compara o tipo e a prioridade dos dois nodos
    if (novo_nodo->tipo == nodo->tipo && novo_nodo->prio == nodo->prio)
        return 1; // nodos são iguais

    return -1; // nodos são diferentes
}

// Função que insere um item na fila de prioridade
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
    struct fpnodo_t *novo_nodo, *aux, *aux2;

    if (f == NULL || item == NULL)
        return -1;

    // Cria um novo nodo com o item, tipo e prioridade fornecidos
    if ((novo_nodo = fpnodo_cria(tipo, prio, item)) == NULL)
        return -1;

    // Verifica se o novo nodo é igual ao primeiro nodo (evita nodo duplicado)
    if (f->num > 0 && compara_nodo(novo_nodo, f->prim) != -1)
    {
        fpnodo_destroi(novo_nodo); // Libera o nodo em caso de duplicação
        return -1;
    }

    // Se a fila estiver vazia ou o novo nodo tiver menor prioridade que o primeiro
    if (f->num == 0 || novo_nodo->prio < f->prim->prio)
    {
        novo_nodo->prox = f->prim; // O novo nodo aponta para o antigo primeiro nodo
        f->prim = novo_nodo;       // O novo nodo se torna o primeiro da fila
    }
    else
    {
        // Percorre a lista até encontrar a posição correta para inserir
        aux = f->prim;
        aux2 = aux->prox;
        while (aux2 != NULL && novo_nodo->prio >= aux2->prio)
        {
            // Se encontrar um nodo dulpicado, libera o nodo e retorna erro
            if (compara_nodo(novo_nodo, aux2) != -1)
            {
                free(novo_nodo->item); // Libera o item, se necessário
                fpnodo_destroi(novo_nodo);
                return -1;
            }
            aux = aux->prox;
            aux2 = aux2->prox;
        }
        novo_nodo->prox = aux->prox; // Insere o nodo na posição correta
        aux->prox = novo_nodo;
    }

    f->num++;

    return f->num;
}

// Função que retira o item com a maior prioridade da fila
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
    struct fpnodo_t *aux;

    void *aux_item;

    if (f == NULL || tipo == NULL || prio == NULL)
        return NULL;

    if (f->num == 0)
        return NULL;

    // Retira o primeiro nodo da fila
    aux = f->prim;
    f->prim = f->prim->prox; // O próximo nodo se torna o primeiro

    // Atribui os valores de tipo e prioridade do nodo retirado
    *tipo = aux->tipo;
    *prio = aux->prio;

    aux_item = aux->item; // Armazena o item retirado

    fpnodo_destroi(aux); // Libera a memória do nodo retirado

    f->num--; // Decrementa o número de elementos na fila

    return aux_item; // Retorna o item retirado
}

// Função que retorna o número de elementos na fila
int fprio_tamanho(struct fprio_t *f)
{
    if (f == NULL)
        return -1;

    return f->num;
}

// Função que imprime a fila de prioridade
void fprio_imprime(struct fprio_t *f)
{
    struct fpnodo_t *aux;

    if (f == NULL)
        return;

    aux = f->prim;

    // Percorre a lista e imprime cada nodo
    while (aux != NULL)
    {
        printf("(");
        printf("%d %d", aux->tipo, aux->prio);
        printf(")");

        if (aux->prox != NULL)
            printf(" "); // Se não for o último nodo, imprime um espaço

        aux = aux->prox; // Avança para o próximo nodo
    }
}