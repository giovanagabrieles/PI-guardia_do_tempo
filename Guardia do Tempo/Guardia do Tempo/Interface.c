#include "Interface.h"
#include <allegro5/allegro_primitives.h>

// Desenha os botoes do menu e das outras telas
void desenhar_botao(float x, float y, float largura, float altura,
    const char texto[], ALLEGRO_FONT* fonte, ALLEGRO_COLOR cor)
{
    al_draw_filled_rounded_rectangle(
        x, y, x + largura, y + altura, 30, 30, cor);

    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        x + largura / 2,
        y + altura / 2 - al_get_font_line_height(fonte) / 2,
        ALLEGRO_ALIGN_CENTER, texto);
}

// Verifica se o clique aconteceu dentro de um botao
bool clicou_botao(int mx, int my, float x, float y,
    float largura, float altura)
{
    return mx >= x && mx <= x + largura &&
        my >= y && my <= y + altura;
}

// Desenha a tela do menu principal
void desenhar_tela_menu(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura)
{
    al_draw_text(titulo, al_map_rgb(0, 0, 255),
        640, 120, ALLEGRO_ALIGN_CENTER, "GUARDIA DO TEMPO");

    desenhar_botao(botaoX, 250, botaoLargura, botaoAltura,
        "NOVO JOGO", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 340, botaoLargura, botaoAltura,
        "CARREGAR JOGO", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 430, botaoLargura, botaoAltura,
        "SAIR", fonte, al_map_rgb(0, 0, 255));
}

// Desenha a tela de carregar jogo
void desenhar_tela_carregar_jogo(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura)
{
    al_draw_text(titulo, al_map_rgb(0, 0, 255),
        640, 150, ALLEGRO_ALIGN_CENTER, "CARREGAR JOGO");

    desenhar_botao(botaoX, 250, botaoLargura, botaoAltura,
        "JOGO 01", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 340, botaoLargura, botaoAltura,
        "JOGO 02", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 430, botaoLargura, botaoAltura,
        "VOLTAR", fonte, al_map_rgb(0, 0, 255));
}

// Desenha a tela de pausa
void desenhar_tela_pausa(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura)
{
    al_draw_filled_rectangle(0, 0, 1280, 720,
        al_map_rgba(0, 0, 0, 160));

    al_draw_text(titulo, al_map_rgb(255, 255, 255),
        640, 190, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");

    desenhar_botao(botaoX, 300, botaoLargura, botaoAltura,
        "CONTINUAR", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 390, botaoLargura, botaoAltura,
        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
}

// Desenha a tela de derrota
void desenhar_tela_derrota(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura)
{
    al_clear_to_color(al_map_rgb(82, 7, 7));

    al_draw_text(titulo, al_map_rgb(250, 250, 250),
        640, 240, ALLEGRO_ALIGN_CENTER, "FIM DE JOGO!");

    desenhar_botao(botaoX, 350, botaoLargura, botaoAltura,
        "TENTAR NOVAMENTE", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 440, botaoLargura, botaoAltura,
        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
}

// Desenha a tela de vitoria
void desenhar_tela_vitoria(ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte, float botaoX,
    float botaoLargura, float botaoAltura)
{
    al_clear_to_color(al_map_rgb(5, 100, 60));

    al_draw_text(titulo, al_map_rgb(255, 255, 255),
        640, 240, ALLEGRO_ALIGN_CENTER, "FASE CONCLUIDA!");

    desenhar_botao(botaoX, 350, botaoLargura, botaoAltura,
        "JOGAR NOVAMENTE", fonte, al_map_rgb(0, 0, 255));

    desenhar_botao(botaoX, 440, botaoLargura, botaoAltura,
        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
}
