/*
 * move_server_player_command.c
 *
 * Created: 15-04-2021 19:32:00
 *  Author: victo
 */
#include <stddef.h>
#include "move_server_player_command.h"

void MSPCMD_Execute(MSPC_MoveServerPlayerCommandParameters params) {
	
	Player* player = NULL;
	for(int i = 0; i<GAME_NO_OF_SERVER_PLAYERS; i++)
	{
		if(params.p_game->serverPlayers[i].ServerAssignedPlayerId == params.playerServerId)
			player = &params.p_game->serverPlayers[i];
	}
    
	if(player!=NULL)
	    PLYR_SetPlayerPosition(player, params.coordinates);
}
