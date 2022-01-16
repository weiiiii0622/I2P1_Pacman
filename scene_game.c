#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_gamewin.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern ALLEGRO_SAMPLE_ID PACMAN_GAMESOUND_ID;
int game_main_Score = 0;
bool game_over = false;
bool win = false;
char randomevent[2];

/* Internal variables*/
static ALLEGRO_SAMPLE_ID gameBGM;
static ALLEGRO_TIMER* game_duration_timer;
static ALLEGRO_TIMER* power_up_timer;
static ALLEGRO_TIMER* skill_clock_timer;
static ALLEGRO_TIMER* skill_speedboot_timer;
static ALLEGRO_TIMER* event_pacman_speedup_timer;
static ALLEGRO_TIMER* event_pacman_slowdown_timer;
static ALLEGRO_TIMER* event_ghost_slowndown_timer;
static const int random_event_duration = 10;
static const int event_pacman_speedup_duration = 2;
static const int event_pacman_slowdown_duration = 2;
static const int event_ghost_slowdown_duration = 2;
static const int power_up_duration = 10;
static const int skill_clock_duration = 5;
static const int skill_speedboot_duration = 2;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void random_event(void);
static void pacman_speedup(void);
static void pacman_slowdown(void);
static void ghost_slowdown(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	game_main_Score = 0;
	// create map
	//basic_map = create_map(NULL);
	// [TODO]
	// Create map from .txt file and design your own map !!
	basic_map = create_map("Assets/test_map.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	
    ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
        srand(time(NULL));
		for (int i = 0; i < GHOST_NUM; i++) {
            //random ghost
            //int flag = rand() % 4;
			//game_log("creating ghost %d\n", flag);
			//ghosts[i] = ghost_create(flag);
            
            game_log("creating ghost %d\n", i);
            ghosts[i] = ghost_create(i%4);
			if (!ghosts[i])
				game_abort("error creating ghost\n");

		}
	}
	GAME_TICK = 0;
    randomevent[0] = '\0';
    
	render_init_screen();
    game_duration_timer = al_create_timer(1.0f);
    al_set_timer_count(game_duration_timer, 1);
    al_start_timer(game_duration_timer);
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
    skill_clock_timer = al_create_timer(1.0f);
    skill_speedboot_timer = al_create_timer(1.0f);
    event_pacman_speedup_timer = al_create_timer(1.0f);
    event_pacman_slowdown_timer = al_create_timer(1.0f);
    event_ghost_slowndown_timer = al_create_timer(1.0f);
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.

	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, '.');
        game_main_Score += 10;
        basic_map->beansCount -= 1;
        if(basic_map->beansCount == 0){
            win = true;
        }
        break;
    case 'P':
        stop_bgm(gameBGM);
        gameBGM = play_audio(PACMAN_POWERUP_BGM, music_volume);
        pacman_eatItem(pman, 'P');
        cheat_mode = true;
        pman->powerUp = true;
            al_set_timer_count(power_up_timer, 0);
        al_start_timer(power_up_timer);
        for(int i=0; i<GHOST_NUM; i++){
            ghost_toggle_FLEE(ghosts[i], true);
        }
        game_log("PowerUp");
        game_main_Score += 50;
        break;
    case 'C':
        game_main_Score += 50;
        pacman_eatItem(pman, 'C');
        break;
    case 'S':
        game_main_Score += 50;
        pacman_eatItem(pman, 'S');
        break;
	default:
		break;
	}
	
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.
    if(basic_map->map[Grid_y][Grid_x] != '#'){
        basic_map->map[Grid_y][Grid_x] = ' ';
    }
    
	
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
        int flag=0;
        for(int i=0; i<GHOST_NUM; i++){
            if(RecAreaOverlap(getDrawArea(ghosts[i]->objData, GAME_TICK_CD), getDrawArea(pman->objData, GAME_TICK_CD)) == true){
                
                
                if(cheat_mode){
                    if(ghosts[i]->status == FLEE){
                        ghost_collided(ghosts[i]);
                        flag = 1;
                    break;
                    }
                    else if(ghosts[i]->status == FREEDOM){
                       flag = 2;
                       break;
                    }
                    else if(ghosts[i]->status == GO_IN){
                        continue;
                    }
                }
                else{
                    flag = ghosts[i]->status == GO_IN?0:2;
                    break;
                }
                
            }
        }
        //game_log("flag=%d", flag);
        if(cheat_mode){
            if(flag == 1){
                game_log("KILL GHOST");
                game_main_Score += 200;
                pacman_eatItem(pman, 'G');
            }
            else if(flag == 2){
                game_log("collide with ghost\n");
                al_rest(1.0);
                pacman_die();
                game_over = true;
                break;
            }
            for(int i=0; i<GHOST_NUM; i++){
                ghosts[i]->previous_timer_val = al_get_timer_count(power_up_timer);
            }
            game_log("powerup countdown %d", (power_up_duration-ghosts[0]->previous_timer_val));
            if(al_get_timer_count(power_up_timer) >= power_up_duration){
                game_log("End of Powerup");
                al_stop_timer(power_up_timer);
                stop_bgm(gameBGM);
                for(int i=0; i<GHOST_NUM; i++){
                    ghost_toggle_FLEE(ghosts[i], false);
                }
                pman->powerUp = false;
                cheat_mode = false;
            }
        }
        
		if(!cheat_mode && flag)
		{
			game_log("collide with ghost\n");
			al_rest(1.0);
            stop_bgm(gameBGM);
			pacman_die();
			game_over = true;
			break;
		}
		
	}
}

