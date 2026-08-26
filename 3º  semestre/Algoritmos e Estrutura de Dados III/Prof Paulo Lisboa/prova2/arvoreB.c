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

struct nodo *encontrarPred (struct nodo *filho_atual, int *idxEncontrado){

    if (filho_atual->eh_folha){
        // o predecessor é o ultimo indice do vetor
        *idxEncontrado = filho_atual->n_chaves - 1;
        return filho_atual;
    }
    
    // desce pelo filho mais a direita
    return encontrarPred (filho_atual->filhos[filho_atual->n_chaves], idxEncontrado);       
}

struct nodo *encontrarSuc (struct nodo *filho_atual, int *idxEncontrado){

    if (filho_atual->eh_folha){
        *idxEncontrado = 0;
        return filho_atual;
    }
    // Caso contrário, continua descendo pelo filho mais à esquerda
    return encontrarSuc(filho_atual->filhos[0], idxEncontrado);       
}

void mesclarFilhos(struct nodo *atual, int32_t indice, int32_t t_arvore){
    struct nodo *filho_esq, *filho_dir;
    

    filho_esq = atual->filhos[indice];
    filho_dir = atual->filhos[indice + 1];
    // chave do pai para o meio do filho esquerdo
    filho_esq->chaves[t_arvore - 1] = atual->chaves[indice]; 

    // copia as chaves do filho direito para o filho esquerdo
    for (int32_t i = 0; i < filho_dir->n_chaves; i++)
        filho_esq->chaves[i + t_arvore] = filho_dir->chaves[i];
    
    // copia os filhos do filho direito para o filho esquerdo, se não for folha
    if (!filho_esq->eh_folha){
        for (int32_t i = 0; i <= filho_dir->n_chaves; i++)
            filho_esq->filhos[i + t_arvore] = filho_dir->filhos[i];
    }

    // atualiza a quantidade de chaves do filho esquerdo
    filho_esq->n_chaves = 2 * t_arvore - 1;
    
    // reorganizando as chaves do nodo atual
    for (int32_t i = indice; i < atual->n_chaves - 1; i++)
        atual->chaves[i] = atual->chaves[i + 1];
    
    // reorganizando os ponteiros de filhos do nodo atual
    for (int32_t i = indice + 1; i < atual->n_chaves; i++)
        atual->filhos[i] = atual->filhos[i + 1];
    
    // atualiza a quantidade de chaves do nodo atual
    atual->n_chaves--;
    
    free(filho_dir->chaves);
    free(filho_dir->filhos);
    free(filho_dir);
}

void EmprestarDaEsquerda(struct nodo *atual, int32_t indice){
    struct nodo *filho = atual->filhos[indice];
    struct nodo *irmao = atual->filhos[indice - 1];

    // desloca as chaves do filho para a direita para abrir espaço para a chave do pai
    for (int32_t i = filho->n_chaves -1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];
    
    // se não for folha, desloca os ponteiros de filhos do filho para a direita
    if (!filho->eh_folha){
        for (int32_t i = filho->n_chaves; i >= 0; i--)
            filho->filhos[i + 1] = filho->filhos[i];
    }

    // a primeira chave do irmão sobe para o pai
    filho->chaves[0] = atual->chaves[indice - 1];

    // o ultimo filho do irmão se torna o primeiro filho do filho
    if (!filho->eh_folha)
        filho->filhos[0] = irmao->filhos[irmao->n_chaves];
    
    // a ultima chave do irmão sobe para o pai
    atual->chaves[indice - 1] = irmao->chaves[irmao->n_chaves - 1]; 

    // atualiza a quantidade de chaves do filho e do irmão
    filho->n_chaves++;
    irmao->n_chaves--;
}   

void EmprestarDaDireita(struct nodo *atual, int32_t indice){
    struct nodo *filho, *irmao;

    filho = atual->filhos[indice];
    irmao = atual->filhos[indice + 1];
    // a chave do pai sobe para o filho
    filho->chaves[filho->n_chaves] = atual->chaves[indice]; 

    // o primeiro filho do irmão se torna o ultimo filho do filho
    if (!filho->eh_folha)
        filho->filhos[filho->n_chaves + 1] = irmao->filhos[0];
    
    // a primeira chave do irmão sobe para o pai
    atual->chaves[indice] = irmao->chaves[0];
    
    for (int32_t i = 1; i < irmao->n_chaves; i++)
        irmao->chaves[i - 1] = irmao->chaves[i];
    
    if (!irmao->eh_folha){
        for (int32_t i = 1; i <= irmao->n_chaves; i++)
            irmao->filhos[i - 1] = irmao->filhos[i];
    }

    // atualiza a quantidade de chaves do filho e do irmão
    filho->n_chaves++;
    irmao->n_chaves--;

}

struct nodo *irmaoImediatoComMaisChaves (struct nodo *atual, int32_t indice, short *direcao){

