#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

// FUNCAO DE DESENHO DO PERSONAGEM
void desenhar_personagem(float posicaoX, float posicaoY,
    float larguraPersonagem, float alturaPersonagem,
    float cameraX, bool personagemVisivel);

#endif