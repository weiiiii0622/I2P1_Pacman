// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_settings.h"
#include "scene_menu_object.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static Button bgm_0, bgm_25, bgm_50, bgm_75, bgm_100, effect_0, effect_25, effect_50, effect_75, effect_100, goback_menu, bgm_img, effect_img;
static Button *pre_bgm, *pre_effect;
static ALLEGRO_SAMPLE_ID settingBGM;

extern float music_volume;
extern float effect_volume;

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

void drawButton_s(Button button) {
    ALLEGRO_BITMAP* _img = button.hovered_img ?
                                                button.pressed ?
                                                    button.hovered_img :
                                                    button.default_img :
                                                button.default_img;
    al_draw_scaled_bitmap(
        _img,
        0, 0,
        al_get_bitmap_width(_img), al_get_bitmap_height(_img),
        button.body.x, button.body.y,
        button.body.w, button.body.h, 0
    );
}

static void init(void){
    bgm_0 = button_create(200, 100, 80, 80, "Assets/0_no.jpg", "Assets/0_yes.jpg");
    bgm_25 = button_create(300, 100, 80, 80, "Assets/25_no.jpg", "Assets/25_yes.jpg");
    bgm_50 = button_create(400, 100, 80, 80, "Assets/50_no.jpg", "Assets/50_yes.jpg");
    bgm_75 = button_create(500, 100, 80, 80, "Assets/75_no.jpg", "Assets/75_yes.jpg");
    bgm_100 = button_create(600, 100, 80, 80, "Assets/100_no.jpg", "Assets/100_yes.jpg");
    effect_0 = button_create(200, 220, 80, 80, "Assets/0_no.jpg", "Assets/0_yes.jpg");
    effect_25 = button_create(300, 220, 80, 80, "Assets/25_no.jpg", "Assets/25_yes.jpg");
    effect_50 = button_create(400, 220, 80, 80, "Assets/50_no.jpg", "Assets/50_yes.jpg");
    effect_75 = button_create(500, 220, 80, 80, "Assets/75_no.jpg", "Assets/75_yes.jpg");
    effect_100 = button_create(600, 220, 80, 80, "Assets/100_no.jpg","Assets/100_yes.jpg");
    goback_menu = button_create(270, 680, 250, 70, "Assets/goback_menu.jpg","Assets/goback_menu.jpg");
    bgm_img = button_create(50, 100, 90, 80, "Assets/bgm.jpg","Assets/bgm.jpg");
    effect_img = button_create(40, 220, 100, 80, "Assets/effect.jpg","Assets/effect.jpg");
    
    if(effect_volume>=0.5){
        if(effect_volume == 0.5){
            pre_effect = &effect_50;
        }
        else{
            pre_effect = effect_volume==0.75?&effect_75:&effect_100;
        }
    }
    else{
        pre_effect = effect_volume==0.25?&effect_25:&effect_0;
    }
    if(music_volume>=0.5){
        if(music_volume == 0.5){
            pre_bgm = &bgm_50;
        }
        else{
            pre_bgm = music_volume==0.75?&bgm_75:&bgm_100;
        }
    }
    else{
        pre_bgm = music_volume==0.25?&bgm_25:&bgm_0;
    }
    
    pre_bgm->pressed = true;
    pre_effect->pressed = true;
    
    stop_bgm(settingBGM);
    settingBGM = play_bgm(settingMusic, music_volume);
    return;
}
static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
    drawButton_s(bgm_0);
    drawButton_s(bgm_25);
    drawButton_s(bgm_50);
    drawButton_s(bgm_75);
    drawButton_s(bgm_100);
    drawButton_s(effect_0);
    drawButton_s(effect_25);
    drawButton_s(effect_50);
    drawButton_s(effect_75);
    drawButton_s(effect_100);
    drawButton_s(goback_menu);
    drawButton_s(bgm_img);
    drawButton_s(effect_img);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    bgm_0.hovered = buttonHover(bgm_0, mouse_x, mouse_y);
    bgm_25.hovered = buttonHover(bgm_25, mouse_x, mouse_y);
    bgm_50.hovered = buttonHover(bgm_50, mouse_x, mouse_y);
    bgm_75.hovered = buttonHover(bgm_75, mouse_x, mouse_y);
    bgm_100.hovered = buttonHover(bgm_100, mouse_x, mouse_y);
    effect_0.hovered = buttonHover(effect_0, mouse_x, mouse_y);
    effect_25.hovered = buttonHover(effect_25, mouse_x, mouse_y);
    effect_50.hovered = buttonHover(effect_50, mouse_x, mouse_y);
    effect_75.hovered = buttonHover(effect_75, mouse_x, mouse_y);
    effect_100.hovered = buttonHover(effect_100, mouse_x, mouse_y);
    goback_menu.hovered = buttonHover(goback_menu, mouse_x, mouse_y);
    
}

