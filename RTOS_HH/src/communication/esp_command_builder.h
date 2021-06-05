#pragma once

void ESPCB_Initialize();
xQueueHandle ESPBC_GetCommandQueueHandle();
void ESPBC_BuildCommandTask(void* pvParameters);
ISR(USART0_RX_vect);