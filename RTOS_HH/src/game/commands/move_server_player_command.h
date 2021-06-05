/*
 * move_server_player_command.h
 *
 * Created: 15-04-2021 19:26:14
 *  Author: victo
 */

#ifndef MOVE_SERVER_PLAYER_COMMAND_H_
#define MOVE_SERVER_PLAYER_COMMAND_H_

#include "game_logic.h"

typedef struct MSPC_MoveServerPlayerCommandParameters {
    Game* p_game;
    int playerServerId;
    Coordinate coordinates;
} MSPC_MoveServerPlayerCommandParameters;

void MSPCMD_Execute(MSPC_MoveServerPlayerCommandParameters params);

#endif /* MOVE_SERVER_PLAYER_COMMAND_H_ */