static void on_mouse_down() {
    int flag=0;
    if (bgm_0.hovered){flag=1;bgm_0.pressed = true; music_volume=0; pre_bgm->pressed=false; pre_bgm=&bgm_0;}
    else if (bgm_25.hovered){flag=1;bgm_25.pressed = true; music_volume=0.25; pre_bgm->pressed=false; pre_bgm=&bgm_25;}
    else if (bgm_50.hovered){flag=1;bgm_50.pressed = true; music_volume=0.5; pre_bgm->pressed=false; pre_bgm=&bgm_50;}
    else if (bgm_75.hovered){flag=1;bgm_75.pressed = true; music_volume=0.75; pre_bgm->pressed=false; pre_bgm=&bgm_75;}
    else if (bgm_100.hovered){flag=1;bgm_100.pressed = true; music_volume=1; pre_bgm->pressed=false; pre_bgm=&bgm_100;}
    
    else if (effect_0.hovered){flag=0;effect_0.pressed = true; effect_volume=0; pre_effect->pressed=false; pre_effect=&effect_0;}
    else if (effect_25.hovered){flag=0;effect_25.pressed = true; effect_volume=0.25; pre_effect->pressed=false; pre_effect=&effect_25;}
    else if (effect_50.hovered){flag=0;effect_50.pressed = true; effect_volume=0.5; pre_effect->pressed=false; pre_effect=&effect_50;}
    else if (effect_75.hovered){flag=0;effect_75.pressed = true; effect_volume=0.75; pre_effect->pressed=false; pre_effect=&effect_75;}
    else if (effect_100.hovered){flag=0;effect_100.pressed = true; effect_volume=1; pre_effect->pressed=false; pre_effect=&effect_100;}
    else if(goback_menu.hovered){
        game_change_scene(scene_menu_create());
    }
    
    if(flag){
        stop_bgm(settingBGM);
        settingBGM = play_bgm(settingMusic, music_volume);
    }
}

static void destroy() {
    stop_bgm(settingBGM);
    al_destroy_bitmap(bgm_0.default_img);
    al_destroy_bitmap(bgm_0.hovered_img);
    al_destroy_bitmap(bgm_25.default_img);
    al_destroy_bitmap(bgm_25.hovered_img);
    al_destroy_bitmap(bgm_50.default_img);
    al_destroy_bitmap(bgm_50.hovered_img);
    al_destroy_bitmap(bgm_75.default_img);
    al_destroy_bitmap(bgm_75.hovered_img);
    al_destroy_bitmap(bgm_100.default_img);
    al_destroy_bitmap(bgm_100.hovered_img);
    al_destroy_bitmap(effect_0.default_img);
    al_destroy_bitmap(effect_0.hovered_img);
    al_destroy_bitmap(effect_25.default_img);
    al_destroy_bitmap(effect_25.hovered_img);
    al_destroy_bitmap(effect_50.default_img);
    al_destroy_bitmap(effect_50.hovered_img);
    al_destroy_bitmap(effect_75.default_img);
    al_destroy_bitmap(effect_75.hovered_img);
    al_destroy_bitmap(effect_100.default_img);
    al_destroy_bitmap(effect_100.hovered_img);
    al_destroy_bitmap(goback_menu.default_img);
    al_destroy_bitmap(goback_menu.hovered_img);
    al_destroy_bitmap(bgm_img.default_img);
    al_destroy_bitmap(bgm_img.hovered_img);
    al_destroy_bitmap(effect_img.default_img);
    al_destroy_bitmap(effect_img.hovered_img);

}


// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
    scene.initialize = &init;
	scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_move = &on_mouse_move;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}
