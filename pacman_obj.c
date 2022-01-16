#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
#include "shared.h"
#include "scene_gameover.h"

/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
static ALLEGRO_SAMPLE_ID PACMAN_GAMESOUND_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 4;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATFRUIT_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_POWERUP_BGM;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

	
    int index_x = pacman->objData.Coord.x;
    int index_y = pacman->objData.Coord.y;
	
	switch (targetDirec)
	{
	case UP:
        index_y -= 1;
        break;
	case DOWN:
        index_y += 1;
        break;
	case LEFT:
        index_x -= 1;
        break;
	case RIGHT:
        index_x += 1;
        break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}
	if (is_wall_block(M, index_x, index_y) || is_room_block(M, index_x, index_y))
		return false;
	
	return true;
}

Pacman* pacman_create() {

	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
	/*
		Pacman* pman = ...
		if(!pman)
			return NULL;
	*/
	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	pman->death_anim_counter = al_create_timer(1.0f / 64);
	pman->powerUp = false;
	/* load sprites */
	pman->move_sprite = load_bitmap("Assets/pacman_move.png");
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	return pman;

}

void pacman_destory(Pacman* pman) {
	
		//[TODO]
		//free pacman resource
		al_destroy_bitmap(pman->move_sprite);
        al_destroy_bitmap(pman->die_sprite);
		al_destroy_timer(pman->death_anim_counter);
		free(pman);
	
}


void pacman_draw(Pacman* pman) {
	/*
		[HW-TODO ]
		Draw Pacman and animations
		hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);

	//Draw default image
//	al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
//		16, 16,
//		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
//		draw_region, draw_region, 0
//	);
	
	if (game_over) {
		/*
			hint: instead of using pman->objData.moveCD, use Pacman's death_anim_counter to create animation
		*/
        int dead_time = al_get_timer_count(pman->death_anim_counter)/5;
        al_draw_scaled_bitmap(pman->die_sprite,
                              dead_time*16, 0,
                              16, 16,
                              drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                              draw_region, draw_region, 0
                              );
        if(dead_time == 12){
            game_log("Print GameOver");
            game_change_scene(scene_gameover_create());
        }
	}
	else {
        int dir=0;
        switch (pman->objData.facing)
        {
            case RIGHT:
                dir=0;
                break;
                
            case LEFT:
                dir=1;
                break;
                
            case UP:
                dir=2;
                break;
                
            case DOWN:
                dir=3;
                break;
                
            default:
                break;
        }
        int tick_time = (pman->objData.moveCD)/32 % 2;
        al_draw_scaled_bitmap(pman->move_sprite,
                              dir*32+tick_time*16, 0,
                              16, 16,
                              drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                              draw_region, draw_region, 0
                              );
	}
}
void pacman_move(Pacman* pacman, Map* M) {
    //if(pacman->powerUp == false) stop_bgm(PACMAN_GAMESOUND_ID);
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
	if (pacman_movable(pacman, M, pacman->objData.nextTryMove)) 
		pacman->objData.preMove = pacman->objData.nextTryMove;
	else if (!pacman_movable(pacman, M, pacman->objData.preMove)) 
		return;

	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove;
	pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item) {
    
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		break;
    case 'P':
        game_log("Eat Power Bean");
        //stop_bgm(PACMAN_GAMESOUND_ID);
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_EATFRUIT_SOUND, effect_volume);
        //PACMAN_GAMESOUND_ID = play_audio(PACMAN_POWERUP_BGM, music_volume);
        break;
    case 'G':
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_EATGHOST_SOUND, effect_volume);
        break;
    case 'C':
        game_log("Eat Clock");
        (pacman->skill)[0] = 'c';
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_EATFRUIT_SOUND, effect_volume);
        break;
    case 'S':
        game_log("Eat Speedboot");
        (pacman->skill)[0] = 's';
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_EATFRUIT_SOUND, effect_volume);
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
    if(pacman->powerUp == false) stop_bgm(PACMAN_GAMESOUND_ID);
	pacman->objData.nextTryMove = next;
}

void pacman_die() {
    //stop_bgm(PACMAN_GAMESOUND_ID);
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}