static void update(void) {

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
        stop_bgm(gameBGM);
        al_start_timer(pman->death_anim_counter);
		return;
	}
    
    if(win){
        stop_bgm(gameBGM);
        win = false;
        game_change_scene(scene_gamewin_create());
    }
    
    if(randomevent[0]=='\0' && (al_get_timer_count(game_duration_timer)%random_event_duration) == 0){
        game_log("RandomEvent");
        randomevent[0] = 't';
        random_event();
    }
    if(event_pacman_slowdown_timer != NULL && al_get_timer_count(event_pacman_slowdown_timer) >= event_pacman_slowdown_duration){
        game_log("retrun normal pacman speed");
        al_stop_timer(event_pacman_slowdown_timer);
        al_set_timer_count(event_pacman_slowdown_timer, 0);
        pman->speed = 4;
        randomevent[0] = '\0';
    }
    if(event_pacman_speedup_timer != NULL && al_get_timer_count(event_pacman_speedup_timer) >= event_pacman_speedup_duration){
        game_log("retrun normal pacman speed");
        al_stop_timer(event_pacman_speedup_timer);
        al_set_timer_count(event_pacman_speedup_timer, 0);
        pman->speed = 4;
        randomevent[0] = '\0';
    }
    if(event_ghost_slowndown_timer != NULL && al_get_timer_count(event_ghost_slowndown_timer) >= event_ghost_slowdown_duration){
        al_stop_timer(event_ghost_slowndown_timer);
        al_set_timer_count(event_ghost_slowndown_timer, 0);
        for(int i=0; i<GHOST_NUM; i++){
            ghost_toggle_SLOW(ghosts[i], false);
        }
        randomevent[0] = '\0';
    }
    if(skill_clock_timer != NULL && al_get_timer_count(skill_clock_timer) >= skill_clock_duration){
        al_stop_timer(skill_clock_timer);
        al_set_timer_count(skill_clock_timer, 0);
        for(int i=0; i<GHOST_NUM; i++){
            ghost_toggle_STOP(ghosts[i], false);
        }
    }
    if(skill_clock_timer != NULL && al_get_timer_count(skill_speedboot_timer) >= skill_speedboot_duration){
        al_stop_timer(skill_speedboot_timer);
        al_set_timer_count(skill_speedboot_timer, 0);
        pman->speed = 4;
    }

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	/*
		al_draw_text(...);
	*/
    
    if((random_event_duration - (al_get_timer_count(game_duration_timer)%(random_event_duration))) <= 5){
        al_draw_textf(
            scoreboardFont,
            al_map_rgba(255, 255, 255, 100),
            SCREEN_W/2+100,
            20,
            ALLEGRO_ALIGN_CENTER,
            "Random Event Comes In %d sec...", (random_event_duration - (al_get_timer_count(game_duration_timer)%(random_event_duration)))
        );
    }
    
    if(randomevent[0] == 'p'){
        al_draw_text(
            scoreboardFont,
            al_map_rgba(0, 255, 0, 100),
            SCREEN_W/2+100,
            20,
            ALLEGRO_ALIGN_CENTER,
            "Pacman Speed UP!"
        );
    }
    else if(randomevent[0] == 'g'){
        al_draw_text(
            scoreboardFont,
            al_map_rgba(0, 255, 0, 100),
            SCREEN_W/2+100,
            20,
            ALLEGRO_ALIGN_CENTER,
            "GHOST SLOW DOWN!"
        );
    }
    else if(randomevent[0] == 's'){
        al_draw_text(
            scoreboardFont,
            al_map_rgba(255, 0, 0, 100),
            SCREEN_W/2+100,
            20,
            ALLEGRO_ALIGN_CENTER,
            "PACMAN SLOW DOWN!"
        );
    }
    
    al_draw_textf(
        scoreboardFont,
        al_map_rgba(255, 255, 255, 100),
        20,
        20,
        ALLEGRO_ALIGN_LEFT,
        "Score: %d", game_main_Score
    );
    
    switch((pman->skill)[0]){
        case 'c':
            al_draw_textf(
                scoreboardFont,
                al_map_rgba(255, 255, 255, 100),
                SCREEN_W/2,
                SCREEN_H-50,
                ALLEGRO_ALIGN_CENTER,
                "Press \"Q\" to use: %s", "Stopwatch"
            );
            break;
        case 's':
            al_draw_textf(
                scoreboardFont,
                al_map_rgba(255, 255, 255, 100),
                SCREEN_W/2,
                SCREEN_H-50,
                ALLEGRO_ALIGN_CENTER,
                "Press \"Q\" to use: %s", "Speedboot"
            );
            break;
        default:
            al_draw_text(
                scoreboardFont,
                al_map_rgba(255, 255, 255, 100),
                SCREEN_W/2,
                SCREEN_H-50,
                ALLEGRO_ALIGN_CENTER,
                "You have no skill yet"
            );
            break;
    }

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
    for (int i = 0; i < GHOST_NUM; i++){
		ghost_draw(ghosts[i]);
    }
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void random_event(void){
    srand(time(NULL));
    int choice = (rand()%100)+1;
    if(choice > 0 && choice <= 40){
        randomevent[0] = 'p';
        game_log("pacman speedup");
        pacman_speedup();
        return;
    }
    else if(choice > 40 && choice <= 80){
        randomevent[0] = 'g';
        game_log("ghost_slowdown");
        ghost_slowdown();
        return;
    }
    else if(choice > 80 && choice <= 100){
        randomevent[0] = 's';
        game_log("pacman_slowdown");
        pacman_slowdown();
        return;
    }
}

