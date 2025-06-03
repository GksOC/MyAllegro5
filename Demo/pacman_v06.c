
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

// ---------- GENERAL -----------

long frames;
long score;

void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float between_f(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if (ax1 > bx2)
        return false;
    if (ax2 < bx1)
        return false;
    if (ay1 > by2)
        return false;
    if (ay2 < by1)
        return false;

    return true;
}

// ---------- DISPLAY ----------

#define BUFFER_W 320
#define BUFFER_H 320

#define DISP_SCALE 2
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY *disp;
ALLEGRO_BITMAP *buffer;

void disp_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

// ---------- KEYBOARD ----------

#define KEY_SEEN 1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT *event)
{
    switch (event->type)
    {
    case ALLEGRO_EVENT_TIMER:
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;
        break;

    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= KEY_RELEASED;
        break;
    }
}

// ---------- SPRITES ----------

#define PACMAN_W 13
#define PACMAN_H 15

typedef struct SPRITES
{
    ALLEGRO_BITMAP *_sheet;

    ALLEGRO_BITMAP *pacman[4]; // direita, esquerda, cima, baixo

} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP *sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    // pixel dentro da spritesheet (x,y,w,h)
    sprites.pacman[0] = sprite_grab(20, 1, PACMAN_W, PACMAN_H);  // direita
    sprites.pacman[1] = sprite_grab(21, 17, PACMAN_W, PACMAN_H); // esquerda
    sprites.pacman[2] = sprite_grab(20, 34, PACMAN_W, PACMAN_H); // cima
    sprites.pacman[3] = sprite_grab(20, 49, PACMAN_W, PACMAN_H); // baixo
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.pacman[0]);
    al_destroy_bitmap(sprites.pacman[1]);
    al_destroy_bitmap(sprites.pacman[2]);
    al_destroy_bitmap(sprites.pacman[3]);
    al_destroy_bitmap(sprites._sheet);
}

// ---------- AUDIO ----------

// ------------ MAPA --------------

#define TAM_X 30
#define TAM_Y 30

// 0 = vazio
// 1 = parede
// 2 = comida
// 3 = vitamina

int mapa[TAM_X][TAM_Y] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void mapa_init()
{
}
void mapa_update()
{
}

void mapa_draw()
{
    int i, j;
    float x, y, h = (float)BUFFER_H / (TAM_Y), w = (float)BUFFER_W / (TAM_X);
    for (i = 0; i < TAM_Y; i++)
    {
        for (j = 0; j < TAM_X; j++)
        {
            x = j * w;
            y = i * h;
            if (mapa[i][j] == 1) // paredes
                al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgba_f(0, 0, 0.5, 1));
            else if (mapa[i][j] == 2) // comida
                al_draw_filled_rectangle(x - 1.5 + w / 2, y - 1.5 + h / 2, x + 1.5 + w / 2, y + 1.5 + h / 2, al_map_rgba_f(1, 1, 1, 1));
            else if (mapa[i][j] == 3) // vitamina
                al_draw_filled_rectangle(x - 1.5 + w / 2, y - 1.5 + h / 2, x + 1.5 + w / 2, y + 1.5 + h / 2, al_map_rgba_f(1, 0, 0, 1));
        }
    }
}
// ------------ PACMAN ------------

#define PACMAN_SPEED 1
#define PACMAN_MAX_X (BUFFER_W - PACMAN_W)
#define PACMAN_MAX_Y (BUFFER_H - PACMAN_H)

typedef struct PACMAN
{
    int x, y;
    int dx, dy;
    int lives;
    int respawn_timer;
    int invincible_timer;
    int pos;
} PACMAN;
PACMAN pacman;

void pacman_init()
{
    pacman.x = 15 * (float)BUFFER_W / TAM_X;
    pacman.y = 20 * (float)BUFFER_H / TAM_Y;
    pacman.dx = 0;
    pacman.dy = 0;
    pacman.lives = 3;
    pacman.respawn_timer = 0;
    pacman.invincible_timer = 120;
    pacman.pos = 0;
}

void pacman_update()
{
    int prox_x, prox_y;
    if (pacman.lives < 0)
        return;

    if (pacman.respawn_timer)
    {
        pacman.respawn_timer--;
        return;
    }

    pacman.x += pacman.dx;
    pacman.y += pacman.dy;

    if (key[ALLEGRO_KEY_LEFT])
    {
        pacman.dx = -PACMAN_SPEED;
        pacman.dy = 0;
        pacman.pos = 1;
    }
    if (key[ALLEGRO_KEY_RIGHT])
    {
        pacman.dx = PACMAN_SPEED;
        pacman.dy = 0;
        pacman.pos = 0;
    }
    if (key[ALLEGRO_KEY_UP])
    {
        pacman.dy = -PACMAN_SPEED;
        pacman.dx = 0;
        pacman.pos = 2;
    }
    if (key[ALLEGRO_KEY_DOWN])
    {
        pacman.dy = PACMAN_SPEED;
        pacman.dx = 0;
        pacman.pos = 3;
    }

    // verifica a proxima posicao para testar colisao
    prox_x = (pacman.x + pacman.dx) / ((float)BUFFER_W / TAM_X);
    prox_y = (pacman.y + pacman.dy) / ((float)BUFFER_H / TAM_Y);
    if (mapa[prox_y][prox_x] == 1)
    {
        printf("[%d,%d][%d,%d]", pacman.x, pacman.y, prox_x, prox_y);
        pacman.dx = 0;
        pacman.dy = 0;
    }

    if (pacman.invincible_timer)
        pacman.invincible_timer--;
    else
    {
        // verifica colisao com fantasmas
    }
}

void pacman_draw()
{
    if (pacman.lives < 0)
        return;
    if (pacman.respawn_timer)
        return;
    if (((pacman.invincible_timer / 2) % 3) == 1)
        return;

    // al_draw_bitmap(sprites.pacman[pacman.pos], pacman.x * (float)BUFFER_W / TAM_X, pacman.y * (float)BUFFER_H / TAM_Y, 0);
    al_draw_bitmap(sprites.pacman[pacman.pos], pacman.x, pacman.y, 0);
}

// ---------- FANTASMAS ----------

// ---------- HUD ----------

// ---------- MAIN ----------

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    // audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();

    // hud_init();

    must_init(al_init_primitives_addon(), "primitives");

    // must_init(al_install_audio(), "audio");
    // must_init(al_init_acodec_addon(), "audio codecs");
    // must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    // fx_init();
    // shots_init();
    pacman_init();
    mapa_draw();
    // aliens_init();
    // stars_init();

    frames = 0;
    score = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            // fx_update();
            // shots_update();
            // stars_update();
            pacman_update();
            // aliens_update();
            // hud_update();

            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            redraw = true;
            frames++;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // stars_draw();
            // aliens_draw();
            // shots_draw();
            // fx_draw();
            mapa_draw();
            pacman_draw();

            // hud_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    // hud_deinit();
    // audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
