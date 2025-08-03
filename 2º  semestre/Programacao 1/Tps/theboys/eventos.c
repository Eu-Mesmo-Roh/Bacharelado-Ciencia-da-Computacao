#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "eventos.h"
#include "entidades.h"
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

// Calcula a distância entre dois pontos (usando fórmula da distância euclidiana)
int calculo_da_distancia(struct local_t *coordenada, struct local_t *prox_coordenada)
{
    int distancia, x, y;

    x = (prox_coordenada->x - coordenada->x) * (prox_coordenada->x - coordenada->x);
    y = (prox_coordenada->y - coordenada->y) * (prox_coordenada->y - coordenada->y);

    distancia = sqrt(x + y);

    return distancia;
}

// Combina as habilidades dos heróis presentes em uma base
struct cjto_t *conjunto_habilidades_da_base(struct mundo_t *mundo, int id_base)
{
    struct cjto_t *conj_habilidades, *temp;
    int i;

    // Cria um conjunto vazio
    conj_habilidades = cjto_cria(1);
    if (!conj_habilidades)
        return NULL;

    i = 0;
    // Itera sobre todos os heróis do mundo
    while (i < mundo->NHerois)
    {
        // Verifica se o herói está presente na base
        if (cjto_pertence(mundo->bases[id_base].presentes, i))
        {
            // Faz a união das habilidades do herói com o conjunto de habilidades
            temp = cjto_uniao(mundo->herois[i].habilidades, conj_habilidades);
            cjto_destroi(conj_habilidades);      // Libera o conjunto antigo
            conj_habilidades = cjto_copia(temp); // Copia o resultado da união
            cjto_destroi(temp);                  // Libera o conjunto temporário
        }

        i++;
    }
    return conj_habilidades;
}

// Troca os elementos dos vetores em duas posições específicas
void troca(int vetor[], int vetor_id[], int a, int b)
{
    int aux, aux_id;
    aux = vetor[a];
    aux_id = vetor_id[a];

    vetor[a] = vetor[b];
    vetor_id[a] = vetor_id[b];

    vetor[b] = aux;
    vetor_id[b] = aux_id;
}

// Ordena os vetores usando o algoritmo Selection Sort
void ordena_vetor(int vetor[], int vetor_id[], int n)
{
    int i, j, min;
    // Percorre o vetor até a penúltima posição
    for (i = 0; i < n - 1; i++)
    {
        min = i; // Assume o índice atual como mínimo

        // Busca o menor elemento no restante do vetor
        for (j = i + 1; j < n; j++)
            if (vetor[j] < vetor[min])
                min = j;

        // Troca os elementos de posição
        troca(vetor, vetor_id, i, min);
    }
}

// Valida os ponteiros do mundo e da fila de prioridade
int ponteiros_invalidos(struct mundo_t *mundo, struct fprio_t *fprio)
{
    if (!mundo)
    {
        printf("Erro no ponteiro mundo");
        return 1;
    }
    if (!mundo->missoes)
    {
        printf("Erro no ponteiro mundo->missoes");
        return 1;
    }
    if (!mundo->bases)
    {
        printf("Erro no ponteiro mundo->bases");
        return 1;
    }
    if (!mundo->herois)
    {
        printf("Erro no ponteiro mundo->herois");
        return 1;
    }
    if (!fprio)
    {
        printf("Erro no ponteiro fprio");
        return 1;
    }

    return 0;
}

// Verifica o status de um herói: retorna 0 se estiver morto, 1 se estiver vivo
int heroi_V_ou_M(struct mundo_t *mundo, int id_heroi)
{
    if (mundo->herois[id_heroi].status == 0)
        return 0;

    return 1;
}

// Cria um evento e inicializa seus campos
struct evento_t *cria_evento(int id_heroi, int id_base, int id_missao, int id_base_dest)
{
    struct evento_t *ev;
    ev = malloc(sizeof(struct evento_t));

    if (!ev)
    {
        free(ev);
        return NULL;
    }

