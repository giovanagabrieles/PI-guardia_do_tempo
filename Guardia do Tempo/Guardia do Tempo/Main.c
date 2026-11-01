#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

//PLATAFORMAS
typedef struct {
    float x;
    float y;
    float largura;
    float altura;
} plataforma;


int main() {
    // INICIALIZACAO DO ALLEGRO E ADDON PRIMITIVAS
    al_init();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(600, 600); // TELA

    float posicaoX = 50;
    float posicaoY = 300;
    float velocidadeHorizontal = 4;
    float velocidadeVertical = 0;
    float gravidade = 0.5;
    float larguraPersonagem = 30;
    float alturaPersonagem = 40;


    //PLATAFORMAS
    plataforma plataformas[3] = {
        {0, 500, 150, 15},
        {250, 425, 350, 15},
        {100, 350, 350, 15}
    };


    bool estaNoChao = false; //INDICA SE O PERSONAGEM ESTA NO CHAO

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();

    al_install_keyboard();     //INICIALIZACAO DO TECLADO

    al_register_event_source(fila, al_get_keyboard_event_source());     //REGISTRO DA FONTE DE EVENTOS DO TECLADO

    al_register_event_source(fila, al_get_display_event_source(display));     //REGISTRO DOS EVENTOS DA JANELA


    bool jogoEmExecucao = true; 

    //LOOP PRINCIPAL
    while (jogoEmExecucao) {

        ALLEGRO_EVENT evento;

        //LEITURA DOS EVENTOS
        while (al_get_next_event(fila, &evento)) {

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                jogoEmExecucao = false;
            }
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {

                if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && estaNoChao ||
                    evento.keyboard.keycode == ALLEGRO_KEY_UP && estaNoChao ||
                    evento.keyboard.keycode == ALLEGRO_KEY_W && estaNoChao) {
                    velocidadeVertical = -10;
                    estaNoChao = false;
                }

                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    jogoEmExecucao = false;
                }
            }
        }
        //MOVIMENTACAO PERSONAGEM
        ALLEGRO_KEYBOARD_STATE teclado;
        al_get_keyboard_state(&teclado);

        if (al_key_down(&teclado, ALLEGRO_KEY_A) || al_key_down(&teclado, ALLEGRO_KEY_LEFT)) {
            posicaoX -= velocidadeHorizontal;
        }

        if (al_key_down(&teclado, ALLEGRO_KEY_D) || al_key_down(&teclado, ALLEGRO_KEY_RIGHT)) {
            posicaoX += velocidadeHorizontal;
        }

        //GRAVIDADE E MOVIMENTO VERTICAL
        float posicaoYAnterior = posicaoY;

        estaNoChao = false;

        velocidadeVertical += gravidade;
        posicaoY += velocidadeVertical;

        //COLISAO COM PLATAFORMAS
        for (int i = 0; i < 3; i++) {
            if (velocidadeVertical > 0 &&
                posicaoYAnterior + alturaPersonagem <= plataformas[i].y &&
                posicaoY + alturaPersonagem >= plataformas[i].y &&
                posicaoX + alturaPersonagem > plataformas[i].x &&
                posicaoX < plataformas[i].x + plataformas[i].largura) {

                posicaoY = plataformas[i].y - alturaPersonagem;
                velocidadeVertical = 0;
                estaNoChao = true;

            }

        }

        //COLISAO COM CHAO
        if (posicaoY + alturaPersonagem >= 580) {
            posicaoY = 580 - alturaPersonagem;
            velocidadeVertical = 0;
            estaNoChao = true;
        }

        //LIMITES DA TELA
        if (posicaoX < 0) {
            posicaoX = 0;
        }

        if (posicaoX + larguraPersonagem > 600) {
            posicaoX = 600 - larguraPersonagem;
        }

        //DESENHO CENARIO E PERSONAGEM
        al_clear_to_color(al_map_rgb(5, 207, 250));


        al_draw_line(0, 580, 600, 580, al_map_rgb(255, 255, 255), 2.0);


        //DESENHO DAS PLATAFORMAS
        for (int i = 0; i < 3; i++) {
            al_draw_filled_rectangle(
                plataformas[i].x,
                plataformas[i].y,
                plataformas[i].x + plataformas[i].largura,
                plataformas[i].y + plataformas[i].altura,
                al_map_rgb(31, 150, 12)

            );
        }

        al_draw_filled_rectangle(
            posicaoX,
            posicaoY,
            posicaoX + larguraPersonagem,
            posicaoY + alturaPersonagem,
            al_map_rgb(237, 61, 17));


        al_flip_display();


        al_rest(0.01); //ATUALIZAÇAO DO JOGO
    }

    //FINALIZAÇAO E LIBERAÇAO DE RECURSOS
    al_destroy_event_queue(fila);
    al_destroy_display(display);

    return 0;
}