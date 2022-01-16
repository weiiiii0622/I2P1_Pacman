#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_gameover.h"
#include "scene_menu.h"
#include "shared.h"
#include "utility.h"

static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID gameoverBGM;

static void init(void);
static void draw(void);
static void destroy(void);
static void on_key_down(int key_code);


static void init(){
    stop_bgm(gameoverBGM);
    gameoverBGM = play_bgm(gameoverMusic, music_volume);
    
    return;
}

static void draw(void ){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    al_draw_text(
        gameoverFont,
        al_map_rgba(255, 0, 0, 100),
        SCREEN_W/2,
        SCREEN_H/2 - 50,
        ALLEGRO_ALIGN_CENTER,
        "GAME OVER"
    );
    al_draw_text(
        menuFont,
        al_map_rgba(255, 255, 255, 70),
        SCREEN_W/2,
        SCREEN_H - 80,
        ALLEGRO_ALIGN_CENTER,
        "PRESS ENTER TO CONTINUE"
    );
    
    return;
}

static void destroy() {
    stop_bgm(gameoverBGM);

    return;
}

static void on_key_down(int keycode) {

    switch (keycode) {
        case ALLEGRO_KEY_ENTER:
            game_change_scene(scene_menu_create());
            break;
        default:
            break;
    }
}

// The only function that is shared across files.
Scene scene_gameover_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "GAMEOVER";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;

    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Gameover scene created");
    return scene;
}
