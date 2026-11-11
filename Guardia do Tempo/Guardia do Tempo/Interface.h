#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

// BOTOES
void desenhar_botao(float x, float y, float largura, float altura,
    const char texto[], ALLEGRO_FONT* fonte, ALLEGRO_COLOR cor);

bool clicou_botao(int mx, int my, float x, float y,
    float largura, float altura);

// TELAS DO JOGO
void desenhar_tela_menu(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura);

void desenhar_tela_carregar_jogo(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura);

void desenhar_tela_pausa(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura);

void desenhar_tela_derrota(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura);

void desenhar_tela_vitoria(
    ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte,
    float botaoX,
    float botaoLargura,
    float botaoAltura,
    int pontuacao,
    int fragmentosTemporais,
    int vidas
);

void desenhar_tela_jornada_incompleta(
    ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte,
    float botaoX,
    float botaoLargura,
    float botaoAltura,
    int pontuacao,
    int fragmentosTemporais,
    int vidas
);

#endif
