#ifndef OBJETOS_H
#define OBJETOS_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

// ESTRUTURA DO OBJETO INTERATIVO
typedef struct {
    float x, y, largura, altura;
    char titulo[60];
    char descricao[300];
} objetoInterativo;

// FUNCOES DE DESENHO
void desenhar_objeto_interativo(objetoInterativo objeto,
    float cameraX, bool pertoDoObjeto, bool objetoEncontrado,
    ALLEGRO_FONT* fonte, int faseAtual);

void desenhar_caixa_informacao(objetoInterativo objeto,
    ALLEGRO_FONT* fonte, bool mostrarInformacao, int faseAtual);

#endif