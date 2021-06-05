#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "projdefs.h"
#include "stream_buffer.h"

#include "esp_data_structs.h"

xQueueHandle g_espCommandQueue;
StreamBufferHandle_t g_uartStreamBuffer;

xQueueHandle ESPBC_GetCommandQueueHandle()
{
	return g_espCommandQueue;
}

void ESPBC_BuildCommandTask(void *pvParameters)
{
	uint8_t ucRxData[ESP_COMMAND_LENGTH_IN_BYTES];

	size_t receivedBytes, bytesInBuffer;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(5);

	while (1)
	{
		bytesInBuffer = xStreamBufferBytesAvailable(g_uartStreamBuffer);
		if (bytesInBuffer > 0 && bytesInBuffer < ESP_COMMAND_LENGTH_IN_BYTES)
		{
			//Wait 1 ms if there is data in the buffer, but it is not complete.
			vTaskDelay(pdMS_TO_TICKS(1));
		}
		receivedBytes = xStreamBufferReceive(g_uartStreamBuffer, (void *)ucRxData, ESP_COMMAND_LENGTH_IN_BYTES, 0);
		if (receivedBytes == ESP_COMMAND_LENGTH_IN_BYTES)
		{
			ESP_Command command;
			memcpy(&command.commandData, &ucRxData, ESP_COMMAND_LENGTH_IN_BYTES);
			//Send the command to the queue which for it to be handled elsewhere
			xQueueSendToBackFromISR(g_espCommandQueue, (void *)&command, pdFALSE);
		}
		else if (receivedBytes > 0 && bytesInBuffer < ESP_COMMAND_LENGTH_IN_BYTES)
		{
			printf("Received %zu bytes from UART which does not constitute a complete command, buffer has been cleared.\r\n", receivedBytes);
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void ESPCB_Initialize()
{
	StreamBufferHandle_t uartStreamBuffer = xStreamBufferCreate(ESP_COMMAND_LENGTH_IN_BYTES * 3, ESP_COMMAND_LENGTH_IN_BYTES);
	xQueueHandle espCommandQueueHandle = xQueueCreate(ESP_COMMAND_QUEUE_SIZE, sizeof(ESP_Command));
	vQueueAddToRegistry(espCommandQueueHandle, "EspCommandQueue");

	g_espCommandQueue = espCommandQueueHandle;
	g_uartStreamBuffer = uartStreamBuffer;
}

ISR(USART0_RX_vect)
{
	uint8_t receivedByte = UDR0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	size_t xBytesSent = xStreamBufferSendFromISR(g_uartStreamBuffer,
												 (void *)&receivedByte,
												 sizeof(uint8_t),
												 &xHigherPriorityTaskWoken);
	if (xBytesSent != sizeof(uint8_t))
	{
		printf("Not enough space to write to buffer!\r\n");
	}
}