    ev->id_heroi = id_heroi;
    ev->id_base = id_base;
    ev->id_missao = id_missao;
    ev->id_base_dest = id_base_dest;

    return ev;
}

/********************** EVENTOS **********************/

// Gerencia o evento de chegada de um herói a uma base
void evento_chega(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    bool espera = false;

    // Verifica se o herói está ativo
    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    // Cria um novo evento
    evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        free(evento);
        printf("ERRO    Não alocou evento chega\n");
        return;
    }

    // Atualiza a base do herói
    mundo->herois[id_heroi].base = id_base;

    // Avalia se o herói deve esperar ou desistir
    if (cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao && lista_tamanho(mundo->bases[id_base].espera) == 0)
        espera = 1; // Há vagas na base e fila vazia
    else if (mundo->herois[id_heroi].paciencia > (10 * lista_tamanho(mundo->bases[id_base].espera)))
        espera = 1; // Paciência do herói suporta a fila
    else
        espera = 0; // O herói desiste

    // Imprime o evento de chegada
    printf("%6d: CHEGA  HEROI %2d ", tempo, id_heroi);
    printf("BASE %d (%2d/%2d) ", id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);

    // Insere o evento na fila de prioridade
    if (espera)
    {
        fprio_insere(fprio, evento, EVENT_ESPERA, tempo);
        printf("ESPERA");
    }
    else
    {
        fprio_insere(fprio, evento, EVENT_DESISTE, tempo);
        printf("DESISTE");
    }
    printf("\n");
}

void evento_espera(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int tamanho_fila;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_vazio, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento espera\n");
        return;
    }

    // adiciona o heroi na fila de espera da base
    lista_insere(mundo->bases[id_base].espera, id_heroi, fim_da_lista);

    // Atualiza o tamanho máximo da fila, se necessário
    tamanho_fila = lista_tamanho(mundo->bases[id_base].espera);
    if (tamanho_fila > mundo->bases[id_base].fila_max)
        mundo->bases[id_base].fila_max = tamanho_fila;

    // Imprime o evento de espera do herói
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes));

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_desiste(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento desiste\n");
        return;
    }

    // Imprime o evento de desistência do herói
    printf("%6d: DESISTE HEROI %2d BASE %d \n", tempo, id_heroi, id_base);

    fprio_insere(fprio, evento, EVENT_VIAJA, tempo);
}

void evento_avisa(struct mundo_t *mundo, int tempo, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int h, id_heroi;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    // Imprime o status atual da base
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d)\n", tempo, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);

    // enquanto ouver vaga na base e houver herois esperando na fila
    while ((cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao) && (lista_tamanho(mundo->bases[id_base].espera) > 0))
    {
        // retira o primeiro heroi da fila da base, armazena o id do heroi
        h = lista_retira(mundo->bases[id_base].espera, &id_heroi, inicio_da_lista);

        // Imprime a mensagem de admissão do herói na base
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, id_base, id_heroi);

        // adiciona o heroi ao conjunto de herois presentes na base
        cjto_insere(mundo->bases[id_base].presentes, h);

        // Cria um evento de entrada para o herói
        evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
        if (!evento)
        {
            printf("ERRO    Não alocou evento avisa\n");
            return;
        }

        fprio_insere(fprio, evento, EVENT_ENTRA, tempo);
    }
}

void evento_entra(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int tempo_permanencia_base;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento entra\n");
        return;
    }

    // calcula o tempo em que o heroi permanece na base
    tempo_permanencia_base = (mundo->herois[id_heroi].paciencia * gera_valor_aleatorio(1, 20) + 15);

    // Imprime a mensagem de entrada do herói e seu tempo de saída
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao, tempo + tempo_permanencia_base);

    // cria proximo evento que acontecera em (tempo + tempo_permanencia_base)
    fprio_insere(fprio, evento, EVENT_SAI, tempo + tempo_permanencia_base);
}

