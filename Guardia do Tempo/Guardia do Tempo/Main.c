#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

//PLATAFORMAS
typedef struct {
    float x;
    float y;
    float largura;
    float altura;
} plataforma;

//OBSTACULOS
typedef struct {
    float x;
    float y;
    float largura;
    float altura;
} obstaculo;


int main() {
    // INICIALIZACAO DO ALLEGRO E ADDON PRIMITIVAS
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();

    ALLEGRO_DISPLAY* display = al_create_display(1280, 720); // TELA
    ALLEGRO_FONT* fonte = al_create_builtin_font();

    float posicaoX = 50;
    float posicaoY = 300;
    float velocidadeHorizontal = 4;
    float velocidadeVertical = 0;
    float gravidade = 0.5;
    float larguraPersonagem = 30;
    float alturaPersonagem = 40;

    //CAMERA E CENARIO
    float cameraX = 0;
    float larguraCenario = 2400;

    int vidas = 5;
    bool invulneravel = false;
    float tempoInvulneravel = 0;


    //PLATAFORMAS
    plataforma plataformas[6] = {
        {0, 500, 150, 15},
        {200, 425, 350, 15},
        {500, 500, 300, 15},
        {800, 425, 250, 15},
        {1100, 350, 300, 15},
        {1450, 425, 350, 15}
    };
        int qtdPlataformas = sizeof(plataformas) / sizeof(plataformas[0]);


    //OBSTACULOS
    obstaculo obstaculos[1] = {
        {650, 650, 40, 30}
    };
        int qtdObstaculos = sizeof(obstaculos) / sizeof(obstaculos[0]);


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

        //TEMPORIZADOR DE INVUNERABILIDADE
        if (invulneravel) {
            tempoInvulneravel += 0.01;

            if (tempoInvulneravel >= 1.0) {
                invulneravel = false;
                tempoInvulneravel = 0;
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
        for (int i = 0; i < qtdPlataformas; i++) {
            if (velocidadeVertical > 0 &&
                posicaoYAnterior + alturaPersonagem <= plataformas[i].y &&
                posicaoY + alturaPersonagem >= plataformas[i].y &&
                posicaoX + larguraPersonagem > plataformas[i].x &&
                posicaoX < plataformas[i].x + plataformas[i].largura) {

                posicaoY = plataformas[i].y - alturaPersonagem;
                velocidadeVertical = 0;
                estaNoChao = true;

            }

        }


        //COLISAO COM CHAO
        if (posicaoY + alturaPersonagem >= 680) {
            posicaoY = 680 - alturaPersonagem;
            velocidadeVertical = 0;
            estaNoChao = true;
        }


        //COLISAO COM OBSTACULO
        for (int i = 0; i < qtdObstaculos; i++) {
            if (!invulneravel &&
                posicaoX + larguraPersonagem > obstaculos[i].x &&
                posicaoX < obstaculos[i].x + obstaculos[i].largura &&
                posicaoY + alturaPersonagem > obstaculos[i].y &&
                posicaoY < obstaculos[i].y + obstaculos[i].altura) {

                vidas--;
                invulneravel = true;
                posicaoX = 50;
                posicaoY = 300;
                velocidadeVertical = 0;
                cameraX = 0;
            }
        }


        //LIMITES DA TELA
        if (posicaoX < 0) {
            posicaoX = 0;
        }

        if (posicaoX + larguraPersonagem > larguraCenario) {
            posicaoX = larguraCenario - larguraPersonagem;
        }


        //MOVIMENTACAO DA CAMERA
        float cameraFoco = posicaoX + larguraPersonagem / 2 - 640;

        if (cameraFoco < 0) {
            cameraFoco = 0;
        }

        if (cameraFoco > larguraCenario - 1280) {
            cameraFoco = larguraCenario - 1280;
        }

        cameraX += (cameraFoco - cameraX) * 0.1;


        //DESENHO CENARIO 
        al_clear_to_color(al_map_rgb(5, 207, 250));


        al_draw_line(-cameraX, 680, larguraCenario - cameraX, 680,
            al_map_rgb(255, 255, 255), 2.0);


        //VIDAS
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 20, 20, 0, "Vidas: %d",vidas);


        //DESENHO DAS PLATAFORMAS
        for (int i = 0; i < qtdPlataformas; i++) {
            al_draw_filled_rectangle(
                plataformas[i].x - cameraX,
                plataformas[i].y,
                plataformas[i].x + plataformas[i].largura - cameraX,
                plataformas[i].y + plataformas[i].altura,
                al_map_rgb(31, 150, 12)

            );
        }


        //DESENHO OBSTACULOS
        for (int i = 0; i < qtdObstaculos; i++) {
            al_draw_filled_rectangle(
                obstaculos[i].x - cameraX,
                obstaculos[i].y,
                obstaculos[i].x + obstaculos[i].largura - cameraX,
                obstaculos[i].y + obstaculos[i].altura,
                al_map_rgb(200, 0, 0)
            );
        }


        //DESENHO PERSONAGEM
        al_draw_filled_rectangle(
            posicaoX - cameraX,
            posicaoY,
            posicaoX + larguraPersonagem - cameraX,
            posicaoY + alturaPersonagem,
            al_map_rgb(237, 61, 17));


        al_flip_display();


        al_rest(0.01); //ATUALIZAÇAO DO JOGO
    }

    //FINALIZAÇAO E LIBERAÇAO DE RECURSOS
    al_destroy_event_queue(fila);
    al_destroy_display(display);
    al_destroy_font(fonte);

    return 0;
}