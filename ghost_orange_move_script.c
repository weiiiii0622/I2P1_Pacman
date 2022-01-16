
#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/* Shared variables */
#define GO_OUT_TIME 256+160
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
static void ghost_orange_move_script_FREEDOM(Ghost* ghost, Map* M, Pacman* pman);
static void ghost_orange_move_script_BLOCKED(Ghost* ghost, Map* M);

static void ghost_orange_move_script_FREEDOM(Ghost* ghost, Map* M, Pacman* pman) {

    
    Directions shortestDirection = shortest_path_direc_orange(M, pman->objData.facing, ghost->objData.Coord.x, ghost->objData.Coord.y, pman->objData.Coord.x, pman->objData.Coord.y);
    
    ghost_NextMove(ghost, shortestDirection);
    
}

static void ghost_orange_move_script_BLOCKED(Ghost* ghost, Map* M) {

    switch (ghost->objData.preMove)
    {
    case UP:
        if (ghost->objData.Coord.y == 10)
            ghost_NextMove(ghost, DOWN);
        else
            ghost_NextMove(ghost, UP);
        break;
    case DOWN:
        if (ghost->objData.Coord.y == 12)
            ghost_NextMove(ghost, UP);
        else
            ghost_NextMove(ghost, DOWN);
        break;
    default:
        ghost_NextMove(ghost, UP);
        break;
    }
}

void ghost_orange_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;
        switch (ghost->status)
        {
        case BLOCKED:
            ghost_orange_move_script_BLOCKED(ghost, M);
            if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
                ghost->status = GO_OUT;
            break;
        case FREEDOM:
            ghost_orange_move_script_FREEDOM(ghost, M, pacman);
            break;
        case GO_OUT:
            ghost_move_script_GO_OUT(ghost, M);
            break;
        case GO_IN:
            ghost_move_script_GO_IN(ghost, M);
            if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B'){
                ghost_orange_move_script_BLOCKED(ghost, M);
                if(ghost->revive_timer==NULL){
                    ghost->revive_timer = al_create_timer(1.0f / 64);
                    al_start_timer(ghost->revive_timer);
                }
                else if(al_get_timer_count(ghost->revive_timer) == 0){
                    al_start_timer(ghost->revive_timer);
                }
                else{
                    game_log("revive_timer=%d", al_get_timer_count(ghost->revive_timer));
                    if(al_get_timer_count(ghost->revive_timer) >= GO_OUT_TIME){
                        al_stop_timer(ghost->revive_timer);
                        al_set_timer_count(ghost->revive_timer, 0);
                        
                        ghost->status = GO_OUT;
                        ghost->speed = 4;
                    }
                }

            }
            break;
        case FLEE:
            ghost_move_script_FLEE(ghost, M, pacman);
            break;
        default:
            break;
        }

        if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
            ghost->objData.preMove = ghost->objData.nextTryMove;
            ghost->objData.nextTryMove = NONE;
        }
        else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
            return;

        switch (ghost->objData.preMove) {
        case RIGHT:
            ghost->objData.Coord.x += 1;
            break;
        case LEFT:
            ghost->objData.Coord.x -= 1;
            break;
        case UP:
            ghost->objData.Coord.y -= 1;
            break;
        case DOWN:
            ghost->objData.Coord.y += 1;
            break;
        default:
            break;
        }
        ghost->objData.facing = ghost->objData.preMove;
        ghost->objData.moveCD = GAME_TICK_CD;
}