void evento_sai(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento1, *evento2;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    // Cria dois eventos: um para o herói viajar e outro para avisar sobre a saida
    evento1 = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    evento2 = cria_evento(id_vazio, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento1 || !evento2)
    {
        printf("ERRO    Não alocou evento sai\n");
        return;
    }

    // Imprime a mensagem de saida do heroi
    printf("%6d: SAI  HEROI %2d BASE %d (%2d/%2d)\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);

    // Remove o herói do conjunto de heróis presentes na base
    cjto_retira(mundo->bases[id_base].presentes, id_heroi);

    // Reseta a base do herói para "vazio"
    mundo->herois[id_heroi].base = id_vazio;

    // insere na fila de prioridade o evento viaja
    fprio_insere(fprio, evento1, EVENT_VIAJA, tempo);

    // insere na fila de prioridade o evento avisa
    fprio_insere(fprio, evento2, EVENT_AVISA, tempo);
}

void evento_viaja(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int distancia, duracao, base_dest;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    // escolhe uma base destino aleatória
    base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);

    // Garante que a base destino não seja a mesma base atual
    while (id_base == base_dest)
    {
        base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);
    }

    evento = cria_evento(id_heroi, base_dest, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento viaja\n");
        return;
    }

    // calcula a distancia entre a base de origem e a base de destino
    distancia = calculo_da_distancia(mundo->bases[id_base].coordenadas, mundo->bases[base_dest].coordenadas);

    // Calcula o tempo de viagem com base na velocidade do herói
    duracao = distancia / mundo->herois[id_heroi].velocidade;

    // Imprime a mensagem de viajem do heroi
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, id_heroi, mundo->bases[id_base].id, base_dest, distancia, mundo->herois[id_heroi].velocidade, tempo + duracao);

    fprio_insere(fprio, evento, EVENT_CHEGA, tempo + duracao);
}

