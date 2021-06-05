/*
 * initialize_players_command.c
 *
 * Created: 02-05-2021 17:00:36
 *  Author: victo
 */ 
#include "initialize_players_command.h"

void IPC_Execute(IPC_InitializePlayerCommandParameters params)
{
	GAME_InitGameAfterServerConnection(params.p_game, params.localPlayerServerAssignedId);
}
