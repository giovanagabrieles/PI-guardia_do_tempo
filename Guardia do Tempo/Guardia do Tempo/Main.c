#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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
    TRANSICAO_FASE,
    DERROTA,
    VITORIA
} EstadoJogo;

// ESTRUTURA DOS PROJETEIS DO CHEFE
typedef struct {
    float x, y;
    float velocidadeX, velocidadeY;
    bool ativo;
} ataqueChefe;

#define MAX_ATAQUES_CHEFE 5

int carregar_recorde() {
    FILE* arquivo = NULL;
    int recorde = 0;

    if (fopen_s(&arquivo, "recorde.txt", "r") == 0 && arquivo != NULL) {

        if (fscanf_s(arquivo, "%d", &recorde) != 1 || recorde < 0) {
            recorde = 0;
        }

        fclose(arquivo);
    }

    return recorde;
}


void atualizar_recorde(int pontuacao, int* recorde) {

    if (pontuacao > *recorde) {

        FILE* arquivo = NULL;

        if (fopen_s(&arquivo, "recorde.txt", "w") == 0 && arquivo != NULL) {

            fprintf(arquivo, "%d", pontuacao);
            fclose(arquivo);

            *recorde = pontuacao;
        }
    }
}

void carregar_fase(
    int faseAtual,
    objetoInterativo objetosHistoricos[],
    plataforma plataformas[],
    obstaculo obstaculos[],
    float* chefeX,
    float* chefeY,
    float* larguraCenario
) {
    if (faseAtual == 1) {

        // FASE 1 - EGITO ANTIGO
        objetosHistoricos[0] = (objetoInterativo){
            350, 395, 30, 30,
            "Papiro com hieroglifos",
            "Os egipcios desenvolveram um sistema de escrita chamado hieroglifos. Eles utilizavam desenhos e simbolos para representar palavras, sons e ideias. Escreviam em paredes, monumentos e folhas feitas de papiro."
        };

        objetosHistoricos[1] = (objetoInterativo){
            1150, 320, 30, 30,
            "Vaso canopico",
            "Os antigos egipcios acreditavam na vida apos a morte e praticavam a mumificacao. Durante esse processo, determinados orgaos eram retirados e guardados em recipientes especiais chamados vasos canopicos."
        };

        objetosHistoricos[2] = (objetoInterativo){
            1600, 395, 30, 30,
            "Miniatura de um shaduf",
            "O rio Nilo era essencial para o Egito Antigo. Suas cheias deixavam o solo fertil para a agricultura. Os egipcios utilizavam o shaduf para elevar agua e irrigar as plantacoes."
        };

        plataformas[0] = (plataforma){ 0, 500, 150, 15 };
        plataformas[1] = (plataforma){ 200, 425, 350, 15 };
        plataformas[2] = (plataforma){ 500, 500, 300, 15 };
        plataformas[3] = (plataforma){ 800, 425, 250, 15 };
        plataformas[4] = (plataforma){ 1100, 350, 300, 15 };
        plataformas[5] = (plataforma){ 1450, 425, 350, 15 };
        plataformas[6] = (plataforma){ 1800, 500, 250, 15 };
        plataformas[7] = (plataforma){ 2050, 425, 250, 15 };
        plataformas[8] = (plataforma){ 2300, 500, 100, 15 };

        obstaculos[0] = (obstaculo){ 650, 470, 40, 30 };

        *chefeX = 2335;
        *chefeY = 460;
        *larguraCenario = 2400;
    }
    else if (faseAtual == 2) {

        // FASE 2 - GRECIA ANTIGA
        objetosHistoricos[0] = (objetoInterativo){
            350, 395, 30, 30,
            "Amfora grega",
            "As amforas eram recipientes utilizados pelos gregos antigos para armazenar e transportar produtos como vinho, azeite e alimentos."
        };

        objetosHistoricos[1] = (objetoInterativo){
            1150, 320, 30, 30,
            "Escudo hoplita",
            "Os hoplitas eram soldados da Grecia Antiga. Eles utilizavam escudos grandes e arredondados para se proteger durante as batalhas."
        };

        objetosHistoricos[2] = (objetoInterativo){
            1600, 395, 30, 30,
            "Pergaminho filosofico",
            "A Grecia Antiga foi um importante centro de desenvolvimento da filosofia. Pensadores como Socrates, Platao e Aristoteles refletiram sobre conhecimento, politica e etica."
        };

        plataformas[0] = (plataforma){ 0, 500, 150, 15 };
        plataformas[1] = (plataforma){ 200, 425, 350, 15 };
        plataformas[2] = (plataforma){ 500, 500, 300, 15 };
        plataformas[3] = (plataforma){ 800, 425, 250, 15 };
        plataformas[4] = (plataforma){ 1100, 350, 300, 15 };
        plataformas[5] = (plataforma){ 1450, 425, 350, 15 };
        plataformas[6] = (plataforma){ 1800, 500, 250, 15 };
        plataformas[7] = (plataforma){ 2050, 425, 250, 15 };
        plataformas[8] = (plataforma){ 2300, 500, 100, 15 };

        obstaculos[0] = (obstaculo){ 650, 470, 40, 30 };

        *chefeX = 2335;
        *chefeY = 460;
        *larguraCenario = 2400;
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


   // OBJETOS HISTORICOS DO EGITO ANTIGO
    objetoInterativo objetosEgito[3] = {
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

    // OBJETOS HISTORICOS DA GRECIA ANTIGA
    objetoInterativo objetosGrecia[3] = {
        {
            350, 395, 30, 30,
            "Amfora grega",
            "As amforas eram recipientes utilizados pelos gregos antigos para armazenar e transportar produtos como vinho, azeite e alimentos. Muitas eram decoradas com cenas mitologicas, esportivas e do cotidiano."
        },
        {
            1150, 320, 30, 30,
            "Escudo hoplita",
            "Os hoplitas eram soldados da Grecia Antiga. Eles utilizavam escudos grandes e arredondados, chamados hoplons ou aspides, para se proteger durante as batalhas e formar uma defesa coletiva."
        },
        {
            1600, 395, 30, 30,
            "Pergaminho filosofico",
            "A Grecia Antiga foi um importante centro de desenvolvimento da filosofia. Pensadores como Socrates, Platao e Aristoteles refletiram sobre temas como conhecimento, politica, etica e a natureza do mundo."
        }
    };

    bool objetosEncontrados[3] = { false, false, false };

    // CHEFE FINAL - ANUBIS
    float chefeX = 2335;
    float chefeY = 460;
    float larguraChefe = 30;
    float alturaChefe = 40;

    bool chefeDesbloqueado = false;
    bool pertoDoChefe = false;
    bool mostrarDialogoChefe = false;

    // ATAQUES DO CHEFE
    ataqueChefe ataquesChefe[MAX_ATAQUES_CHEFE] = { 0 };

    float tempoAtaqueChefe = 0;
    float intervaloAtaqueChefe = 1.5;

    float velocidadeAtaqueChefe = 3.0;
    
    // CHEFE FINAL - MINOTAURO
    float minotauroX = 2335;
    float minotauroY = 460;
    float larguraMinotauro = 30;
    float alturaMinotauro = 40;

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


    // PERGUNTAS DA GRECIA ANTIGA
    char perguntasGrecia[4][200] = {
        "Qual era a cidade grega conhecida pelo desenvolvimento da democracia?",
        "Quem era o deus grego dos mares?",
        "Qual era o principal objetivo dos Jogos Olimpicos na Grecia Antiga?",
        "Quem foi um importante filosofo grego, mestre de Platao?"
    };

    // ALTERNATIVAS DA GRECIA
    char alternativasGrecia[4][3][120] = {
        {
            "Esparta",
            "Atenas",
            "Troia"
        },
        {
            "Zeus",
            "Hades",
            "Poseidon"
        },
        {
            "Homenagear os deuses por meio de competicoes",
            "Escolher os reis das cidades",
            "Treinar soldados para as guerras"
        },
        {
            "Aristoteles",
            "Socrates",
            "Herodoto"
        }
    };

    // RESPOSTAS CORRETAS DA GRECIA
    int respostasCorretasGrecia[4] = { 1, 2, 0, 1 };


    int perguntaAtual = 0;


    // CAMERA E CENARIO
    float cameraX = 0;
    float larguraCenario = 2400;

    int pontuacao = 0;
    int vidas = 5;
    int recorde = carregar_recorde();
    int faseAtual = 1; // 1 = Egito, 2 = Grecia
    int fragmentosTemporais = 0;
    #define MAX_FRAGMENTOS_TEMPORAIS 2
    bool invulneravel = false;
    float tempoInvulneravel = 0;
    bool personagemVisivel = true;
    bool derrota = false;
    bool estaNoChao = false;

    // PLATAFORMAS EGITO ANTIGO
    plataforma plataformasEgito[9] = {
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

    // PLATAFORMAS DA GRECIA ANTIGA
    plataforma plataformasGrecia[9] = {
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

    int qtdPlataformas = 9;

    // OBSTACULOS EGITO ANTIGO
    obstaculo obstaculosEgito[1] = {
        { 650, 470, 40, 30 }
    };

    // OBSTACULOS DA GRECIA ANTIGA
    obstaculo obstaculosGrecia[1] = {
        { 650, 470, 40, 30 }
    };

    // DADOS ATIVOS DA FASE
    objetoInterativo* objetosHistoricos = objetosEgito;
    plataforma* plataformas = plataformasEgito;
    obstaculo* obstaculos = obstaculosEgito;

    // CARREGA OS DADOS INICIAIS DO EGITO
    carregar_fase(
        faseAtual,
        objetosHistoricos,
        plataformas,
        obstaculos,
        &chefeX,
        &chefeY,
        &larguraCenario
    );

    int qtdObstaculos = 1;

    bool obstaculoSuperado = false;

    // Reinicia apenas os dados da partida
    // (mantem plataformas, obstaculos e configuracoes originais)

        #define REINICIAR_PARTIDA() do { \
            posicaoX = 50; \
            posicaoY = 300; \
            velocidadeVertical = 0; \
            cameraX = 0; \
            vidas = 5; \
            pontuacao = 0; \
            fragmentosTemporais = 0; \
            faseAtual = 1; \
            objetosHistoricos = objetosEgito; \
            plataformas = plataformasEgito; \
            obstaculos = obstaculosEgito; \
            carregar_fase( \
                faseAtual, \
                objetosHistoricos, \
                plataformas, \
                obstaculos, \
                &chefeX, \
                &chefeY, \
                &larguraCenario \
            ); \
            obstaculoSuperado = false; \
            invulneravel = false; \
            tempoInvulneravel = 0; \
            personagemVisivel = true; \
            derrota = false; \
            estaNoChao = false; \
            mostrarInformacao = false; \
            pertoDoObjeto = false; \
            objetosEncontrados[0] = false; \
            objetosEncontrados[1] = false; \
            objetosEncontrados[2] = false; \
            chefeDesbloqueado = false; \
            pertoDoChefe = false; \
            mostrarDialogoChefe = false; \
            objetoAtual = -1; \
            perguntaAtual = 0; \
            tempoAtaqueChefe = 0; \
            for (int i = 0; i < MAX_ATAQUES_CHEFE; i++) { \
            ataquesChefe[i].ativo = false; \
            } \
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
                            pontuacao += 100;
                        }

                        objetoAtual = -1;
                    }
                    else if (mostrarDialogoChefe) {
                        mostrarDialogoChefe = false;
                        perguntaAtual = 0;
                        estado = QUESTIONARIO;
                    }
                    else if (pertoDoChefe && chefeDesbloqueado) {
                        mostrarDialogoChefe = true;
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
                    int respostaCorretaAtual;

                    if (faseAtual == 1) {
                        respostaCorretaAtual = respostasCorretas[perguntaAtual];
                    }
                    else {
                        respostaCorretaAtual = respostasCorretasGrecia[perguntaAtual];
                    }

                    if (alternativaEscolhida == respostaCorretaAtual) {

                        perguntaAtual++;

                        // ACERTOU AS QUATRO PERGUNTAS
                        if (perguntaAtual >= 4) {

                            pontuacao += 200;

                            // GANHA UM FRAGMENTO TEMPORAL AO CONCLUIR A FASE
                            if (fragmentosTemporais < MAX_FRAGMENTOS_TEMPORAIS) {
                                fragmentosTemporais++;
                            }

                            if (faseAtual == 1) {

                                // AVANCA DO EGITO PARA A GRECIA
                                faseAtual = 2;

                                // TROCA OS DADOS ATIVOS PARA A GRECIA
                                objetosHistoricos = objetosGrecia;
                                plataformas = plataformasGrecia;
                                obstaculos = obstaculosGrecia;

                                carregar_fase(
                                    faseAtual,
                                    objetosHistoricos,
                                    plataformas,
                                    obstaculos,
                                    &chefeX,
                                    &chefeY,
                                    &larguraCenario
                                );

                                // REINICIA OS OBJETOS DA NOVA FASE
                                objetosEncontrados[0] = false;
                                objetosEncontrados[1] = false;
                                objetosEncontrados[2] = false;

                                // REINICIA O ESTADO DO CHEFE
                                chefeDesbloqueado = false;
                                pertoDoChefe = false;
                                mostrarDialogoChefe = false;

                                // REINICIA OS ATAQUES DO CHEFE
                                tempoAtaqueChefe = 0;

                                for (int i = 0; i < MAX_ATAQUES_CHEFE; i++) {
                                    ataquesChefe[i].ativo = false;
                                }

                                // REPOSICIONA O PERSONAGEM
                                posicaoX = 50;
                                posicaoY = 300;
                                velocidadeVertical = 0;
                                cameraX = 0;

                                // REINICIA CONTROLES DA FASE
                                objetoAtual = -1;
                                perguntaAtual = 0;
                                mostrarInformacao = false;
                                pertoDoObjeto = false;
                                obstaculoSuperado = false;
                                invulneravel = false;
                                tempoInvulneravel = 0;
                                personagemVisivel = true;
                                estaNoChao = false;

                                estado = TRANSICAO_FASE;
                            }

                            else {
                                // FINALIZOU AS DUAS FASES
                                atualizar_recorde(pontuacao, &recorde);
                                estado = VITORIA;
                            }
                        }
                    }

                    // RESPOSTA ERRADA
                    else {
                        vidas--;

                        if (vidas <= 0) {
                            atualizar_recorde(pontuacao, &recorde);
                            estado = DERROTA;
                        }
                    }
                }
            }

            else if (estado == PAUSA &&
                evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                estado = GAMEPLAY;
            }
            else if (estado == TRANSICAO_FASE &&
                evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
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

                // ATAQUES DO CHEFE
                if (chefeDesbloqueado && !mostrarInformacao && !mostrarDialogoChefe) {

                    // CONTROLA O INTERVALO ENTRE OS DISPAROS
                    tempoAtaqueChefe += 1.0 / 60.0;

                    if (tempoAtaqueChefe >= intervaloAtaqueChefe) {

                        // PROCURA UM ESPACO LIVRE PARA O NOVO PROJETIL
                        for (int i = 0; i < MAX_ATAQUES_CHEFE; i++) {

                            if (!ataquesChefe[i].ativo) {

                                // CALCULA A DIRECAO DO JOGADOR
                                float dx = (posicaoX + larguraPersonagem / 2) -
                                    (chefeX + larguraChefe / 2);

                                float dy = (posicaoY + alturaPersonagem / 2) -
                                    (chefeY + alturaChefe / 2);

                                float distancia = sqrtf(dx * dx + dy * dy);

                                // CRIA O PROJETIL
                                if (distancia > 0) {

                                    ataquesChefe[i].x = chefeX + larguraChefe / 2 - 9;
                                    ataquesChefe[i].y = chefeY + alturaChefe / 2 - 9;

                                    ataquesChefe[i].velocidadeX =
                                        (dx / distancia) * velocidadeAtaqueChefe;

                                    ataquesChefe[i].velocidadeY =
                                        (dy / distancia) * velocidadeAtaqueChefe;

                                    ataquesChefe[i].ativo = true;
                                }

                                tempoAtaqueChefe = 0;
                                break;
                            }
                        }
                    }
                }

                // MOVIMENTACAO E COLISAO DOS ATAQUES DO CHEFE
                for (int i = 0; i < MAX_ATAQUES_CHEFE; i++) {

                    if (ataquesChefe[i].ativo) {

                        // MOVIMENTA O PROJETIL
                        ataquesChefe[i].x += ataquesChefe[i].velocidadeX;
                        ataquesChefe[i].y += ataquesChefe[i].velocidadeY;

                        // REMOVE PROJETEIS QUE SAIRAM DO CENARIO
                        if (ataquesChefe[i].x < 0 ||
                            ataquesChefe[i].x > larguraCenario ||
                            ataquesChefe[i].y < -20 ||
                            ataquesChefe[i].y > 740) {

                            ataquesChefe[i].ativo = false;
                        }

                        // VERIFICA COLISAO COM O PERSONAGEM
                        else if (
                            posicaoX + larguraPersonagem > ataquesChefe[i].x &&
                            posicaoX < ataquesChefe[i].x + 18 &&
                            posicaoY + alturaPersonagem > ataquesChefe[i].y &&
                            posicaoY < ataquesChefe[i].y + 18) {

                            // O PROJETIL DESAPARECE AO ATINGIR O PERSONAGEM
                            ataquesChefe[i].ativo = false;

                            if (!invulneravel) {

                                vidas--;
                                pontuacao -= 100;

                                if (pontuacao < 0) {
                                    pontuacao = 0;
                                }

                                // ATIVA A INVULNERABILIDADE
                                invulneravel = true;
                                tempoInvulneravel = 0;
                                personagemVisivel = true;

                                // RETORNA O PERSONAGEM AO INICIO
                                posicaoX = 50;
                                posicaoY = 300;
                                velocidadeVertical = 0;
                                cameraX = 0;
                            }
                        }
                    }
                }

          
                if (!mostrarInformacao && !mostrarDialogoChefe) {  // BLOQUEIA TODA MOVIMENTAÇÃO DO PERSONAGEM CASO A CAIXA DE TEXTO ESTIVER ABERTA
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

                // VERIFICA SE OS TRES OBJETOS FORAM ENCONTRADOS
                chefeDesbloqueado =
                    objetosEncontrados[0] &&
                    objetosEncontrados[1] &&
                    objetosEncontrados[2];

                // VERIFICA A PROXIMIDADE DO ANUBIS
                pertoDoChefe = false;

                if (chefeDesbloqueado &&
                    posicaoX + larguraPersonagem >= chefeX - 40 &&
                    posicaoX <= chefeX + larguraChefe + 40 &&
                    posicaoY + alturaPersonagem >= chefeY - 30 &&
                    posicaoY <= chefeY + alturaChefe + 30) {

                    pertoDoChefe = true;
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
                        pontuacao -= 100;

                        if (pontuacao < 0) {
                            pontuacao = 0;
                        }

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
                        pontuacao -= 100;

                        if (pontuacao < 0) {
                            pontuacao = 0;
                        }

                        invulneravel = true;
                        posicaoX = 50;
                        posicaoY = 300;
                        velocidadeVertical = 0;
                        cameraX = 0;
                    }
                }
                // GANHA PONTOS AO SUPERAR O OBSTACULO
                if (!obstaculoSuperado &&
                    posicaoX > obstaculos[0].x + obstaculos[0].largura) {

                    pontuacao += 50;
                    obstaculoSuperado = true;
                }

                // PERDA DE VIDAS
                if (vidas <= 0) {
                    derrota = true;

                    atualizar_recorde(pontuacao, &recorde);

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
                    faseAtual == 1
                    ? "Desafio do Egito Antigo"
                    : "Desafio da Grecia Antiga"
                );

                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    640, 160,
                    ALLEGRO_ALIGN_CENTER,
                    "Pergunta %d de 4",
                    perguntaAtual + 1
                );

                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 100, 100),
                    640, 195,
                    ALLEGRO_ALIGN_CENTER,
                    "Energia do chefe: %d/4",
                    4 - perguntaAtual
                );


                if (perguntaAtual < 4) {

                    char (*perguntasAtuais)[200];
                    char (*alternativasAtuais)[3][120];

                    if (faseAtual == 1) {
                        perguntasAtuais = perguntas;
                        alternativasAtuais = alternativas;
                    }
                    else {
                        perguntasAtuais = perguntasGrecia;
                        alternativasAtuais = alternativasGrecia;
                    }

                    al_draw_multiline_text(
                        fonte,
                        al_map_rgb(255, 255, 255),
                        640, 230,
                        900, 35,
                        ALLEGRO_ALIGN_CENTER,
                        perguntasAtuais[perguntaAtual]
                    );

                    for (int i = 0; i < 3; i++) {
                        al_draw_textf(
                            fonte,
                            al_map_rgb(255, 220, 150),
                            640, 360 + i * 65,
                            ALLEGRO_ALIGN_CENTER,
                            "%d - %s",
                            i + 1,
                            alternativasAtuais[perguntaAtual][i]
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

            else if (estado == TRANSICAO_FASE) {

                al_clear_to_color(al_map_rgb(30, 25, 50));

                al_draw_text(
                    titulo,
                    al_map_rgb(255, 220, 0),
                    640, 180,
                    ALLEGRO_ALIGN_CENTER,
                    "FASE 1 CONCLUIDA!"
                );

                al_draw_text(
                    titulo,
                    al_map_rgb(255, 255, 255),
                    640, 250,
                    ALLEGRO_ALIGN_CENTER,
                    "BEM-VINDO A GRECIA ANTIGA!"
                );

                al_draw_text(
                    fonte,
                    al_map_rgb(220, 220, 220),
                    640, 330,
                    ALLEGRO_ALIGN_CENTER,
                    "Voce venceu Anubis e agora enfrentara o Minotauro."
                );

                al_draw_text(
                    fonte,
                    al_map_rgb(255, 220, 0),
                    640, 430,
                    ALLEGRO_ALIGN_CENTER,
                    "Pressione ENTER para continuar"
                );
            }

            // DEMAIS TELAS E GAMEPLAY
            else {
                if (faseAtual == 1) {
                    // Ceu do Egito
                    al_clear_to_color(al_map_rgb(240, 190, 120));
                }
                else {
                    // Ceu da Grecia
                    al_clear_to_color(al_map_rgb(120, 190, 220));
                }

                int quantidadeEncontrada = 0;

                for (int i = 0; i < 3; i++) {
                    if (objetosEncontrados[i]) {
                        quantidadeEncontrada++;
                    }
                }

                // OBJETOS HISTÓRICOS ENCONTRADOS 
                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    20, 50,
                    0,
                    "Objetos: %d/3",
                    quantidadeEncontrada
                );

                // FRAGMENTOS TEMPORAIS
                al_draw_textf(
                    fonte,
                    al_map_rgb(100, 220, 255),
                    20, 140,
                    0,
                    "Fragmentos Temporais: %d/%d",
                    fragmentosTemporais,
                    MAX_FRAGMENTOS_TEMPORAIS
                );

                //PONTUACAO
                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 220, 0),
                    20, 80,
                    0,
                    "Pontuacao: %d",
                    pontuacao
                );
                //RECORDE
                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    20, 110,
                    0,
                    "Recorde: %d",
                    recorde
                );

                // INDICADOR DA FASE ATUAL
                al_draw_text(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    640, 30,
                    ALLEGRO_ALIGN_CENTER,
                    faseAtual == 1
                    ? "FASE 1 - EGITO ANTIGO"
                    : "FASE 2 - GRECIA ANTIGA"
                );

                // INDICADOR DE VIDAS
                al_draw_text(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    1120, 30,
                    ALLEGRO_ALIGN_CENTER,
                    "Vidas:"
                );
                //VIDA
                for (int i = 0; i < 5; i++) {
                    ALLEGRO_COLOR corVida = (i < vidas)
                        ? al_map_rgb(220, 40, 40)
                        : al_map_rgb(120, 120, 120);

                    al_draw_filled_circle(
                        1070 + i * 25, 65, 8,
                        corVida
                    );
                }

                // INDICADOR DE FRAGMENTOS TEMPORAIS
                al_draw_text(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    1120, 95,
                    ALLEGRO_ALIGN_CENTER,
                    "Fragmentos:"
                );

                //FRAGMENTOS TEMPORAIS
                for (int i = 0; i < MAX_FRAGMENTOS_TEMPORAIS; i++) {

                    ALLEGRO_COLOR corFragmento = (i < fragmentosTemporais)
                        ? al_map_rgb(0, 180, 255)       // Conquistado
                        : al_map_rgb(100, 100, 100);    // Nao conquistado

                    al_draw_filled_circle(
                        1100 + i * 40,
                        130,
                        10,
                        corFragmento
                    );
                }

                desenhar_plataformas(
                    plataformas,
                    qtdPlataformas,
                    cameraX,
                    faseAtual
                );

                desenhar_obstaculos(
                    obstaculos,
                    qtdObstaculos,
                    cameraX,
                    faseAtual
                );

                // DESENHA O CHEFE DA FASE ATUAL
                ALLEGRO_COLOR corChefe;

                if (!chefeDesbloqueado) {
                    corChefe = al_map_rgb(80, 80, 80);
                }
                else if (faseAtual == 1) {
                    // Anubis: roxo
                    corChefe = al_map_rgb(120, 20, 150);
                }
                else {
                    // Minotauro: marrom-avermelhado
                    corChefe = al_map_rgb(150, 60, 40);
                }

                al_draw_filled_rectangle(
                    chefeX - cameraX,
                    chefeY,
                    chefeX + larguraChefe - cameraX,
                    chefeY + alturaChefe,
                    corChefe
                );

                al_draw_rectangle(
                    chefeX - cameraX,
                    chefeY,
                    chefeX + larguraChefe - cameraX,
                    chefeY + alturaChefe,
                    al_map_rgb(255, 200, 0),
                    2
                );

                // DESENHA OS ATAQUES DO CHEFE
                for (int i = 0; i < MAX_ATAQUES_CHEFE; i++) {

                    if (ataquesChefe[i].ativo) {

                        // PROJETIL EM TOM VERMELHO
                        al_draw_filled_rectangle(
                            ataquesChefe[i].x - cameraX,
                            ataquesChefe[i].y,
                            ataquesChefe[i].x + 18 - cameraX,
                            ataquesChefe[i].y + 18,
                            al_map_rgb(220, 50, 40)
                        );

                        // CONTORNO DOURADO
                        al_draw_rectangle(
                            ataquesChefe[i].x - cameraX,
                            ataquesChefe[i].y,
                            ataquesChefe[i].x + 18 - cameraX,
                            ataquesChefe[i].y + 18,
                            al_map_rgb(255, 200, 0),
                            2
                        );
                    }
                }

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
                        objetosEncontrados[i],
                        fonte,
                        faseAtual
                    );
                }

                // AVISO PARA FALAR COM ANUBIS
                if (pertoDoChefe && !mostrarInformacao && !mostrarDialogoChefe) {
                    al_draw_text(
                        fonte,
                        al_map_rgb(255, 255, 255),
                        chefeX - cameraX,
                        chefeY - 25,
                        0,
                        faseAtual == 1
                        ? "E - Falar com Anubis"
                        : "E - Falar com Minotauro"
                    );
                }

                // DIALOGO INICIAL DO ANUBIS
                if (mostrarDialogoChefe) {

                    // Fundo da caixa
                    al_draw_filled_rounded_rectangle(
                        300, 200, 980, 520,
                        20, 20,
                        al_map_rgba(0, 0, 0, 230)
                    );

                    // Borda dourada
                    al_draw_rounded_rectangle(
                        300, 200, 980, 520,
                        20, 20,
                        al_map_rgb(255, 200, 0),
                        3
                    );

                    // Titulo
                    al_draw_text(
                        fonte,
                        al_map_rgb(255, 200, 0),
                        640, 240,
                        ALLEGRO_ALIGN_CENTER,
                        faseAtual == 1
                        ? "ANUBIS - GUARDIAO DO SUBMUNDO"
                        : "MINOTAURO - GUARDIAO DO LABIRINTO"
                    );

                    // Mensagem
                    al_draw_multiline_text(
                        fonte,
                        al_map_rgb(255, 255, 255),
                        340, 300,
                        600, 28,
                        ALLEGRO_ALIGN_LEFT,
                        faseAtual == 1
                        ? "Voce encontrou os tres artefatos do Egito Antigo. Agora precisa provar seus conhecimentos para enfrentar Anubis!"
                        : "Voce encontrou os tres artefatos da Grecia Antiga. Agora precisa provar seus conhecimentos para enfrentar o Minotauro!"
                    );

                    // Instrucao
                    al_draw_text(
                        fonte,
                        al_map_rgb(255, 220, 0),
                        640, 470,
                        ALLEGRO_ALIGN_CENTER,
                        "Aperte E para iniciar a batalha"
                    );
                }

                if (mostrarInformacao && objetoAtual != -1) {
                    desenhar_caixa_informacao(
                        objetosHistoricos[objetoAtual],
                        fonte,
                        mostrarInformacao,
                        faseAtual
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