/*
 * esp_command_to_game_command_params.c
 *
 * Created: 02-05-2021 17:07:17
 *  Author: victo
 */ 
#include "esp_command_to_game_command_params.h"

void ECTGCP_ToMoveServerPlayerParams(ESP_Command cmd, Game* p_game, MSPC_MoveServerPlayerCommandParameters mspcp[])
{
	uint16_t playerNXPosition[GAME_NO_OF_PLAYERS];
	uint16_t playerNYPosition[GAME_NO_OF_PLAYERS];
	
	playerNXPosition[0] =  cmd.commandData[1] << 8 | cmd.commandData[2];
	playerNYPosition[0] =  cmd.commandData[3] << 8 | cmd.commandData[4];
	playerNXPosition[1] =  cmd.commandData[5] << 8 | cmd.commandData[6];
	playerNYPosition[1] =  cmd.commandData[7] << 8 | cmd.commandData[8];
	playerNXPosition[2] =  cmd.commandData[9] << 8 | cmd.commandData[10];
	playerNYPosition[2] =  cmd.commandData[11] << 8 | cmd.commandData[12];
	playerNXPosition[3] =  cmd.commandData[13] << 8 | cmd.commandData[14];
	playerNYPosition[3] =  cmd.commandData[15] << 8 | cmd.commandData[16];
	for(int i= 0; i< GAME_NO_OF_PLAYERS;i++)
	{
		mspcp[i].playerServerId = i;
		mspcp[i].coordinates.XPosition = playerNXPosition[i];
		mspcp[i].coordinates.YPosition = playerNYPosition[i];
		mspcp[i].p_game = p_game;
	}
}

IPC_InitializePlayerCommandParameters ECTGCP_ToInitializePlayersParams(ESP_Command cmd, Game* p_game)
{
	IPC_InitializePlayerCommandParameters params;
	
	params.p_game = p_game;
	params.localPlayerServerAssignedId = cmd.commandData[1];
	
	return params;
}
