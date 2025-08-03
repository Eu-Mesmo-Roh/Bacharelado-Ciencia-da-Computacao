#ifndef EVENTOS
#define EVENTOS

#include <math.h>
#include <stdbool.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"

// Definições de cada evento 
#define EVENT_CHEGA 1
#define EVENT_ESPERA 2
#define EVENT_DESISTE 3
#define EVENT_AVISA 4
#define EVENT_ENTRA 5
#define EVENT_SAI 6
#define EVENT_VIAJA 7
#define EVENT_MORRE 8
#define EVENT_MISSAO 9
#define EVENT_FIM 10

//Define para vazio
#define id_vazio -10    // Identificador padrão para valores não utilizados
#define sem_missao = id_vazio   // Identificador indicando a falta de missão

//Define Lista
#define fim_da_lista -1 // fim da lista
#define inicio_da_lista 0 // inicio da lista

// Estrutura que descreve um evento no sistema.
struct evento_t
{
    int id_heroi;   // Identificador do herói associado ao evento.
    int id_base;    // Identificador da base envolvida no evento.
    int id_missao;  // Identificador da missão associada ao evento.
    int id_base_dest;   // Identificador da base de destino, caso o evento envolva 
};

// Cria um Evento 
// Retorno: ponteiro para o evento criado ou NULL se erro 
struct evento_t *cria_evento(int id_heroi, int id_base, int id_missao, int id_base_dest);

// Função responsável por tratar o evento de chegada de um herói a uma base.
// se o heroi não entrar na base ou decidir não espererar dispara o evento DESISTE
void evento_chega(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável por colocar o herói na fila de espera de uma base.
// Dispara o evento AVISA
void evento_espera(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável por tratar a desistência de um herói 
// que estava aguardando em uma base.
// Dispara o evento VIAJA.
void evento_desiste(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável avisar que há vagas disponíveis em uma base
// permitindo que heróis saiam da fila e entrem na base.
// Dispara o evento ENTRA.
void evento_avisa(struct mundo_t *mundo, int tempo, int id_base, struct fprio_t *fprio);

// Função responsável por calcular o tempo de permanencia do heroi na base
// aṕos isso agenda sua saída.
// Dispara o evento SAI.
void evento_entra(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável por Tratar a saída de um herói de uma base
// retirando o heroi da base e escolhendo uma base destino para o mesmo.
// Dispara o evento AVISA pois uma vaga foi liberada.
void evento_sai(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável por Tratar o evento de viagem de um herói entre bases.
// Dispara o evento CHEGA.
void evento_viaja(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

// Função responsável por Tratar a morte de um herói durante uma missão
// retira o heroi da base.
// Dispara o evento Avisa.
void evento_morre(struct mundo_t *mundo, int tempo, int id_heroi, int id_missao,struct fprio_t *fprio);

// Função responsável por Executar uma missão
// verifica se ela foi concluída com sucesso.
// Dispara o evento MORRE caso o heroi morra durante a missao
// Dispara o evento MISSAO caso a missão não possa ser realizada
void evento_missao(struct mundo_t *mundo, int tempo, int id_missao, struct fprio_t *fprio);

// Função responsável por finalizar a execução dos eventos no mundo.
void evento_fim(struct mundo_t *mundo, struct fprio_t *fprio);

// Função responsável por inicializar o processamento de eventos no mundo.
void evento_inicio(struct mundo_t *mundo, struct fprio_t *fprio);

// Função responsável por processar os eventos armazenados na fila de prioridade.
void processa_evento(struct mundo_t *mundo, struct fprio_t *fprio);


#endif
