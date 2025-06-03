#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

long frames;
long score;
int count;

#define TAM 31

// 0 = vazio
// 1 = parede
// 2 = comida
// 3 = vitamina

unsigned char mapa[TAM][TAM] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 1,
    1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 0, 0, 0, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 2, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 0, 1,
    1, 2, 2, 2, 2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 2, 2, 2, 2, 0, 1,
    1, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 1,
    1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};


void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void newRandSeed(){
    int seed = time(NULL);
    srand(seed);
}

int between(int lo, int hi)
{
    //newRandSeed();
    return lo + (rand() % (hi - lo));
}

float between_f(float lo, float hi)
{
    //newRandSeed();
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

//verificar colisão.

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}

//definindo propriedades da janela

#define BUFFER_W 496
#define BUFFER_H 496

#define DISP_SCALE 2
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

//O MAPA DEVE SER QUADRADO, OU SEJA, BUFFER_H = BUFFER_W. A MATRIZ DO MAPA É SEMPRE QUADRADA
#define BLOCK_SIZE (BUFFER_H/TAM)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 2, ALLEGRO_SUGGEST);

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

// Keboard input

#define KEY_SEEN     1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
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

// configuring sprites

#define PLAYER_W 13
#define PLAYER_H 13

#define LIFE_W 10
#define LIFE_H 11

#define ALIEN_SIZE 16

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3


typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* player[8];
    ALLEGRO_BITMAP* life;

    ALLEGRO_BITMAP* alien[32];

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* powerup[4];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.player[0] = sprite_grab(0, 0, PLAYER_W, PLAYER_H);
    sprites.player[1] = sprite_grab(16, 0, PLAYER_W, PLAYER_H);
    sprites.player[2] = sprite_grab(0, 16, PLAYER_W, PLAYER_H);
    sprites.player[3] = sprite_grab(16, 16, PLAYER_W, PLAYER_H);
    sprites.player[4] = sprite_grab(0, 32, PLAYER_W, PLAYER_H);
    sprites.player[5] = sprite_grab(16, 32, PLAYER_W, PLAYER_H);
    sprites.player[6] = sprite_grab(0, 47, PLAYER_W, PLAYER_H);
    sprites.player[7] = sprite_grab(16, 47, PLAYER_W, PLAYER_H);

    sprites.life = sprite_grab(131, 17, LIFE_W, LIFE_H);

    sprites.alien[0] = sprite_grab(0, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[1] = sprite_grab(16, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[2] = sprite_grab(32, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[3] = sprite_grab(48, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[4] = sprite_grab(64, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[5] = sprite_grab(80, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[6] = sprite_grab(94, 64, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[7] = sprite_grab(112, 64, ALIEN_SIZE, ALIEN_SIZE);

    sprites.alien[8] = sprite_grab(0, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[9] = sprite_grab(16, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[10] = sprite_grab(32, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[11] = sprite_grab(48, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[12] = sprite_grab(64, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[13] = sprite_grab(80, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[14] = sprite_grab(94, 80, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[15] = sprite_grab(112, 80, ALIEN_SIZE, ALIEN_SIZE);

    sprites.alien[16] = sprite_grab(0, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[17] = sprite_grab(16, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[18] = sprite_grab(32, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[19] = sprite_grab(48, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[20] = sprite_grab(64, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[21] = sprite_grab(80, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[22] = sprite_grab(94, 96, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[23] = sprite_grab(112, 96, ALIEN_SIZE, ALIEN_SIZE);

    sprites.alien[24] = sprite_grab(0, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[25] = sprite_grab(16, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[26] = sprite_grab(32, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[27] = sprite_grab(48, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[28] = sprite_grab(64, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[29] = sprite_grab(80, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[30] = sprite_grab(94, 112, ALIEN_SIZE, ALIEN_SIZE);
    sprites.alien[31] = sprite_grab(112, 112, ALIEN_SIZE, ALIEN_SIZE);


    sprites.explosion[0] = sprite_grab(127, 175, 17, 16);
    sprites.explosion[1] = sprite_grab(127, 191, 17, 16);
    sprites.explosion[2] = sprite_grab(127, 175, 17, 16);
    sprites.explosion[3] = sprite_grab(127, 191, 17, 16);

    sprites.sparks[0] = sprite_grab(144, 175, 17, 16);
    sprites.sparks[1] = sprite_grab(144, 191, 17, 16);
    sprites.sparks[2] = sprite_grab(144, 175, 17, 16);

    sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
    sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
    sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
    sprites.powerup[3] = sprite_grab(30, 49, 9, 12);
}

void sprites_deinit()
{
    for(int i = 0; i < 32; i++){
        al_destroy_bitmap(sprites.player[i]);
    }
    
    al_destroy_bitmap(sprites.sparks[0]);
    al_destroy_bitmap(sprites.sparks[1]);
    al_destroy_bitmap(sprites.sparks[2]);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites.powerup[0]);
    al_destroy_bitmap(sprites.powerup[1]);
    al_destroy_bitmap(sprites.powerup[2]);
    al_destroy_bitmap(sprites.powerup[3]);

    al_destroy_bitmap(sprites._sheet);
}

// audio configuration

ALLEGRO_SAMPLE* sample_explode[2];

void audio_init()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);


    sample_explode[0] = al_load_sample("explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");
}

void audio_deinit()
{
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
}

// FX

typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init()
{
    for(int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(bool spark, int x, int y)
{
    if(!spark)
        al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

    for(int i = 0; i < FX_N; i++)
    {
        if(fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].spark = spark;
        fx[i].used = true;
        return;
    }
}

void fx_update()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        fx[i].frame++;

        if((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2)))
        || ( fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2)))
        )
            fx[i].used = false;
    }
}

void fx_draw()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 2;
        ALLEGRO_BITMAP* bmp =
            fx[i].spark
            ? sprites.sparks[frame_display]
            : sprites.explosion[frame_display]
        ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}

// Player

#define PLAYER_MAX_X (BUFFER_W - PLAYER_W)
#define PLAYER_MAX_Y (BUFFER_H - PLAYER_H)

typedef struct PLAYER
{
    float speed;
    int direction;
    int x, y;
    int lives;
    int respawn_timer;
    int invincible_timer;
    //armazena a posição do bloco a frente na direção do pacman
    int nextX;
    int nextY;
} PLAYER;
PLAYER player;

void player_init()
{
    //VELOCIDADE DO PACMAN
    player.speed = (7/4);
    player.direction = 0;
    //POSICAO DE COMEÇO DO JOGADOR
    player.x = (BUFFER_W / 2) - (PLAYER_W / 2);
    player.y = 368 - (PLAYER_H / 2);
    player.lives = 3;
    player.respawn_timer = 0;
    player.invincible_timer = 120;
    player.nextX = 0;
    player.nextY = 0;
}

//verifica se foi comida ou vitamina, aplica os efeitos e limpa do mapa
void comida(int y, int x){
    if(mapa[y][x] == 2){
        score += 10;
        mapa[y][x] = 0;
        count++;
    }
    else if(mapa[y][x] == 3){
        score += 100;
        mapa[y][x] = 0;
        count++;
    }
}

//verifica se o player ou alien colidiu com a parede
bool wall_collision(bool pacman, int direction){
    int bs = BLOCK_SIZE;
    
    if(pacman){
        int ps = PLAYER_H;
        if(mapa[player.y/bs][player.x/bs] == 1
        || mapa[player.y/bs][(player.x + ps)/bs] == 1
        || mapa[(player.y +ps)/bs][player.x/bs] == 1
        || mapa[(player.y +ps)/bs][(player.x + ps)/bs] == 1)
        {
            switch(direction){
                case 0:
                    player.x -= player.speed;
                    break;
                case 2:
                    player.x += player.speed;
                    break;
                case 4:
                    player.y += player.speed;
                    break;
                case 6:
                    player.y -= player.speed;
                    break;
                default:
                    printf("Direcao do Pacman inválida!");
                    exit(1);
            }
        }

        //verificar se colidiu com comida ou vitamina
        int item = 0, tx, ty;
        ty = (player.y - (bs/2))/bs; tx = (player.x - (bs/2))/bs;
        if(mapa[ty][tx] > 1)
            comida(ty, tx);
        ty = (player.y - (bs/2))/bs; tx = (player.x + (bs/2))/bs;
        if(mapa[ty][tx] > 1)
            comida(ty, tx);
        ty = (player.y + (bs/2))/bs; tx = (player.x - (bs/2))/bs;
        if(mapa[ty][tx] > 1)
            comida(ty, tx);
        ty = (player.y + (bs/2))/bs; tx = (player.x + (bs/2))/bs;
        if(mapa[ty][tx] > 1)
            comida(ty, tx);
            

    }else{ //alien
        for(int n = 0; n < 4; n++){

        }
    }
}

void player_update()
{

    if(player.lives < 0)
        return;

    if(player.respawn_timer)
    {
        player.respawn_timer--;
        return;
    }
    if(player.invincible_timer >0)
        player.invincible_timer--;


    if(key[ALLEGRO_KEY_LEFT]){
        player.direction = 2;
    } 
    if(key[ALLEGRO_KEY_RIGHT]){
        player.direction = 0;  
    }   
    if(key[ALLEGRO_KEY_UP]){
        player.direction = 4;
    }   
    if(key[ALLEGRO_KEY_DOWN]){
        player.direction = 6;
    }

    switch(player.direction){
        case 0:
            player.x += player.speed;
            break;
        case 2:
            player.x -= player.speed;
            break;
        case 4:
            player.y -= player.speed;
            break;
        case 6:
            player.y += player.speed;
            break;
        default:
            printf("Direcao do Pacman inválida!");
            exit(1);
    }
    wall_collision(true, player.direction);

    if(player.x < 0)
        player.x = 0;
    if(player.y < 0)
        player.y = 0;

    if(player.x > PLAYER_MAX_X)
        player.x = PLAYER_MAX_X;
    if(player.y > PLAYER_MAX_Y)
        player.y = PLAYER_MAX_Y;
}

void player_draw()
{
    if(player.lives < 0)
        return;
    if(player.respawn_timer)
        return;
    if(((player.invincible_timer / 2) % 3) == 1)
        return;

    al_draw_bitmap(sprites.player[player.direction + ((frames/10)%2)], player.x, player.y, 0);
}

// Inimigos, naves aliens

typedef enum ALIEN_T
{
    ALIEN_VERMELHO = 0,
    ALIEN_ROSA,
    ALIEN_CIANO,
    ALIEN_LARANJA,
} ALIEN_T;

typedef struct ALIEN
{
    int x, y, invincible_timer, direction;
    ALIEN_T type;
    float speed;
    bool vivo, run;
} ALIEN;

#define ALIENS_N 4
ALIEN aliens[ALIENS_N];

void aliens_init()
{
    for(int i = 0; i < ALIENS_N; i++){
        aliens[i].invincible_timer = 0;
        aliens[i].vivo = false;
        aliens[i].run = false;
        aliens[i].type = i;
        aliens[i].direction = 0;
    }
}

void aliens_update()
{
    int bs = BLOCK_SIZE;
    for(int i = 0; i < ALIENS_N; i++)
    {
        if(!aliens[i].vivo)
        {
            aliens[i].y = 10*bs;
            switch(aliens[i].type)
            {
                case ALIEN_VERMELHO:
                    aliens[i].x = 12*bs;
                    break;
                case ALIEN_ROSA:
                    aliens[i].x = 14*bs;
                    break;
                case ALIEN_CIANO:
                    aliens[i].x = 16*bs;
                    break;
                case ALIEN_LARANJA:
                    aliens[i].x = 18*bs;
            }
            aliens[i].vivo = true; 
        }

        if(aliens[i].invincible_timer)
            aliens[i].invincible_timer--;

        //verifica colisão player com alien (fantasma)
        if(collide(aliens[i].x, aliens[i].y, aliens[i].x+ALIEN_SIZE, aliens[i].y+ALIEN_SIZE, player.x, player.y, player.x+PLAYER_W, player.y+PLAYER_H)){
            if(aliens[i].invincible_timer){
                continue;
            }else if(player.invincible_timer){
                continue;
            }else if(aliens[i].run){
                aliens[i].vivo = false;
            }else{
                player.lives--;
                fx_add(false, player.x+(PLAYER_H/2), player.y+(PLAYER_H/2));
                fx_add(false, player.x+4,  player.y-10);
                fx_add(false, player.x-8,  player.y+8);
                player.x = (BUFFER_W / 2) - (PLAYER_W / 2);
                player.y = 368 - (PLAYER_H / 2);
                player.invincible_timer = 180;
                player.respawn_timer = 60;
                player.direction = 0;
                continue;
            }
        }    

        int cx = aliens[i].x + (ALIEN_SIZE / 2);
        int cy = aliens[i].y + (ALIEN_SIZE / 2);

        if(!aliens[i].vivo)
        {
            fx_add(false, cx, cy);

            switch(aliens[i].type)
            {
                case ALIEN_VERMELHO:
                    score += 200;
                    break;

                case ALIEN_ROSA:
                    fx_add(false, cx+4,  cy-4);
                    score += 400;
                    break;

                case ALIEN_CIANO:
                    score += 800;
                    fx_add(false, cx-10, cy-4);
                    fx_add(false, cx+4,  cy+10);
                    fx_add(false, cx+8,  cy+8);
                    break;
                case ALIEN_LARANJA:
                    score += 1600;
                    fx_add(false, cx-10, cy-4);
                    fx_add(false, cx+4,  cy+10);
                    fx_add(false, cx+8,  cy+8);
                    fx_add(false, cx+10, cy+4);
                    fx_add(false, cx-4,  cy-10);
                    fx_add(false, cx-8,  cy-8);
            }
            continue;
        }

    }
}

void aliens_draw()
{
    for(int i = 0; i < ALIENS_N; i++)
    {
        if(((aliens[i].invincible_timer / 2) % 3) == 1)
            continue;
        al_draw_bitmap(sprites.alien[i*8 + aliens[i].direction*2 + ((frames/10)%2)], aliens[i].x, aliens[i].y, 0);
    }
}

// BG particle FX

typedef struct STAR
{
    float y;
    float speed;
} STAR;

#define STARS_N ((BUFFER_W / 2) - 1)
STAR stars[STARS_N];

void stars_init()
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y = between_f(0, BUFFER_H);
        stars[i].speed = between_f(0.1, 1);
    }
}

void stars_update()
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y += stars[i].speed;
        if(stars[i].y >= BUFFER_H)
        {
            stars[i].y = 0;
            stars[i].speed = between_f(0.1, 1);
        }
    }
}

void stars_draw()
{
    float star_x = 1.5;
    for(int i = 0; i < STARS_N; i++)
    {
        float l = stars[i].speed * 0.8;
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(l,l,l));
        star_x += 2;
    }
}

//HUD

ALLEGRO_FONT* font;
long score_display;

void hud_init()
{
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit()
{
    al_destroy_font(font);
}

void hud_update()
{
    if(frames % 2)
        return;

    for(long i = 5; i > 0; i--)
    {
        long diff = 1 << i;
        if(score_display <= (score - diff))
            score_display += diff;
    }
}

void hud_draw()
{
    al_draw_textf(
        font,
        al_map_rgb_f(1,1,1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    int spacing = LIFE_W + 1;
    for(int i = 0; i < player.lives; i++)
        al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

    if(player.lives < 0)
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, BUFFER_H / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );
}

void map_draw()
{
    int i, j;
    float x, y, bs = BLOCK_SIZE;
    for (j = 0; j < TAM; j++)
    {
        for (i = 0; i < TAM; i++)
        {
            y = i * bs;
            x = j * bs;

            if (mapa[i][j] == 1) // paredes
                al_draw_filled_rectangle(x, y, x + bs, y + bs, al_map_rgba_f(0, 0, 0.5, 1));
            else if (mapa[i][j] == 2) // comida
                al_draw_filled_rectangle(x + (bs*0.75), y + (bs*0.75), x + (bs*1.25), y + (bs*1.25), al_map_rgba_f(1, 1, 1, 1));
            else if (mapa[i][j] == 3) // vitamina
                al_draw_filled_rectangle(x + (bs*0.5), y + (bs*0.5), x + (bs*1.5), y + (bs*1.5), al_map_rgba_f(1, 0.753, 0.753, 1));
            
        }
    }
}


// COMECAR O JOGO
int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();

    hud_init();

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    fx_init();
    player_init();
    aliens_init();
    stars_init();

    frames = 0;
    score = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                fx_update();
                stars_update();
                player_update();
                aliens_update();
                hud_update();

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                redraw = true;
                frames++;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        keyboard_update(&event);
        if(redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0,0,0));
            
            stars_draw();
            map_draw();
            aliens_draw();
            fx_draw();
            player_draw();

            hud_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}