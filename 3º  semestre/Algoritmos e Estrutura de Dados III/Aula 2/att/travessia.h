#ifndef TRAVESSIA_H
#define TRAVESSIA_H

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Faz a travessia em pré-ordem
void pre_order(pessoa *raiz);

// Faz a travessia em-ordem
void em_ordem(pessoa *raiz);

// Faz a travessia em pos-ordem
void pos_order(pessoa *raiz);

#endif