/*
 * esp_data_structs.h
 *
 * Created: 01-05-2021 19:27:50
 *  Author: victo
 */

#ifndef ESP_DATA_STRUCTS_H_
#define ESP_DATA_STRUCTS_H_

#include "esp_defines.h"

#include "stdint-gcc.h"
typedef struct ESP_Command
{
	uint8_t commandData[ESP_COMMAND_LENGTH_IN_BYTES];
} ESP_Command;

#endif /* ESP_DATA_STRUCTS_H_ */