// [shared.h]
// you should put shared variables between files in this header.

#ifndef SCENE_SHARED_H
#define SCENE_SHARED_H
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

// TODO: Shared constant variable (without initializing value).

// TODO: More shared resources or data that needed to be accessed
// across different scenes. initialize the value in 'shared.c'.;

extern ALLEGRO_BITMAP* clock_img;
extern ALLEGRO_BITMAP* speedboot_img;

extern ALLEGRO_FONT* font_pirulen_32;
extern ALLEGRO_FONT* font_pirulen_24;

extern ALLEGRO_SAMPLE* themeMusic;
extern ALLEGRO_SAMPLE* settingMusic;
extern ALLEGRO_SAMPLE* winMusic;
extern ALLEGRO_SAMPLE* gameoverMusic;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATFRUIT_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_INTERMISSION_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_POWERUP_BGM;
extern ALLEGRO_FONT* menuFont;
extern ALLEGRO_FONT* gameoverFont;
extern ALLEGRO_FONT* gamewinFont;
extern ALLEGRO_FONT* scoreboardFont;
extern int fontSize;
extern float music_volume;
extern float effect_volume;
extern bool gameDone;

// Initialize shared variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void shared_init(void);
// Free shared variables and resources.
void shared_destroy(void);

#endif
