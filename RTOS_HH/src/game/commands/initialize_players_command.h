/*
 * initialize_players_command.h
 *
 * Created: 02-05-2021 17:00:29
 *  Author: victo
 */ 


#ifndef INITIALIZE_PLAYERS_COMMAND_H_
#define INITIALIZE_PLAYERS_COMMAND_H_

#include "game_logic.h"


typedef struct IPC_InitializePlayerCommandParameters {
	Game* p_game;
	int localPlayerServerAssignedId;
} IPC_InitializePlayerCommandParameters;


void IPC_Execute(IPC_InitializePlayerCommandParameters params);

#endif /* INITIALIZE_PLAYERS_COMMAND_H_ */