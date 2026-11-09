#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Interface.h"

// PLATAFORMAS
typedef struct {
    float x, y, largura, altura;
} plataforma;

// OBSTACULOS
typedef struct {
    float x, y, largura, altura;
} obstaculo;

// OBJETO INTERATIVO
typedef struct {
    float x, y, largura, altura;
    char titulo[60];
    char descricao[300];
} objetoInterativo;

// ESTADOS DO JOGO
typedef enum {
    MENU,
    CARREGAR_JOGO,
    GAMEPLAY,
    PAUSA,
    DERROTA,
    VITORIA
} EstadoJogo;



// Desenha todas as plataformas do cenario
void desenhar_plataformas(plataforma plataformas[],
    int qtdPlataformas, float cameraX)
{
    for (int i = 0; i < qtdPlataformas; i++) {
        al_draw_filled_rectangle(
            plataformas[i].x - cameraX, plataformas[i].y,
            plataformas[i].x + plataformas[i].largura - cameraX,
            plataformas[i].y + plataformas[i].altura,
            al_map_rgb(31, 150, 12));
    }
}

// Desenha todos os obstaculos do cenario
void desenhar_obstaculos(obstaculo obstaculos[],
    int qtdObstaculos, float cameraX)
{
    for (int i = 0; i < qtdObstaculos; i++) {
        al_draw_filled_rectangle(
            obstaculos[i].x - cameraX, obstaculos[i].y,
            obstaculos[i].x + obstaculos[i].largura - cameraX,
            obstaculos[i].y + obstaculos[i].altura,
            al_map_rgb(200, 0, 0));
    }
}

// Desenha o personagem na tela
void desenhar_personagem(float posicaoX, float posicaoY,
    float larguraPersonagem, float alturaPersonagem,
    float cameraX, bool personagemVisivel)
{
    if (personagemVisivel) {
        al_draw_filled_rectangle(
            posicaoX - cameraX, posicaoY,
            posicaoX + larguraPersonagem - cameraX,
            posicaoY + alturaPersonagem,
            al_map_rgb(237, 61, 17));
    }
}

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

        al_draw_text(fonte, al_map_rgb(255, 220, 0), 640, 240, ALLEGRO_ALIGN_CENTER,objeto.titulo);

        al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255), 390, 300, 500, 30, 0, objeto.descricao);

        al_draw_text(fonte, al_map_rgb(200, 200, 200), 640, 470, ALLEGRO_ALIGN_CENTER, "Aperte E para fechar");
    }
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

    // TESTE DE OBJETO INTERATIVO HISTÓRICO
    objetoInterativo objetoTeste = {
    350, 385, 30, 30,
    "Objeto misterioso",
    "Este é um texto de exemplo. Uma informação histórica sera apresentada sobre este objeto. Teste 1"
    };

    bool mostrarInformacao = false;
    bool pertoDoObjeto = false;

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
        mostrarInformacao = false; pertoDoObjeto = false; \
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
                    evento.keyboard.keycode == ALLEGRO_KEY_W) && estaNoChao && !mostrarInformacao) { // NÃO VAI PULAR ENQUANTO ESTIVER LENDO
                    velocidadeVertical = -10;
                    estaNoChao = false;
                }

                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    estado = PAUSA;

                //TECLA DE INTERAÇÃO. ATUALMENTE ESTÁ NO E. APERTAR E DE NOVO FECHA A CAIXA
                if (evento.keyboard.keycode == ALLEGRO_KEY_E) {
                    if (mostrarInformacao) {
                        mostrarInformacao = false;
                    }
                    else if (pertoDoObjeto) {
                        mostrarInformacao = true;
                    }
                }
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
          
                if (!mostrarInformacao) {  // BLOQUEIA TODA MOVIMENTAÇÃO DO PERSONAGEM CASO A CAIXA DE TEXTO ESTIVER ABERTA
                // MOVIMENTACAO PERSONAGEM
                ALLEGRO_KEYBOARD_STATE teclado;
                al_get_keyboard_state(&teclado);

                if (al_key_down(&teclado, ALLEGRO_KEY_A) ||
                    al_key_down(&teclado, ALLEGRO_KEY_LEFT))
                    posicaoX -= velocidadeHorizontal;

                if (al_key_down(&teclado, ALLEGRO_KEY_D) ||
                    al_key_down(&teclado, ALLEGRO_KEY_RIGHT))
                    posicaoX += velocidadeHorizontal;

                // VERIFICAÇÃO SE O PERSONAGEM ESTÁ PERTO PARA INTERAGIR COM O OBJETO
                pertoDoObjeto =
                    posicaoX + larguraPersonagem >= objetoTeste.x - 40 &&
                    posicaoX <= objetoTeste.x + objetoTeste.largura + 40 &&
                    posicaoY + alturaPersonagem >= objetoTeste.y - 30 &&
                    posicaoY <= objetoTeste.y + objetoTeste.altura + 30;

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
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
            redesenhar = true;

        // DESENHO DAS TELAS
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;

            if (estado == MENU || estado == CARREGAR_JOGO) {
                al_clear_to_color(al_map_rgb(190, 150, 210));


                if (estado == MENU) {
                   desenhar_tela_menu(titulo, fonte, botaoX,botaoLargura, botaoAltura);

                }
                else {
                    desenhar_tela_carregar_jogo(titulo, fonte, botaoX, botaoLargura, botaoAltura);
                }

            }
            else {
                // CENARIO E PERSONAGEM: desenho original da equipe
                al_clear_to_color(al_map_rgb(5, 207, 250));
                al_draw_line(-cameraX, 680, larguraCenario - cameraX, 680,
                    al_map_rgb(255, 255, 255), 2.0);

                al_draw_textf(fonte, al_map_rgb(255, 255, 255),
                    20, 20, 0, "Vidas: %d", vidas);

                desenhar_plataformas(plataformas, qtdPlataformas, cameraX);

                desenhar_obstaculos(obstaculos, qtdObstaculos, cameraX);

                desenhar_personagem(posicaoX, posicaoY, larguraPersonagem, alturaPersonagem, cameraX, personagemVisivel);

                desenhar_objeto_interativo(objetoTeste, cameraX,
                    pertoDoObjeto, fonte);

                // CAIXA DE INFORMAÇÃO HISTÓRICA
                desenhar_caixa_informacao(objetoTeste, fonte, mostrarInformacao);

                //ESTADO DO JOGO: PAUSA, DERROTA E VITORIA
                if (estado == PAUSA) {
                    desenhar_tela_pausa(titulo, fonte, botaoX, botaoLargura, botaoAltura);

                }
                else if (estado == DERROTA) {
                    desenhar_tela_derrota(titulo, fonte, botaoX, botaoLargura, botaoAltura);
                }
                else if (estado == VITORIA) {
                    desenhar_tela_vitoria(titulo, fonte, botaoX, botaoLargura, botaoAltura);
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