static void pacman_speedup(void){
    al_start_timer(event_pacman_speedup_timer);
    pman->speed = 8;
}

static void pacman_slowdown(void){
    al_start_timer(event_pacman_slowdown_timer);
    pman->speed = 2;
}

static void ghost_slowdown(void){
    al_start_timer(event_ghost_slowndown_timer);
    for(int i=0; i<GHOST_NUM; i++){
        ghost_toggle_SLOW(ghosts[i], true);
    }
}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
    delete_map(basic_map);
    pacman_destory(pman);
    for(int i=0; i<GHOST_NUM; i++){
        ghost_destory(ghosts[i]);
    }
    stop_bgm(gameBGM);
    al_destroy_timer(game_duration_timer);
    al_destroy_timer(skill_clock_timer);
    al_destroy_timer(skill_speedboot_timer);
    al_destroy_timer(event_pacman_speedup_timer);
    al_destroy_timer(event_pacman_slowdown_timer);
    al_destroy_timer(event_ghost_slowndown_timer);
    al_destroy_timer(power_up_timer);
}

static void on_key_down(int key_code) {

	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, 1);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, 2);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, 4);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, 3);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
        case ALLEGRO_KEY_Q:
            switch((pman->skill)[0]){
                case 'c':
                    game_log("Use Stopwatch");
                    al_set_timer_count(skill_clock_timer, 0);
                    al_start_timer(skill_clock_timer);
                    for(int i=0; i<GHOST_NUM; i++){
                        ghost_toggle_STOP(ghosts[i], true);
                    }
                    (pman->skill)[0] = '\0';
                    break;
                case 's':
                    game_log("Use Speedboot");
                    al_set_timer_count(skill_speedboot_timer, 0);
                    al_start_timer(skill_speedboot_timer);
                    pman->speed = 8;
                    (pman->skill)[0] = '\0';
                    break;
            default:
                break;
            }
            break;
		
        default:
            break;
	}
}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
