#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// PLATAFORMAS
typedef struct {
    float x, y, largura, altura;
} plataforma;

// OBSTACULOS
typedef struct {
    float x, y, largura, altura;
} obstaculo;

// ESTADOS DO JOGO
typedef enum {
    MENU,
    CARREGAR_JOGO,
    GAMEPLAY,
    PAUSA,
    DERROTA,
    VITORIA
} EstadoJogo;

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

int main()
{
    // INICIALIZACAO DO ALLEGRO
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // JANELA: mantida em 1280 x 720
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    if (!display) return -1;

    ALLEGRO_FONT* fonte = al_load_ttf_font("arial.ttf", 22, 0);
    ALLEGRO_FONT* titulo = al_load_ttf_font("arial.ttf", 42, 0);
    if (!fonte) fonte = al_create_builtin_font();
    if (!titulo) titulo = fonte;

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // ESTADO E CONTROLE DAS TELAS
    EstadoJogo estado = MENU;
    bool jogoEmExecucao = true;
    bool redesenhar = true;

    // BOTOES
    float botaoX = 490;
    float botaoLargura = 300;
    float botaoAltura = 60;

    // VARIAVEIS DA GAMEPLAY (valores originais da equipe)
    float posicaoX = 50;
    float posicaoY = 300;
    float velocidadeHorizontal = 4;
    float velocidadeVertical = 0;
    float gravidade = 0.5;
    float larguraPersonagem = 30;
    float alturaPersonagem = 40;

    // CAMERA E CENARIO
    float cameraX = 0;
    float larguraCenario = 2400;

    int vidas = 5;
    bool invulneravel = false;
    float tempoInvulneravel = 0;
    bool personagemVisivel = true;
    bool derrota = false;
    bool estaNoChao = false;

    // PLATAFORMAS (originais)
    plataforma plataformas[6] = {
        { 0, 500, 150, 15 },
        { 200, 425, 350, 15 },
        { 500, 500, 300, 15 },
        { 800, 425, 250, 15 },
        { 1100, 350, 300, 15 },
        { 1450, 425, 350, 15 }
    };
    int qtdPlataformas = sizeof(plataformas) / sizeof(plataformas[0]);

    // OBSTACULOS (originais)
    obstaculo obstaculos[1] = {
        { 650, 650, 40, 30 }
    };
    int qtdObstaculos = sizeof(obstaculos) / sizeof(obstaculos[0]);

    // Reinicia apenas os dados da partida
    // (mantem plataformas, obstaculos e configuracoes originais)
#define REINICIAR_PARTIDA() do { \
        posicaoX = 50; posicaoY = 300; \
        velocidadeVertical = 0; cameraX = 0; vidas = 5; \
        invulneravel = false; tempoInvulneravel = 0; \
        personagemVisivel = true; derrota = false; estaNoChao = false; \
    } while (0)

    while (jogoEmExecucao) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogoEmExecucao = false;
        }

        // TECLAS: pulo, pausa e retorno ao menu
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (estado == GAMEPLAY) {
                if ((evento.keyboard.keycode == ALLEGRO_KEY_SPACE ||
                    evento.keyboard.keycode == ALLEGRO_KEY_UP ||
                    evento.keyboard.keycode == ALLEGRO_KEY_W) && estaNoChao) {
                    velocidadeVertical = -10;
                    estaNoChao = false;
                }

                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    estado = PAUSA;
            }
            else if (estado == PAUSA &&
                evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                estado = GAMEPLAY;
            }
        }

        // CLIQUES DOS BOTOES
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mx = evento.mouse.x;
            int my = evento.mouse.y;

            if (estado == MENU) {
                if (clicou_botao(mx, my, botaoX, 250, botaoLargura, botaoAltura)) {
                    REINICIAR_PARTIDA();
                    estado = GAMEPLAY;
                }
                else if (clicou_botao(mx, my, botaoX, 340, botaoLargura, botaoAltura)) {
                    estado = CARREGAR_JOGO;
                }
                else if (clicou_botao(mx, my, botaoX, 430, botaoLargura, botaoAltura)) {
                    jogoEmExecucao = false;
                }
            }
            else if (estado == CARREGAR_JOGO) {
                if (clicou_botao(mx, my, botaoX, 250, botaoLargura, botaoAltura)) {
                    // Ainda nao existe salvamento; inicia uma partida demonstrativa.
                    REINICIAR_PARTIDA();
                    estado = GAMEPLAY;
                }
                else if (clicou_botao(mx, my, botaoX, 340, botaoLargura, botaoAltura)) {
                    printf("Ainda nao ha um segundo jogo salvo.\\n");
                }
                else if (clicou_botao(mx, my, botaoX, 430, botaoLargura, botaoAltura)) {
                    estado = MENU;
                }
            }
            else if (estado == PAUSA) {
                if (clicou_botao(mx, my, botaoX, 300, botaoLargura, botaoAltura))

                    estado = GAMEPLAY;
                else if (clicou_botao(mx, my, botaoX, 390, botaoLargura, botaoAltura))
                    estado = MENU;
            }
            else if (estado == DERROTA || estado == VITORIA) {
                if (clicou_botao(mx, my, botaoX, 350, botaoLargura, botaoAltura)) {
                    REINICIAR_PARTIDA();
                    estado = GAMEPLAY;
                }
                else if (clicou_botao(mx, my, botaoX, 440, botaoLargura, botaoAltura)) {
                    estado = MENU;
                }
            }
        }

        // ATUALIZACAO DA GAMEPLAY: roda somente durante a partida
        if (evento.type == ALLEGRO_EVENT_TIMER && estado == GAMEPLAY) {
            // IMPEDIR MOVIMENTOS APOS DERROTA
            if (!derrota) {
                // TEMPORIZADOR DE INVULNERABILIDADE
                if (invulneravel) {
                    tempoInvulneravel += 1.0 / 60;
                    personagemVisivel = ((int)(tempoInvulneravel * 10) % 2 == 0);

                    if (tempoInvulneravel >= 2.0) {
                        invulneravel = false;
                        tempoInvulneravel = 0;
                        personagemVisivel = true;
                    }
                }

                // MOVIMENTACAO PERSONAGEM
                ALLEGRO_KEYBOARD_STATE teclado;
                al_get_keyboard_state(&teclado);

                if (al_key_down(&teclado, ALLEGRO_KEY_A) ||
                    al_key_down(&teclado, ALLEGRO_KEY_LEFT))
                    posicaoX -= velocidadeHorizontal;

                if (al_key_down(&teclado, ALLEGRO_KEY_D) ||
                    al_key_down(&teclado, ALLEGRO_KEY_RIGHT))
                    posicaoX += velocidadeHorizontal;

                // GRAVIDADE E MOVIMENTO VERTICAL
                float posicaoYAnterior = posicaoY;
                estaNoChao = false;
                velocidadeVertical += gravidade;
                posicaoY += velocidadeVertical;

                // COLISAO COM PLATAFORMAS
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

                // COLISAO COM CHAO
                if (posicaoY + alturaPersonagem >= 680) {
                    posicaoY = 680 - alturaPersonagem;
                    velocidadeVertical = 0;
                    estaNoChao = true;
                }

                // COLISAO COM OBSTACULO
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

                // PERDA DE VIDAS
                if (vidas <= 0) {
                    derrota = true;
                    estado = DERROTA;
                }

                // LIMITES DA TELA
                if (posicaoX < 0) posicaoX = 0;
                if (posicaoX + larguraPersonagem > larguraCenario)
                    posicaoX = larguraCenario - larguraPersonagem;

                // MOVIMENTACAO DA CAMERA
                float cameraFoco = posicaoX + larguraPersonagem / 2 - 640;
                if (cameraFoco < 0) cameraFoco = 0;
                if (cameraFoco > larguraCenario - 1280)
                    cameraFoco = larguraCenario - 1280;
                cameraX += (cameraFoco - cameraX) * 0.1;

                // Condicao provisoria de vitoria: chegar ao final do cenario.
                // Substituir pela coleta do fragmento quando ela for implementada.
                if (posicaoX >= larguraCenario - larguraPersonagem)
                    estado = VITORIA;
            }
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
            redesenhar = true;

        // DESENHO DAS TELAS
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;

            if (estado == MENU || estado == CARREGAR_JOGO) {
                al_clear_to_color(al_map_rgb(190, 150, 210));

                if (estado == MENU) {
                    al_draw_text(titulo, al_map_rgb(0, 0, 255),
                        640, 120, ALLEGRO_ALIGN_CENTER, "GUARDIA DO TEMPO");

                    desenhar_botao(botaoX, 250, botaoLargura, botaoAltura,
                        "NOVO JOGO", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 340, botaoLargura, botaoAltura,
                        "CARREGAR JOGO", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 430, botaoLargura, botaoAltura,
                        "SAIR", fonte, al_map_rgb(0, 0, 255));
                }
                else {
                    al_draw_text(titulo, al_map_rgb(0, 0, 255),
                        640, 150, ALLEGRO_ALIGN_CENTER, "CARREGAR JOGO");
                    desenhar_botao(botaoX, 250, botaoLargura, botaoAltura,
                        "JOGO 01", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 340, botaoLargura, botaoAltura,
                        "JOGO 02", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 430, botaoLargura, botaoAltura,
                        "VOLTAR", fonte, al_map_rgb(0, 0, 255));
                }
            }
            else {
                // CENARIO E PERSONAGEM: desenho original da equipe
                al_clear_to_color(al_map_rgb(5, 207, 250));
                al_draw_line(-cameraX, 680, larguraCenario - cameraX, 680,
                    al_map_rgb(255, 255, 255), 2.0);

                al_draw_textf(fonte, al_map_rgb(255, 255, 255),
                    20, 20, 0, "Vidas: %d", vidas);

                for (int i = 0; i < qtdPlataformas; i++) {
                    al_draw_filled_rectangle(
                        plataformas[i].x - cameraX, plataformas[i].y,
                        plataformas[i].x + plataformas[i].largura - cameraX,
                        plataformas[i].y + plataformas[i].altura,
                        al_map_rgb(31, 150, 12));
                }

                for (int i = 0; i < qtdObstaculos; i++) {
                    al_draw_filled_rectangle(
                        obstaculos[i].x - cameraX, obstaculos[i].y,
                        obstaculos[i].x + obstaculos[i].largura - cameraX,
                        obstaculos[i].y + obstaculos[i].altura,
                        al_map_rgb(200, 0, 0));
                }

                if (personagemVisivel) {
                    al_draw_filled_rectangle(
                        posicaoX - cameraX, posicaoY,
                        posicaoX + larguraPersonagem - cameraX,
                        posicaoY + alturaPersonagem,
                        al_map_rgb(237, 61, 17));
                }

                if (estado == PAUSA) {
                    al_draw_filled_rectangle(0, 0, 1280, 720,
                        al_map_rgba(0, 0, 0, 160));
                    al_draw_text(titulo, al_map_rgb(255, 255, 255),
                        640, 190, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");
                    desenhar_botao(botaoX, 300, botaoLargura, botaoAltura,
                        "CONTINUAR", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 390, botaoLargura, botaoAltura,
                        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
                }
                else if (estado == DERROTA) {
                    al_clear_to_color(al_map_rgb(82, 7, 7));
                    al_draw_text(titulo, al_map_rgb(250, 250, 250),
                        640, 240, ALLEGRO_ALIGN_CENTER, "FIM DE JOGO!");
                    desenhar_botao(botaoX, 350, botaoLargura, botaoAltura,
                        "TENTAR NOVAMENTE", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 440, botaoLargura, botaoAltura,
                        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
                }
                else if (estado == VITORIA) {
                    al_clear_to_color(al_map_rgb(5, 100, 60));
                    al_draw_text(titulo, al_map_rgb(255, 255, 255),
                        640, 240, ALLEGRO_ALIGN_CENTER, "FASE CONCLUIDA!");
                    desenhar_botao(botaoX, 350, botaoLargura, botaoAltura,
                        "JOGAR NOVAMENTE", fonte, al_map_rgb(0, 0, 255));
                    desenhar_botao(botaoX, 440, botaoLargura, botaoAltura,
                        "MENU PRINCIPAL", fonte, al_map_rgb(0, 0, 255));
                }
            }

            al_flip_display();
        }
    }

    // FINALIZACAO E LIBERACAO DE RECURSOS
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(display);

    if (titulo != fonte) al_destroy_font(titulo);
    al_destroy_font(fonte);

    return 0;
}