void evento_morre(struct mundo_t *mundo, int tempo, int id_heroi, int id_missao, struct fprio_t *fprio)
{
    struct evento_t *evento;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_vazio, mundo->herois[id_heroi].base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento morre\n");
        return;
    }

    // Remove o herói do conjunto de presentes na base
    cjto_retira(mundo->bases[mundo->herois[id_heroi].base].presentes, id_heroi);

    // Reseta a base do herói e altera seu status para "morto"
    mundo->herois[id_heroi].base = -10;
    mundo->herois[id_heroi].status = 0;

    // Imprime a mensagem de morte do heroi
    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", tempo, id_heroi, id_missao);

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_missao(struct mundo_t *mundo, int tempo, int id_missao, struct fprio_t *fprio)
{
    struct evento_t *evento1, *evento2;
    struct cjto_t *habilidades_base, *aux_cjto;
    int id_base[mundo->NBases], distancia[mundo->NBases];
    int i, j, base_apta, risco;

    evento1 = cria_evento(id_vazio, id_vazio, id_missao, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento1)
    {
        free(evento1);
        printf("ERRO    Não Alocou evento missao");
        return;
    }

    // Incrementa o contador de tentativas da missão
    mundo->missoes[id_missao].tentativas++;

    // Imprime a mensagem de requisitos da missão
    printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, id_missao, mundo->missoes[id_missao].tentativas);
    printf("[ ");
    cjto_imprime(mundo->missoes[id_missao].habilidades);
    printf(" ]\n");
    ;

    // Calcula a distância das bases até o local da missão
    for (i = 0; i < mundo->NBases; i++)
    {
        distancia[i] = calculo_da_distancia(mundo->bases[i].coordenadas, mundo->missoes[id_missao].coordenadas);
        id_base[i] = i;
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", tempo, id_missao, i, distancia[i]);
        cjto_imprime(mundo->bases[i].presentes);
        printf(" ]\n");
        for (j = 0; j < mundo->NHerois; j++)
        {
            if (i == mundo->herois[j].base)
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", tempo, id_missao, j);
                cjto_imprime(mundo->herois[j].habilidades);
                printf(" ]\n");
            }
        }
    }

    // Ordena as bases pela distância
    ordena_vetor(distancia, id_base, mundo->NBases);

    base_apta = -1;

    // Verifica se há uma base apta a cumprir a missão
    for (i = 0; i < mundo->NBases; i++)
    {
        habilidades_base = conjunto_habilidades_da_base(mundo, id_base[i]);
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", tempo, id_missao, i);
        cjto_imprime(habilidades_base);
        printf(" ]\n");

        // caso o conjuto de habilidades da base tenha todas as habilidades da missao, ela pode ser cumprida
        if (cjto_contem(mundo->missoes[id_missao].habilidades, habilidades_base) || cjto_iguais(habilidades_base, mundo->missoes[id_missao].habilidades))
        {
            base_apta = id_base[i];
            aux_cjto = cjto_copia(habilidades_base);
            cjto_destroi(habilidades_base);
            break;
        }
        cjto_destroi(habilidades_base);
    }

    if (base_apta >= 0)
    {
        free(evento1);
        // Imprime a mensagem de missão cumprida
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", tempo, id_missao, id_base[i]);
        cjto_imprime(aux_cjto);
        printf("\n");

        cjto_destroi(aux_cjto);

        // Incrementa a quantidade de missões realizadas pela base e troca o estatus da missão para realizada.
        mundo->bases[base_apta].qtd_missoes++;
        mundo->missoes[id_missao].realizadas = 1;

        // incrementa a experiencia dos herois presentes na base ou mata o heroi
        for (j = 0; j < mundo->NHerois; j++)
        {
            if (base_apta == mundo->herois[j].base)
            {
                risco = mundo->missoes[id_missao].perigo / (mundo->herois[j].paciencia + mundo->herois[j].experiencia + 1.0);
                if (risco > gera_valor_aleatorio(0, 30))
                {
                    evento2 = cria_evento(mundo->herois[j].id, base_apta, id_missao, id_vazio);
                    if (!evento2)
                    {
                        free(evento2);
                        printf("ERRO    evento morre");
                        return;
                    }
                    fprio_insere(fprio, evento2, EVENT_MORRE, tempo);
                }
                mundo->herois[j].experiencia++;
            }
        }
    }
    else
    {
        // Reagenda a missão para o futuro
        printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, id_missao);
        fprio_insere(fprio, evento1, EVENT_MISSAO, tempo + 60 * 24);
    }
}

void evento_fim(struct mundo_t *mundo, struct fprio_t *fprio)
{
    int missoes_cumpridas, tentativas, i, maior_tent, menor_tent, soma_mortos;
    double taxa_conclusao, taxa_m_media, taxa_mortalidade;
    missoes_cumpridas = 0;
    tentativas = 0;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    // Imprime o tempo final da simulação
    printf("%d: FIM\n", mundo->tempo);
    soma_mortos = 0;

    // imprime o status final de cada heroi
    for (i = 0; i < mundo->NHerois; i++)
    {
        // print do codigo
        if (heroi_V_ou_M(mundo, mundo->herois[i].id))
            printf("HEROI %2d VIVO  PAC %3d", i, mundo->herois[i].paciencia);
        else
        {
            printf("HEROI %2d MORTO  PAC %3d", i, mundo->herois[i].paciencia);
            soma_mortos++;
        }

        printf(" VEL %4d EXP %4d HABS ", mundo->herois[i].velocidade, mundo->herois[i].experiencia);
        printf("[ ");
        cjto_imprime(mundo->herois[i].habilidades); // Imprime o conjunto de habilidades do herói
        printf(" ]\n");
    }

    // imprime o status final de cada base
    for (i = 0; i < mundo->NBases; i++)
    {
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, mundo->bases[i].lotacao, mundo->bases[i].fila_max, mundo->bases[i].qtd_missoes);
    }

    // Inicializa os valores de menor e maior tentativas de missões
    menor_tent = T_FIM_DO_MUNDO;
    maior_tent = 0;

    // Calcula as estatísticas de missões cumpridas e tentativas
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tentativas = mundo->missoes[i].tentativas + tentativas;
        missoes_cumpridas += mundo->missoes[i].realizadas;
        if (menor_tent > mundo->missoes[i].tentativas)
            menor_tent = mundo->missoes[i].tentativas;
        if (maior_tent < mundo->missoes[i].tentativas)
            maior_tent = mundo->missoes[i].tentativas;
    }

    // Calcula a taxa de conclusão de missões
    taxa_conclusao = ((double)missoes_cumpridas / (double)mundo->NMissoes) * 100;

    // Imprime quantos eventoas foram tratados durante a execução do programa
    printf("EVENTOS TRATADOS: %d\n", mundo->eventos_tratados);

    // Imprime estatísticas sobre missões
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cumpridas, mundo->NMissoes, (double)taxa_conclusao);

    // Calcula a média de tentativas por missão
    taxa_m_media = (double)tentativas / (double)mundo->NMissoes;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", menor_tent, maior_tent, taxa_m_media);

    // Calcula e imprime a taxa de mortalidade
    taxa_mortalidade = ((double)soma_mortos / (double)mundo->NHerois) * 100;
    printf("TAXA MORTALIDADE: %.1f%%\n", (double)taxa_mortalidade);
}

