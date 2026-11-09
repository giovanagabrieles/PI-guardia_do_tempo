#ifndef CENARIO_H
#define CENARIO_H

#include <allegro5/allegro.h>

// PLATAFORMAS
typedef struct {
    float x, y, largura, altura;
} plataforma;

// OBSTACULOS
typedef struct {
    float x, y, largura, altura;
} obstaculo;

// FUNCOES DE DESENHO
void desenhar_plataformas(plataforma plataformas[],
    int qtdPlataformas, float cameraX);

void desenhar_obstaculos(obstaculo obstaculos[],
    int qtdObstaculos, float cameraX);

#endif