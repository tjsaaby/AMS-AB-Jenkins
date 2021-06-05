/*
 * move_local_player_command.h
 *
 * Created: 15-04-2021 19:36:18
 *  Author: victo
 */

#ifndef MOVE_LOCAL_PLAYER_COMMAND_H_
#define MOVE_LOCAL_PLAYER_COMMAND_H_
#include "game_logic.h"

typedef enum MLPC_PlayerMoveDirection {
    MLPC_LEFT,
    MLPC_RIGHT,
} MLPC_PlayerMoveDirection;

typedef struct MLPC_MoveLocalPlayerCommandParameters {
    Game* p_game;
    MLPC_PlayerMoveDirection direction;
} MLPC_MoveLocalPlayerCommandParameters;

void MLPCMD_Execute(MLPC_MoveLocalPlayerCommandParameters params);

#endif /* MOVE_LOCAL_PLAYER_COMMAND_H_ */