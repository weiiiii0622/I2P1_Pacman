// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"
ALLEGRO_BITMAP* clock_img = NULL;
ALLEGRO_BITMAP* speedboot_img = NULL;

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* settingMusic = NULL;
ALLEGRO_SAMPLE* winMusic = NULL;
ALLEGRO_SAMPLE* gameoverMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_POWERUP_BGM = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EATFRUIT_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_INTERMISSION_SOUND = NULL;

ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* gameoverFont = NULL;
ALLEGRO_FONT* gamewinFont = NULL;
ALLEGRO_FONT* scoreboardFont = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
    speedboot_img = load_bitmap("Assets/speedboot.png");
    clock_img = load_bitmap("Assets/clock.png");
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
    gameoverFont = load_font("Assets/Minecraft.ttf", 70);
    gamewinFont = load_font("Assets/Minecraft.ttf", 50);
    scoreboardFont = load_font("Assets/Minecraft.ttf", 30);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
    settingMusic = load_audio("Assets/Music/setting_theme.ogg");
    winMusic = load_audio("Assets/Music/win_theme.ogg");
    gameoverMusic = load_audio("Assets/Music/gameover_theme.ogg");
    PACMAN_POWERUP_BGM = load_audio("Assets/Music/pacman_powerup.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
    PACMAN_EATFRUIT_SOUND = load_audio("Assets/Music/pacman_eatfruit.ogg");
    PACMAN_EATGHOST_SOUND = load_audio("Assets/Music/pacman_eatghost.ogg");
    PACMAN_INTERMISSION_SOUND = load_audio("Assets/Music/pacman_intermission.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
}

void shared_destroy(void) {
    
    al_destroy_bitmap(speedboot_img);
    al_destroy_bitmap(clock_img);
	al_destroy_font(menuFont);
    al_destroy_font(gameoverFont);
    al_destroy_font(gamewinFont);
    al_destroy_font(scoreboardFont);
	al_destroy_sample(themeMusic);
    al_destroy_sample(settingMusic);
    al_destroy_sample(winMusic);
    al_destroy_sample(gameoverMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_POWERUP_BGM);
    al_destroy_sample(PACMAN_EATFRUIT_SOUND);
    al_destroy_sample(PACMAN_EATGHOST_SOUND);
    al_destroy_sample(PACMAN_INTERMISSION_SOUND);
    al_destroy_sample(PACMAN_DEATH_SOUND);
}
