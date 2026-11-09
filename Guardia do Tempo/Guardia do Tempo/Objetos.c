#include "Objetos.h"
#include <allegro5/allegro_primitives.h>

// Desenha o objeto interativo no cenario
void desenhar_objeto_interativo(objetoInterativo objeto,
    float cameraX, bool pertoDoObjeto, ALLEGRO_FONT* fonte)
{
    al_draw_filled_rectangle(
        objeto.x - cameraX, objeto.y,
        objeto.x + objeto.largura - cameraX,
        objeto.y + objeto.altura,
        al_map_rgb(255, 215, 0));

    if (pertoDoObjeto) {
        al_draw_text(
            fonte,
            al_map_rgb(255, 255, 255),
            objeto.x - cameraX,
            objeto.y - 25, 0,
            "Pressione E para interagir");
    }
}

// Desenha a caixa de informacao historica
void desenhar_caixa_informacao(objetoInterativo objeto,
    ALLEGRO_FONT* fonte, bool mostrarInformacao)
{
    if (mostrarInformacao) {

        al_draw_filled_rounded_rectangle(340, 200, 940, 520, 20, 20,
            al_map_rgba(0, 0, 0, 230));

        al_draw_rounded_rectangle(340, 200, 940, 520, 20, 20,
            al_map_rgb(255, 200, 0), 3);

        al_draw_text(fonte, al_map_rgb(255, 220, 0), 640, 240,
            ALLEGRO_ALIGN_CENTER, objeto.titulo);

        al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255),
            390, 300, 500, 30, 0, objeto.descricao);

        al_draw_text(fonte, al_map_rgb(200, 200, 200), 640, 470,
            ALLEGRO_ALIGN_CENTER, "Aperte E para fechar");
    }
}