#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Interface.h"
#include "Cenario.h"
#include "Personagem.h"
#include "Objetos.h"


// ESTADOS DO JOGO
typedef enum {
    MENU,
    CARREGAR_JOGO,
    GAMEPLAY,
    PAUSA,
    QUESTIONARIO,
    DERROTA,
    VITORIA
} EstadoJogo;




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


   // OBJETOS HISTORICOS DO EGITO ANTIGO
    objetoInterativo objetosHistoricos[3] = {
        {
            350, 395, 30, 30,
            "Papiro com hieroglifos",
            "Os egipcios desenvolveram um sistema de escrita chamado hieroglifos. Eles utilizavam desenhos e simbolos para representar palavras, sons e ideias. Escreviam em paredes, monumentos e folhas feitas de papiro."
        },
        {
            1150, 320, 30, 30,
            "Vaso canopico",
            "Os antigos egipcios acreditavam na vida apos a morte e praticavam a mumificacao. Durante esse processo, determinados orgaos eram retirados e guardados em recipientes especiais chamados vasos canopicos."
        },
        {
            1600, 395, 30, 30,
            "Miniatura de um shaduf",
            "O rio Nilo era essencial para o Egito Antigo. Suas cheias deixavam o solo fertil para a agricultura. Os egipcios utilizavam o shaduf, um dispositivo que ajudava a elevar agua do rio ou de canais para irrigar as plantacoes."
        }
    };

    bool objetosEncontrados[3] = { false, false, false };

    int objetoAtual = -1;

    bool mostrarInformacao = false;
    bool pertoDoObjeto = false;

    // PERGUNTAS DO CHEFE FINAL
    char perguntas[4][200] = {
        "Qual era o nome do sistema de escrita egipcio?",
        "Para que serviam os vasos canopicos?",
        "Por que o rio Nilo era importante?",
        "Qual era a funcao do shaduf?"
    };

    // ALTERNATIVAS DAS PERGUNTAS
    char alternativas[4][3][120] = {
        {
            "Alfabeto romano",
            "Hieroglifos",
            "Escrita medieval"
        },
        {
            "Guardar alimentos",
            "Armazenar agua",
            "Guardar orgaos retirados na mumificacao"
        },
        {
            "Deixar o solo fertil para a agricultura",
            "Construir apenas barcos",
            "Impedir o crescimento das plantacoes"
        },
        {
            "Escrever simbolos religiosos",
            "Elevar agua para irrigar as plantacoes",
            "Construir piramides"
        }
    };

    // INDICE DA ALTERNATIVA CORRETA
    int respostasCorretas[4] = { 1, 2, 0, 1 };

    int perguntaAtual = 0;


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
    plataforma plataformas[9] = {
    { 0, 500, 150, 15 },
    { 200, 425, 350, 15 },
    { 500, 500, 300, 15 },
    { 800, 425, 250, 15 },
    { 1100, 350, 300, 15 },
    { 1450, 425, 350, 15 },
    { 1800, 500, 250, 15 },
    { 2050, 425, 250, 15 },
    { 2300, 500, 100, 15 }
    };
    int qtdPlataformas = sizeof(plataformas) / sizeof(plataformas[0]);

    // OBSTACULOS (originais)
    obstaculo obstaculos[1] = {
    { 650, 470, 40, 30 }
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
        objetosEncontrados[0] = false; \
        objetosEncontrados[1] = false; \
        objetosEncontrados[2] = false; \
        objetoAtual = -1; \
        perguntaAtual = 0; \
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

                        // Marca o objeto como encontrado
                        if (objetoAtual != -1) {
                            objetosEncontrados[objetoAtual] = true;
                        }

                        objetoAtual = -1;
                    }
                    else if (pertoDoObjeto) {
                        mostrarInformacao = true;
                    }
                }
            }

            else if (estado == QUESTIONARIO) {

                int alternativaEscolhida = -1;

                if (evento.keyboard.keycode == ALLEGRO_KEY_1)
                    alternativaEscolhida = 0;

                else if (evento.keyboard.keycode == ALLEGRO_KEY_2)
                    alternativaEscolhida = 1;

                else if (evento.keyboard.keycode == ALLEGRO_KEY_3)
                    alternativaEscolhida = 2;

                if (alternativaEscolhida != -1) {

                    // RESPOSTA CORRETA
                    if (alternativaEscolhida == respostasCorretas[perguntaAtual]) {

                        perguntaAtual++;

                        // ACERTOU AS QUATRO PERGUNTAS
                        if (perguntaAtual >= 4) {
                            estado = VITORIA;
                        }
                    }

                    // RESPOSTA ERRADA
                    else {
                        vidas--;

                        if (vidas <= 0) {
                            estado = DERROTA;
                        }
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

                // VERIFICA PROXIMIDADE DOS OBJETOS HISTORICOS
                pertoDoObjeto = false;
                objetoAtual = -1;

                for (int i = 0; i < 3; i++) {

                    if (!objetosEncontrados[i] &&
                        posicaoX + larguraPersonagem >= objetosHistoricos[i].x - 40 &&
                        posicaoX <= objetosHistoricos[i].x + objetosHistoricos[i].largura + 40 &&
                        posicaoY + alturaPersonagem >= objetosHistoricos[i].y - 30 &&
                        posicaoY <= objetosHistoricos[i].y + objetosHistoricos[i].altura + 30) {

                        pertoDoObjeto = true;
                        objetoAtual = i;
                        break;
                    }
                }

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
                // PERDE VIDA AO CAIR DA TELA
                if (posicaoY > 720) {
                    if (!invulneravel) {
                        vidas--;
                        invulneravel = true;
                        tempoInvulneravel = 0;
                    }

                    posicaoX = 50;
                    posicaoY = 300;
                    velocidadeVertical = 0;
                    cameraX = 0;
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

                // VERIFICA SE OS TRES OBJETOS FORAM ENCONTRADOS
                bool todosObjetosEncontrados =
                    objetosEncontrados[0] &&
                    objetosEncontrados[1] &&
                    objetosEncontrados[2];

                // AO CHEGAR AO FINAL, INICIA O QUESTIONARIO
                if (posicaoX >= larguraCenario - larguraPersonagem &&
                    todosObjetosEncontrados) {

                    perguntaAtual = 0;
                    estado = QUESTIONARIO;
                }
                }
            }
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
            redesenhar = true;

        // DESENHO DAS TELAS
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;

            // MENU E CARREGAMENTO
            if (estado == MENU || estado == CARREGAR_JOGO) {
                al_clear_to_color(al_map_rgb(190, 150, 210));

                if (estado == MENU) {
                    desenhar_tela_menu(titulo, fonte, botaoX, botaoLargura, botaoAltura);
                }
                else {
                    desenhar_tela_carregar_jogo(titulo, fonte, botaoX, botaoLargura, botaoAltura);
                }
            }

            // QUESTIONARIO
            else if (estado == QUESTIONARIO) {

                al_clear_to_color(al_map_rgb(30, 25, 50));

                al_draw_text(
                    titulo,
                    al_map_rgb(255, 220, 0),
                    640, 80,
                    ALLEGRO_ALIGN_CENTER,
                    "Desafio do Egito Antigo"
                );

                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    640, 160,
                    ALLEGRO_ALIGN_CENTER,
                    "Pergunta %d de 4",
                    perguntaAtual + 1
                );

                if (perguntaAtual < 4) {
                    al_draw_multiline_text(
                        fonte,
                        al_map_rgb(255, 255, 255),
                        640, 230,
                        900, 35,
                        ALLEGRO_ALIGN_CENTER,
                        perguntas[perguntaAtual]
                    );

                    for (int i = 0; i < 3; i++) {
                        al_draw_textf(
                            fonte,
                            al_map_rgb(255, 220, 150),
                            640, 360 + i * 65,
                            ALLEGRO_ALIGN_CENTER,
                            "%d - %s",
                            i + 1,
                            alternativas[perguntaAtual][i]
                        );
                    }
                }

                al_draw_text(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    640, 590,
                    ALLEGRO_ALIGN_CENTER,
                    "Vidas restantes:"
                );

                for (int i = 0; i < vidas; i++) {
                    al_draw_filled_circle(
                        600 + i * 20, 630, 7,
                        al_map_rgb(220, 40, 40)
                    );
                }

                al_draw_text(
                    fonte,
                    al_map_rgb(200, 200, 200),
                    640, 675,
                    ALLEGRO_ALIGN_CENTER,
                    "Pressione 1, 2 ou 3 para responder"
                );
            }

            // DEMAIS TELAS E GAMEPLAY
            else {
                al_clear_to_color(al_map_rgb(5, 207, 250));

                int quantidadeEncontrada = 0;

                for (int i = 0; i < 3; i++) {
                    if (objetosEncontrados[i]) {
                        quantidadeEncontrada++;
                    }
                }

                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    20, 50,
                    0,
                    "Objetos: %d/3",
                    quantidadeEncontrada
                );

                desenhar_plataformas(plataformas, qtdPlataformas, cameraX);
                desenhar_obstaculos(obstaculos, qtdObstaculos, cameraX);

                desenhar_personagem(
                    posicaoX, posicaoY,
                    larguraPersonagem, alturaPersonagem,
                    cameraX, personagemVisivel
                );

                for (int i = 0; i < 3; i++) {
                    desenhar_objeto_interativo(
                        objetosHistoricos[i],
                        cameraX,
                        pertoDoObjeto && objetoAtual == i,
                        fonte
                    );
                }

                if (mostrarInformacao && objetoAtual != -1) {
                    desenhar_caixa_informacao(
                        objetosHistoricos[objetoAtual],
                        fonte,
                        mostrarInformacao
                    );
                }

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