#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main() {
    al_init();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(600, 600);

    float x = 50;
    float y = 300;
    float velocidade = 4;
    float velocidadeY = 0;
    float gravidade = 0.5;
    float largura = 30;
    float altura = 40;

    bool noChao = false;

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();

    al_install_keyboard();

    al_register_event_source(fila, al_get_keyboard_event_source());

    bool jogo = true;

    while (jogo) {

        ALLEGRO_EVENT evento;

        while (al_get_next_event(fila, &evento)) {

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                jogo = false;
            }
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {

                if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && noChao || evento.keyboard.keycode == ALLEGRO_KEY_UP && noChao || evento.keyboard.keycode == ALLEGRO_KEY_W && noChao) {
                    velocidadeY = -10;
                    noChao = false;
                }

                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    jogo = false;
                }
            }
        }

        ALLEGRO_KEYBOARD_STATE teclado;
        al_get_keyboard_state(&teclado);

        if (al_key_down(&teclado, ALLEGRO_KEY_A) || al_key_down(&teclado, ALLEGRO_KEY_LEFT)) {
            x -= velocidade;
        }

        if (al_key_down(&teclado, ALLEGRO_KEY_D) || al_key_down(&teclado, ALLEGRO_KEY_RIGHT)) {
            x += velocidade;
        }

        velocidadeY += gravidade;
        y += velocidadeY;


        if (y + altura >= 580) {
            y = 580 - altura;
            velocidadeY = 0;
            noChao = true;
        }

        if (x < 0) {
            x = 0;
        }

        if (x + largura > 600) {
            x = 600 - largura;
        }


        al_clear_to_color(al_map_rgb(255, 0, 0));


        al_draw_line(0, 580, 600, 580, al_map_rgb(255, 255, 255), 2.0);

        al_draw_filled_rectangle(
            x,
            y,
            x + largura,
            y + altura,
            al_map_rgb(0, 100, 255));


        al_flip_display();


        al_rest(0.01);
    }


    al_destroy_event_queue(fila);
    al_destroy_display(display);

    return 0;
}