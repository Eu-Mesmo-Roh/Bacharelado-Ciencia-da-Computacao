#ifndef CAMERA_H
#define CAMERA_H


// Estrutura para representar a câmera no jogo
typedef struct camera
{
    float x, y; // Posição da câmera
    float largura_tela, altura_tela; // Tamanho da câmera (área visível)
}camera;

// Estrutura para representar o player no jogo
typedef struct player player;

// Função para iniciar a câmera, definindo suas propriedades iniciais
int iniciar_camera(camera *c, float largura_tela, float altura_tela);

// Função para atualizar a posição da câmera com base na posição do player
int atualizar_camera(camera *c, player *p);

#endif // CAMERA_H