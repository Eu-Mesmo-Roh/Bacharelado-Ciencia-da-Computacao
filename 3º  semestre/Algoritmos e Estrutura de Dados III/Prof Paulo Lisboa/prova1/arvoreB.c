#include "arvoreB.h"

struct nodo *cria_nodo (int32_t t_arvore, bool eh_folha){
    struct nodo *novo_nodo;

    novo_nodo = malloc (sizeof (struct nodo));
    if (!novo_nodo)
        erro ("Falha ao alocar memoria para nodo");//função contida na fila.h

    // valores iniciais para o nodo
    novo_nodo->n_chaves = 0;
    novo_nodo->eh_folha = eh_folha;

    // alocação conforme descrição do Cormen para nº max chaves e filhos
    novo_nodo->chaves = malloc (sizeof (int32_t) * (2 * t_arvore - 1));
    if (!novo_nodo->chaves)
        erro ("Falha ao alocar memoria para chaves de um nodo");

    novo_nodo->filhos = malloc (sizeof (struct nodo *) * (2 * t_arvore));
    if (!novo_nodo->filhos)
        erro ("Falha ao alocar memoria para filhos do nodo");

    // atribui todos os ponteiros de filho como NULL para evitar lixos de memoria 
    for (int32_t i = 0; i < (2 * t_arvore); i++ )
        novo_nodo->filhos[i] = NULL;

    return novo_nodo;
}

struct arvoreB *criarArvoreB (int32_t t_arvore){
    struct arvoreB *nova_arvore;

    nova_arvore = malloc (sizeof (struct arvoreB));
    if (!nova_arvore)
        erro ("Falha ao alocar memoria para arvore");

    nova_arvore->t_arvore = t_arvore;

    // cria o nodo e preenche os membros para armazenar na arvore 
    nova_arvore->raiz = cria_nodo (t_arvore, true);
    
    return nova_arvore;
}

void dividir_filho (struct nodo *nodo, int32_t indice, int32_t t_arvore){
    struct nodo *aux, *novo_nodo;

    aux = nodo->filhos[indice];
    novo_nodo = cria_nodo (t_arvore, aux->eh_folha);
    novo_nodo->n_chaves = t_arvore - 1;

    // copia todas as t-1 chaves dos filhos do nodo para o novo
    for (int32_t i = 0; i < t_arvore - 1; i++)
        novo_nodo->chaves[i] = aux->chaves[i + t_arvore];

    // copia os filhos maiores caso o nodo não seja folha 
    if (!aux->eh_folha)
        for (int32_t i = 0; i < t_arvore; i++)
            novo_nodo->filhos[i] = aux->filhos[i + t_arvore];

    aux->n_chaves = t_arvore - 1;

    // desloca os ponteiros de filhos
    for (int32_t i = nodo->n_chaves; i >= indice + 1; i--)
        nodo->filhos[i + 1] = nodo->filhos[i];

    nodo->filhos [indice + 1] = novo_nodo;

    // abre espaço no pai para chave que for substituir
    for (int32_t i = nodo->n_chaves - 1; i >= indice; i--)
        nodo->chaves[i + 1] = nodo->chaves[i];

        // split na chave escolhida por mediana
    nodo->chaves[indice] = aux->chaves[t_arvore - 1];   
    nodo->n_chaves++;
}

void inserir_não_cheio (struct nodo *nodo, int32_t chave, int32_t t_arvore){
    int32_t aux;

    aux = nodo->n_chaves;

    if (nodo->eh_folha){
        // descoloca as chaves maiores do nodo para abrir espaço para nova
        // ser inserida - adaptado para c
        while ((aux > 0) && (chave < nodo->chaves[aux - 1])){
            nodo->chaves[aux] = nodo->chaves[aux - 1];
            aux--;
        }

        nodo->chaves[aux] = chave;
        nodo->n_chaves++; // atualiza quantidade de chaves no nodo
    }

    else{
        // encontra o filho ao qual descer 
        while ((aux > 0) && (chave < nodo->chaves[aux - 1]))
            aux--;

        // realiza a divisão do filho se ele já esta cheio
        if (nodo->filhos[aux]->n_chaves ==  (2 * t_arvore - 1)){
            dividir_filho (nodo, aux, t_arvore); 

            // verificação para saber se a chave desce para o filho a direita
            if (chave > nodo->chaves[aux])
                aux++;
        }

        //chamada recursiva descendo para o filho
        inserir_não_cheio (nodo->filhos[aux], chave, t_arvore);
    }
}

struct nodo *dividir_Raiz (struct arvoreB *arvore){
    struct nodo *nova_raiz;

    nova_raiz = cria_nodo(arvore->t_arvore, false);
    nova_raiz->filhos[0] = arvore->raiz; // adaptado para linguagem c
    arvore->raiz = nova_raiz;
    
    dividir_filho (nova_raiz, 0, arvore->t_arvore);
    
    return nova_raiz; 
}

