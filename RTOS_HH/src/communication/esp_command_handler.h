#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "esp_defines.h"
#include "esp_data_structs.h"
#include "game_logic.h"

typedef struct ESPCH_TaskParams{
	Game *game;
	xQueueHandle espCommandQueueHandle;
	}ESPCH_TaskParams;

void ESPCH_HandleCommandsTask(void *pvParameters);