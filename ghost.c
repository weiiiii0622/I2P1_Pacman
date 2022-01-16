#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"
#include "scene_game.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;

/* shared variables. */
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;

/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
static const int power_up_duration = 10;
// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 4;

Ghost* ghost_create(int flag) {

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case Blinky:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
    case Pinky:
        ghost->speed = 2;
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
        ghost->move_script = &ghost_pink_move_script;
        break;
    case Inky:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
        ghost->move_script = &ghost_blue_move_script;
        break;
    case Clyde:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_orange.png");
            ghost->move_script = &ghost_orange_move_script;
        break;
            
	default:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}
void ghost_destory(Ghost* ghost) {
	
		//[TODO]
		//free ghost resource
        al_destroy_bitmap(ghost->move_sprite);
        al_destroy_bitmap(ghost->flee_sprite);
        al_destroy_bitmap(ghost->dead_sprite);
		free(ghost);
	
}
void ghost_draw(Ghost* ghost) {
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);

	//Draw default image
//	al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
//		16, 16,
//		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
//		draw_region, draw_region, 0
//	);

	/*
		[TODO]
		Draw ghost according to its status
		hint : use ghost->objData.moveCD value to determine which frame of the animation to draw.

			A not so good way is:

			if(ghost->objData.moveCD % 16 == 0){
				al_draw_scaled_bitmap(...);
			}
			else if(ghost->objData.moveCD % 16 == 1){
				al_draw_scaled_bitmap(...);
			}...

			since modulo operation is expensive, better avoid using it.
	*/

	int bitmap_x_offset = 0;
	// [TODO] below is for animation usage, change the sprite you want to use.
	if (ghost->status == FLEE) {
        
        if((power_up_duration-ghost->previous_timer_val)<4){
            if(ghost->objData.moveCD >= 0 && ghost->objData.moveCD < 16){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      0, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
            else if(ghost->objData.moveCD >= 16 && ghost->objData.moveCD < 32){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      16, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
            else if(ghost->objData.moveCD >= 32 && ghost->objData.moveCD < 48){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      32, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
            else if(ghost->objData.moveCD >= 48){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      48, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
        }
        else{
            if(ghost->objData.moveCD >= 32){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      0, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
            else if(ghost->objData.moveCD < 32){
                al_draw_scaled_bitmap(ghost->flee_sprite,
                                      16, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            }
        }

	}
	else if (ghost->status == GO_IN) {
        int dir=0;
        switch (ghost->objData.facing)
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
        al_draw_scaled_bitmap(ghost->dead_sprite,
                              dir*16, 0,
                              16, 16,
                              drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                              draw_region, draw_region, 0
                              );
	}
	else {

        int dir=0;
		switch (ghost->objData.facing)
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
        int tick_time = (ghost->objData.moveCD)/32 % 2;
        al_draw_scaled_bitmap(ghost->move_sprite,
                              dir*32+tick_time*16, 0,
                              16, 16,
                              drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                              draw_region, draw_region, 0
                              );
	}
}
void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}
void printGhostStatus(GhostStatus S) {

	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.

	
    int index_x = ghost->objData.Coord.x;
    int index_y =  ghost->objData.Coord.y;

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

	if (is_wall_block(M, index_x, index_y) || (room && is_room_block(M, index_x, index_y)))
		return false;
	

	return true;
}

void ghost_toggle_STOP(Ghost* ghost, bool setSTOP) {
    
        if(setSTOP){
            if(ghost->status == FREEDOM || ghost->status == FLEE){
                ghost->speed = 0;
            }
        }else{
            if(ghost->status == FREEDOM){
                
                switch (ghost->typeFlag){
                    case Pinky:
                        ghost->speed = 2;
                        break;
                    default:
                        ghost->speed = 4;
                        break;
                }
            }
            else if(ghost->status == FLEE){
                ghost->speed = 2;
            }
        }
}

void ghost_toggle_SLOW(Ghost* ghost, bool setSLOW) {
    
        if(setSLOW){
            if(ghost->status == FREEDOM && ghost->speed != 0){
                switch (ghost->typeFlag){
                    case Pinky:
                        break;
                    default:
                        ghost->speed = 2;
                        break;
                }
            }
        }else{
            if(ghost->status == FREEDOM && ghost->speed != 0){
                
                switch (ghost->typeFlag){
                    case Pinky:
                        break;
                    default:
                        ghost->speed = 4;
                        break;
                }
            }
        }
}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
	// [TODO]
	// TODO: Here is reserved for power bean implementation.
	// The concept is "When pacman eats the power bean, only
	// ghosts who are in state FREEDOM will change to state FLEE.
	// For those who are not (BLOCK, GO_IN, etc.), they won't change state."
	// This implementation is based on the classic PACMAN game.
	// You are allowed to do your own implementation of power bean system.
	
		if(setFLEE){
			if(ghost->status == FREEDOM){
				ghost->status = FLEE;
                ghost->speed = 2;
			}
		}else{
            if(ghost->status == FLEE)
                ghost->status = FREEDOM;
            if(ghost->speed != 0){
                game_log("Ghost Normal");

                switch (ghost->typeFlag){
                    case Pinky:
                        ghost->speed = 2;
                        break;
                    default:
                        ghost->speed = 4;
                        break;
                }
            }
		}
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE) {
		ghost->status = GO_IN;
		ghost->speed = 4;
	}
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
    
    static Directions proba[4];
    static Directions prev_dir;
    // possible movement
    int cnt = 0;
    srand(time(NULL));
    for (Directions i = 1; i <= 4; i++)
        if (ghost_movable(ghost, M, i, 1) && (i+prev_dir)!=5 && i!=shortestDirection)     proba[cnt++] = i;
    if(cnt == 0){
        prev_dir = 5-prev_dir;
        ghost_NextMove(ghost, prev_dir);
    }
    else{
        prev_dir = proba[rand()%cnt];
        ghost_NextMove(ghost, prev_dir);
    }

}