void inserirArvoreB (struct arvoreB* arvore, int32_t chave){
    struct nodo *aux, *novo_nodo;

    aux = arvore->raiz;
    // verificar se o nodo já esta cheio, ou seja as 2t-1 preenchidas
    if (aux->n_chaves == (2 * arvore->t_arvore - 1)){
        novo_nodo = dividir_Raiz (arvore);
        inserir_não_cheio (novo_nodo, chave, arvore->t_arvore);
    }

    else 
        // insere na raiz, pois é o unico nodo
        inserir_não_cheio(aux, chave, arvore->t_arvore);

}

void imprimirArvoreB(struct arvoreB* arvore){
    
    if(!arvore || !arvore->raiz)
        erro("Arvore vazia");

    struct Fila *fila = criarFila();
    if(!fila)
        erro("Falha ao criar fila");
    
    int nivel, nos_nivel_atual;
    
    nivel = 0;
    
    //incia a busca em largura, inserindo a raiz da arvore na fila
    inserir_fila(fila, arvore->raiz);

    while (!fila_vazia(fila)){
        // recebe quantos nodos tem no nivel atual para controle de impressão
        nos_nivel_atual = fila->tamanho;

        // cabeçalho do nivel
        printf("----//----\n");
        printf("Nivel %d\n", nivel);
        printf("----//----\n");

        // percorre os nodos do nivel atual
        for(int i = 0; i < nos_nivel_atual; i++){
            struct nodo *nodo_atual = remover_fila(fila);

            // Indica se o nodo é interno ou folha
            printf("%c ", nodo_atual->eh_folha ? 'F' : 'I');

            //numero de chaves
            printf("(n:%d) [", nodo_atual->n_chaves);

            for(int j = 0; j < nodo_atual->n_chaves; j++){
                printf("%d", nodo_atual->chaves[j]);
                // espaço entre as chaves, mas não após a última
                if(j < nodo_atual->n_chaves - 1)
                    printf(" ");
            }
            printf("]");

            // espaçamento entre os nodos do mesmo nivel, mas não após o último
            if(i < nos_nivel_atual - 1)
                printf("  ");
                
            // insere os filhos dos nodos do nivel atual na fila para o próximo nivel
            if(!nodo_atual->eh_folha){
                for(int j = 0; j <= nodo_atual->n_chaves; j++){
                    if(nodo_atual->filhos[j] != NULL)
                        inserir_fila(fila, nodo_atual->filhos[j]);
                }
            }
        }

        printf("\n");
        nivel++;
    }
    liberar_fila(fila);
}

void travessiaEmOrdem(struct nodo *nodo_atual){
    int32_t i;

    if(!nodo_atual)
        return;


    for(i = 0; i < nodo_atual->n_chaves; i++){
        // desce para folha, que é menor, antes de imprimir a chave
        if(!nodo_atual->eh_folha)
            travessiaEmOrdem(nodo_atual->filhos[i]);
    
        printf("%d ", nodo_atual->chaves[i]);
    }
    
    // vai para o ultimo filho mais a direita da arvore
    if(!nodo_atual->eh_folha) 
        travessiaEmOrdem(nodo_atual->filhos[i]);
}

void imprimirEmOrdem(struct arvoreB* arvore){
    
    if(!arvore || !arvore->raiz)
        erro("Arvore não existe ou esta vazia");

    printf("Em ordem: ");
    travessiaEmOrdem(arvore->raiz);
    printf("\n");
}

struct nodo* buscarArvoreBrec (struct nodo *atual, int32_t chave, int32_t* idxEncontrado){
    int32_t aux;

    aux = 0; // adaptado para vetores em c 
    // laço para não ultrapassar onde a chave esta ou "deveria" estar
    while ((aux < atual->n_chaves) && (chave > atual->chaves[aux]))
        aux++;

    if ((aux < atual->n_chaves) && (chave == atual->chaves[aux])){
        *idxEncontrado = aux;
        return atual;
    } 
    
    // não encontrado
    if (atual->eh_folha){
        *idxEncontrado = -1;
        return NULL;
    }
    
    atual = atual->filhos[aux]; // atual atualizado para ponteiro do filho 

    return buscarArvoreBrec (atual, chave, idxEncontrado); 
}

struct nodo* buscarArvoreB(struct arvoreB* arvore, int32_t chave, int32_t* idxEncontrado){
    struct nodo *aux;

    // chamada de função auxiliar para buscar chave recursivamente
    aux = buscarArvoreBrec (arvore->raiz, chave, idxEncontrado);
    return aux;    
}

void liberarNodos(struct nodo* atual){
    
    if(!atual)
       return;
    
    if(!atual->eh_folha){
        for(int i = 0; i <= atual->n_chaves; i++)
            liberarNodos(atual->filhos[i]);
    }

    free(atual->chaves);
    free(atual->filhos);
    free(atual);        
}

void deletarArvore(struct arvoreB* arvore){
    
    if(!arvore)
        erro ("Arvore não existe");

    liberarNodos(arvore->raiz);
    free(arvore);
}    