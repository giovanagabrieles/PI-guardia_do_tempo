#include "Cenario.h"
#include <allegro5/allegro_primitives.h>

// Desenha todas as plataformas do cenario
void desenhar_plataformas(
    plataforma plataformas[],
    int qtdPlataformas,
    float cameraX,
    int faseAtual
) {
    ALLEGRO_COLOR corPlataforma;

    if (faseAtual == 1) {
        // Egito: areia e pedra
        corPlataforma = al_map_rgb(121, 80, 47);
    }
    else {
        // Grecia: marmore
        corPlataforma = al_map_rgb(210, 210, 200);
    }

    for (int i = 0; i < qtdPlataformas; i++) {
        al_draw_filled_rectangle(
            plataformas[i].x - cameraX,
            plataformas[i].y,
            plataformas[i].x + plataformas[i].largura - cameraX,
            plataformas[i].y + plataformas[i].altura,
            corPlataforma
        );
    }
}

// Desenha todos os obstaculos do cenario
void desenhar_obstaculos(obstaculo obstaculos[],
    int qtdObstaculos, float cameraX, int faseAtual)
{
    ALLEGRO_COLOR corObstaculo;

    if (faseAtual == 1) {
        // Egito: pedra escura
        corObstaculo = al_map_rgb(1181, 45, 36);
    }
    else {
        // Grecia: pedra avermelhada
        corObstaculo = al_map_rgb(150, 75, 55);
    }

    for (int i = 0; i < qtdObstaculos; i++) {
        al_draw_filled_rectangle(
            obstaculos[i].x - cameraX, obstaculos[i].y,
            obstaculos[i].x + obstaculos[i].largura - cameraX,
            obstaculos[i].y + obstaculos[i].altura,
            corObstaculo);
    }
}