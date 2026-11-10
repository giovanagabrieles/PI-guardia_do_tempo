#include "Cenario.h"
#include <allegro5/allegro_primitives.h>

// Desenha todas as plataformas do cenario
void desenhar_plataformas(plataforma plataformas[],
    int qtdPlataformas, float cameraX)
{
    for (int i = 0; i < qtdPlataformas; i++) {
        al_draw_filled_rectangle(
            plataformas[i].x - cameraX, plataformas[i].y,
            plataformas[i].x + plataformas[i].largura - cameraX,
            plataformas[i].y + plataformas[i].altura,
            al_map_rgb(31, 150, 12));
    }
}

// Desenha todos os obstaculos do cenario
void desenhar_obstaculos(obstaculo obstaculos[],
    int qtdObstaculos, float cameraX)
{
    for (int i = 0; i < qtdObstaculos; i++) {
        al_draw_filled_rectangle(
            obstaculos[i].x - cameraX, obstaculos[i].y,
            obstaculos[i].x + obstaculos[i].largura - cameraX,
            obstaculos[i].y + obstaculos[i].altura,
            al_map_rgb(200, 0, 0));
    }
}