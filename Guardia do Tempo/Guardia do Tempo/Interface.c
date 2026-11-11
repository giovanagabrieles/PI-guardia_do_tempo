#include "Interface.h"
#include <allegro5/allegro_primitives.h>

// DESENHA OS BOTOES DO MENU E DE OUTRAS TELAS
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

// VERIFICA SE O CLIQUE ACONTECEU DENTRO DO BOTAO
bool clicou_botao(int mx, int my, float x, float y,
    float largura, float altura)
{
    return mx >= x && mx <= x + largura &&
        my >= y && my <= y + altura;
}

// DESENHA A TELA DE MENU PRINCIPAL
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

// DESENHA A TELA DE CARREGAR JOGO
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

// DESENHA A TELA DE PAUSA
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

// DESENHA A TELA DE DERROTA
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

// DESENHA A TELA DE VITORIA
void desenhar_tela_vitoria(
    ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte,
    float botaoX,
    float botaoLargura,
    float botaoAltura,
    int pontuacao,
    int fragmentosTemporais,
    int vidas
) {
    al_clear_to_color(al_map_rgb(20, 30, 55));

    al_draw_text(titulo, al_map_rgb(255, 220, 0),
        640, 90, ALLEGRO_ALIGN_CENTER, "PARABENS!");

    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        640, 155, ALLEGRO_ALIGN_CENTER,
        "Voce completou toda a jornada!");

    al_draw_textf(fonte, al_map_rgb(255, 255, 255),
        640, 280, ALLEGRO_ALIGN_CENTER,
        "Pontuacao final: %d", pontuacao);

    al_draw_textf(fonte, al_map_rgb(0, 180, 255),
        640, 330, ALLEGRO_ALIGN_CENTER,
        "Fragmentos temporais: %d/2", fragmentosTemporais);

    al_draw_textf(fonte, al_map_rgb(255, 100, 100),
        640, 380, ALLEGRO_ALIGN_CENTER,
        "Vidas restantes: %d", vidas);

    desenhar_botao(botaoX, 500, botaoLargura, botaoAltura,
        "JOGAR NOVAMENTE", fonte, al_map_rgb(50, 130, 90));

    desenhar_botao(botaoX, 590, botaoLargura, botaoAltura,
        "VOLTAR AO MENU", fonte, al_map_rgb(90, 90, 110));
}
// DESENHA A TELA DE JORNADA INCOMPLETA
void desenhar_tela_jornada_incompleta(
    ALLEGRO_FONT* titulo,
    ALLEGRO_FONT* fonte,
    float botaoX,
    float botaoLargura,
    float botaoAltura,
    int pontuacao,
    int fragmentosTemporais,
    int vidas
) {
    al_clear_to_color(al_map_rgb(35, 25, 45));

    al_draw_text(titulo, al_map_rgb(255, 150, 100),
        640, 100, ALLEGRO_ALIGN_CENTER,
        "JORNADA INCOMPLETA");

    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        640, 190, ALLEGRO_ALIGN_CENTER,
        "Voce nao cumpriu todos os requisitos.");

    al_draw_textf(fonte, al_map_rgb(255, 255, 255),
        640, 280, ALLEGRO_ALIGN_CENTER,
        "Pontuacao: %d", pontuacao);

    al_draw_textf(fonte, al_map_rgb(0, 180, 255),
        640, 330, ALLEGRO_ALIGN_CENTER,
        "Fragmentos temporais: %d/2", fragmentosTemporais);

    al_draw_textf(fonte, al_map_rgb(255, 100, 100),
        640, 380, ALLEGRO_ALIGN_CENTER,
        "Vidas restantes: %d", vidas);

    desenhar_botao(botaoX, 500, botaoLargura, botaoAltura,
        "TENTAR NOVAMENTE", fonte, al_map_rgb(50, 130, 90));

    desenhar_botao(botaoX, 590, botaoLargura, botaoAltura,
        "VOLTAR AO MENU", fonte, al_map_rgb(90, 90, 110));
}
