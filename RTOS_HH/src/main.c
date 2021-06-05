/******************************************************
FreeRTOS demo program.
Implementing 2 tasks, each blinking a LED.

Target = "Arduino Mega2560" + "PR I/O Shield"

Henning Hargaard 25.2.2018
*******************************************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "TFTdriver.h"
#include "color.h"
#include "game_logic.h"
#include "uart_int.h"
#include "game/commands/move_local_player_command.h"
#include "game/commands/move_server_player_command.h"
#include "communication/esp_command_builder.h"
#include "communication/esp_command_handler.h"
#include "game/Touch.h"
#include "game/commands/check_if_touched.h"

void GenerateTestCommands(void *pvParameters)
{
	Game *p_game = (Game *)pvParameters;
	portTickType xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency = 20;
	
	MLPC_MoveLocalPlayerCommandParameters params1 = {.direction = MLPC_LEFT, .p_game = p_game};
	MSPC_MoveServerPlayerCommandParameters params2 = {.coordinates = {.XPosition = 40, .YPosition = 40}, .p_game = p_game, .playerServerId = 1};

	while (1)
	{
		MLPCMD_Execute(params1);
		MSPCMD_Execute(params2);
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}


FILE uart_str = FDEV_SETUP_STREAM(UART_PutChar, NULL, _FDEV_SETUP_RW);

int main(int argc, char **argv)
{
	TFT_DisplayInit();
	UART_InitUART(57600, 8, 'N', 1);
	initTouch();
	stdout = &uart_str;
	printf("Test\r\n");
	printf("Test\r\n");
	EICRB &= ~(1 << ISC40);
	EICRB |= 1 << ISC41;
	EIMSK |= 1 << INT4;
	//Make the whole screen white
	TFT_FillRectangle(0, 0, 320, 240, COLOR_White());
	//Make buttons
	RGB black = {.Red = 0, .Green = 0, .Blue = 0};
	RGB yellow = {.Red = 31, .Green = 63, .Blue = 0};
	TFT_DrawCharacter(250,170,'L', yellow,black, 6, 8);
	TFT_DrawCharacter(250,20,'R', yellow, black, 6, 8);
	
	//Initialize the game and run the tasks associated with it
	Game game;
	GAME_InitGame(&game);
	
	xTaskCreate(GAME_RefreshScreenTask, "Task1", configMINIMAL_STACK_SIZE, (void *)&game, tskIDLE_PRIORITY, NULL);
	//xTaskCreate(GAME_MovePlayersTask, "Task2", configMINIMAL_STACK_SIZE, (void *)&game, tskIDLE_PRIORITY, NULL);
	//Initialize ESP32 Communication and tasks associated  with it
	ESPCB_Initialize();
	ESPCH_TaskParams espchTaskParams = {.espCommandQueueHandle = ESPBC_GetCommandQueueHandle(), .game = &game};
	xTaskCreate(ESPBC_BuildCommandTask, "Task4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(ESPCH_HandleCommandsTask, "Task0", configMINIMAL_STACK_SIZE, (void *)&espchTaskParams, tskIDLE_PRIORITY, NULL);
	xTaskCreate(polIfTouched, "Task5", configMINIMAL_STACK_SIZE, &game, tskIDLE_PRIORITY, NULL);
	//xTaskCreate(GenerateTestCommands, "Task3", configMINIMAL_STACK_SIZE, &game, tskIDLE_PRIORITY, NULL);
	
	vTaskStartScheduler(); // When starting the scheduler, interrupts should be enabled.


	while (1)
	{
	}
}