void evento_inicio(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;
    int base, tempo, i;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    // Gera o evento de chegada inicial para cada herói
    for (i = 0; i < mundo->NHerois; i++)
    {
        base = gera_valor_aleatorio(0, mundo->NBases - 1); // Base aleatória
        tempo = gera_valor_aleatorio(0, 4320);             // Tempo entre 0 e 3 dias (60 * 24 * 3)
        if (!(evento = cria_evento(i, base, id_vazio, id_vazio)))
            return;
        fprio_insere(fprio, evento, EVENT_CHEGA, tempo);
    }

    // Gera o evento de missão inicial para cada missão
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tempo = gera_valor_aleatorio(4320, T_FIM_DO_MUNDO - 1); // Missões ocorrem após 3 dias até o fim
        if (!(evento = cria_evento(id_vazio, id_vazio, i, id_vazio)))
            return;
        fprio_insere(fprio, evento, EVENT_MISSAO, tempo);
    }

    // Insere o evento "fim do mundo"
    if (!(evento = cria_evento(id_vazio, id_vazio, id_vazio, id_vazio)))
        return;
    fprio_insere(fprio, evento, EVENT_FIM, T_FIM_DO_MUNDO);
}

void processa_evento(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;
    int tipo, tempo;

    // Retira o primeiro evento da fila de prioridade
    evento = fprio_retira(fprio, &tipo, &tempo);
    if (!evento)
    {
        printf("ERRO    Erro no switch");
        return;
    }

    // Processa eventos enquanto o tempo não ultrapassa o fim do mundo
    while (evento && tempo <= T_FIM_DO_MUNDO)
    {
        mundo->eventos_tratados++; // Incrementa o número de eventos tratados

        // Switch para determinar o tipo do evento e chamar a função correspondente
        switch (tipo)
        {
        case EVENT_CHEGA:
            evento_chega(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_ESPERA:
            evento_espera(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_DESISTE:
            evento_desiste(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_AVISA:
            evento_avisa(mundo, tempo, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_ENTRA:
            evento_entra(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_SAI:
            evento_sai(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_VIAJA:
            evento_viaja(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            free(evento);
            break;
        case EVENT_MORRE:
            evento_morre(mundo, tempo, evento->id_heroi, evento->id_missao, fprio);
            free(evento);
            break;
        case EVENT_MISSAO:
            evento_missao(mundo, tempo, evento->id_missao, fprio);
            free(evento);
            break;
        case EVENT_FIM:
            evento_fim(mundo, fprio);
            free(evento);
            return;
        }
        evento = NULL;                               // Libera o evento processado
        evento = fprio_retira(fprio, &tipo, &tempo); // Retira o próximo evento
    }
    if (evento)
        free(evento); // Garante que a memória é liberada se o evento for nulo
}
