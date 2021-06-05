/*
 * move_local_player_command.c
 *
 * Created: 15-04-2021 19:40:06
 *  Author: victo
 */
#include "move_local_player_command.h"

#include "player.h"
#include "uart_int.h"

void MLPCMD_Execute(MLPC_MoveLocalPlayerCommandParameters params) {
    Player* player = &params.p_game->localPlayer;
    if (params.direction == MLPC_LEFT)
        PLYR_MoveLeft(player);
    else if (params.direction == MLPC_RIGHT)
        PLYR_MoveRight(player);
    else
        UART_SendString("Invalid player move direction");
}