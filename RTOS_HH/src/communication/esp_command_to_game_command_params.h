/*
 * esp_command_to_game_command_params.h
 *
 * Created: 02-05-2021 17:07:03
 *  Author: victo
 */ 


#ifndef ESP_COMMAND_TO_GAME_COMMAND_PARAMS_H_
#define ESP_COMMAND_TO_GAME_COMMAND_PARAMS_H_

#include "esp_data_structs.h"
#include "game/commands/move_server_player_command.h"
#include "game/commands/initialize_players_command.h"
/**
 * @brief Convert ESP_Command to MSPC_MoveServerPlayerCommandParameters.
 */
void ECTGCP_ToMoveServerPlayerParams(ESP_Command cmd, Game* p_game, MSPC_MoveServerPlayerCommandParameters mspcp[]);

/**
 * @brief Convert ESP_Command to IPC_InitializePlayerCommandParameters.
 */
IPC_InitializePlayerCommandParameters ECTGCP_ToInitializePlayersParams(ESP_Command cmd, Game* p_game);




#endif /* ESP_COMMAND_TO_GAME_COMMAND_PARAMS_H_ */