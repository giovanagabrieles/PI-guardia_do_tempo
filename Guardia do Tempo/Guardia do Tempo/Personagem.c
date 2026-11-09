#include "Personagem.h"
#include <allegro5/allegro_primitives.h>

// Desenha o personagem na tela
void desenhar_personagem(float posicaoX, float posicaoY,
    float larguraPersonagem, float alturaPersonagem,
    float cameraX, bool personagemVisivel)
{
    if (personagemVisivel) {
        al_draw_filled_rectangle(
            posicaoX - cameraX, posicaoY,
            posicaoX + larguraPersonagem - cameraX,
            posicaoY + alturaPersonagem,
            al_map_rgb(237, 61, 17));
    }
}