    if (indice > atual->n_chaves || indice < 0)
        erro ("indice invalido para buscar irmaos");

    if (indice == 0){
        *direcao = 1; // 1 - "Direita"
        return atual->filhos[1];
    }

    // irmao esquerdo possui mais chaves
    else if (indice == atual->n_chaves || atual->filhos[indice - 1]->n_chaves > atual->filhos[indice + 1]->n_chaves){
        *direcao = 0;
        return atual->filhos[indice - 1];
    }

    // irmao direito possui mais chaves
    else{ 
        *direcao = 1;   
        return atual->filhos[indice + 1];
    }
}

bool removerChaveArvoreBrec (struct arvoreB *arvore, struct nodo *atual, int32_t chave){
    int32_t indice, indice_aux, chave_aux;
    struct nodo *aux, *irmao;
    short direcao_irmao; 

    indice = 0;

    // Percorre as chaves até encontrar a posição correta
    while (indice < atual->n_chaves && chave > atual->chaves[indice])
        indice++; 
    
    // chave encontrada no nodo atual
    if (indice < atual->n_chaves && chave == atual->chaves[indice]){
        
        // caso 1 a chave esta na folha ou não existe
        if (atual->eh_folha){
            // remove a chave do nodo e redimenciona o vetor
            for (int32_t i = indice; i < atual->n_chaves - 1; i++)
                atual->chaves[i] = atual->chaves[i + 1];

            atual->n_chaves--; // att a qtd de chaves no nodo
            return true;
        }

        // caso 2 a chave esta em um nodo interno
        else{

            // 2a - filho esquerdo possui t-chaves
            if (atual->filhos[indice]->n_chaves >= arvore->t_arvore){
                
                aux = encontrarPred (atual->filhos[indice], &indice_aux);
                chave_aux = aux->chaves[indice_aux];
                atual->chaves[indice] = chave_aux;
                return removerChaveArvoreBrec (arvore, atual->filhos[indice], chave_aux);
            }
            
            // 2b - filho direito possui t-chaves
            else if (atual->filhos[indice + 1]->n_chaves >= arvore->t_arvore){

                aux = encontrarSuc (atual->filhos[indice + 1], &indice_aux);
                chave_aux = aux->chaves[indice_aux];
                atual->chaves[indice] = chave_aux;
                return removerChaveArvoreBrec (arvore, atual->filhos[indice + 1], chave_aux);                    
            }

            // 2c - ambos os filhos tem t-1 chaves
            else{
                // mescla os filhos e desce para o filho esquerdo, que agora tem 2t-1 chaves
                mesclarFilhos (atual, indice, arvore->t_arvore);

                if (atual == arvore->raiz && atual->n_chaves == 0){

                    // apaga a raiz antiga e atualiza a raiz para o filho mesclado
                    arvore->raiz = atual->filhos[0];

                    free (atual->chaves);
                    free (atual->filhos);
                    free (atual);
                    return removerChaveArvoreBrec (arvore, arvore->raiz, chave);
                }
            
                return removerChaveArvoreBrec (arvore, atual->filhos[indice], chave);;
            }
        }
    }

    // caso 3 a chave não esta no nodo atual, mas pode estar em um filho
    else{
        // 3a - Chegamos na folha e não achamos a chave
        if (atual->eh_folha)
            return false;
        
        // 3b - garantir que o no que vamos descer tem pelo menos t-chaves
        if (atual->filhos[indice]->n_chaves < arvore->t_arvore){
            irmao = irmaoImediatoComMaisChaves(atual, indice, &direcao_irmao);

            if (irmao->n_chaves >= arvore->t_arvore){
                
                if (direcao_irmao ==  0)
                    EmprestarDaEsquerda (atual, indice);

                else
                    EmprestarDaDireita (atual, indice);
            }

    
            // 3c - nenhum irmão possui chaves suficientes
            else{
                // mescla com o filho esquerdo
                if (direcao_irmao == 0){
                    mesclarFilhos (atual, indice - 1, arvore->t_arvore);
                    indice--; // para o return final da funcao descer pelo no corretp
                }

                else // mescla com o filho direito 
                    mesclarFilhos (atual, indice, arvore->t_arvore);
                
                if (atual->n_chaves < 1){ // atual era raiz e ficou sem nodo
                    
                    arvore->raiz = atual->filhos[0];
                    free (atual->chaves);
                    free (atual->filhos);
                    free (atual);

                    return removerChaveArvoreBrec (arvore, arvore->raiz, chave);
                } 
            }
        }
    }

    // atual->filhos[indice] possui t ou mais chaves
    return removerChaveArvoreBrec (arvore, atual->filhos[indice], chave);
}

bool removerChaveArvoreB (struct arvoreB* arvore, int32_t chave){
    
    if (!arvore)
        erro ("Arvore não existe");

    return removerChaveArvoreBrec (arvore, arvore->raiz, chave);
}