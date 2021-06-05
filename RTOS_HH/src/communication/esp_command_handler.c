#include <stdio.h>
#include "esp_command_handler.h"
#include "esp_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "game/commands/move_server_player_command.h"
#include "esp_command_to_game_command_params.h"


typedef enum CommandType
{
	ESP_COMMAND_INITIALIZE_PLAYERS = 1,
	ESP_COMMAND_SERVER_PLAYERS_STATUS_UPDATE,
	ESP_COMMAND_BALL_STATUS_UPDATE,
} CommandType;


//Return -1 if command syntax is bad.
int CheckCommandSyntax(ESP_Command command)
{
	int lastByteCheck = command.commandData[ESP_COMMAND_LENGTH_IN_BYTES - 1] == ESP_COMMAND_LAST_BYTE_VALUE;
	int secondLastByteCheck = command.commandData[ESP_COMMAND_LENGTH_IN_BYTES - 2] == ESP_COMMAND_SECOND_LAST_BYTE_VALUE;
	if (!(lastByteCheck && secondLastByteCheck))
		return -1;
	else
		return 0;
}

CommandType GetCommandType(ESP_Command command)
{
	return (CommandType)command.commandData[0];
}

void HandleInitializePlayersCommand(ESP_Command command, Game *p_game)
{
	IPC_InitializePlayerCommandParameters initParams = ECTGCP_ToInitializePlayersParams(command, p_game);
	IPC_Execute(initParams);
	printf("Initialize players command executed\r\n");
}

void HandleCommandServerPlayerStatusUpdate(ESP_Command command, Game* p_game)
{
	MSPC_MoveServerPlayerCommandParameters mspcp[4];
	ECTGCP_ToMoveServerPlayerParams(command, p_game, mspcp);
	
	for(int i = 0; i<GAME_NO_OF_PLAYERS; i++)
		MSPCMD_Execute(mspcp[i]);
	
	printf("HandleCommandServerPlayerStatusUpdate executed\r\n");	
}

void HandleEspCommand(ESP_Command command, Game* p_game)
{
	int ret = CheckCommandSyntax(command);
	if (ret < 0)
	{
		printf("Received command with incorrect syntax. Command will not be handled.\r\n");
		printf("Command: [");
		for (int i = 0; i < ESP_COMMAND_LENGTH_IN_BYTES; i++)
			printf("%d, ", command.commandData[i]);
		printf("]\r\n");
		return;
	}
	CommandType type = GetCommandType(command);

	switch (type)
	{
	case ESP_COMMAND_SERVER_PLAYERS_STATUS_UPDATE:
		HandleCommandServerPlayerStatusUpdate(command, p_game);
		break;
	case ESP_COMMAND_BALL_STATUS_UPDATE:
		printf("Handle Ball Command (not implemented)\r\n");
		break;
	case ESP_COMMAND_INITIALIZE_PLAYERS:
		HandleInitializePlayersCommand(command, p_game);
		break;
	default:
		printf("Unknown command ID %d\r\n", (int)type);
		break;
	}
}

void ESPCH_HandleCommandsTask(void *pvParameters)
{
	TickType_t xLastWakeTime= xTaskGetTickCount();
	const TickType_t xFrequency = 10;
	 
	ESPCH_TaskParams* taskParams = ((ESPCH_TaskParams*)pvParameters);
	xQueueHandle espCommandQueueHndl = taskParams->espCommandQueueHandle;
	

	while (1)
	{
		ESP_Command rxedCommand;
		if (xQueueReceive(espCommandQueueHndl, &rxedCommand, (portTickType)10) == pdTRUE)
		{
			//Command has been received from queue
			HandleEspCommand(rxedCommand, taskParams->game);
		}
		
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}
