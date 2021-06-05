#include "check_if_touched.h"
#include "move_local_player_command.h"
#include "move_server_player_command.h"
#include "FreeRTOS.h"
#include "game/Touch.h"
#include "player.h"
#define MOVE_PLAYER_AMOUNT 10
int touch_flag;

void polIfTouched(void *pvParameters)
{
    Game *p_game = (Game *)pvParameters;
    //xQueueHandle xTouchQueue = NULL;

	while(1)
	{
		if(touch_flag == 1)
		{
			touch_flag = 0;
            uint16_t *coordTouch; 
            MLPC_MoveLocalPlayerCommandParameters localParamsLeft = {.direction = MLPC_LEFT, .p_game = p_game};
            MLPC_MoveLocalPlayerCommandParameters localParamsRight = {.direction = MLPC_RIGHT, .p_game = p_game}; 
			MSPC_MoveServerPlayerCommandParameters serverParamLeft = {.coordinates = {.XPosition = p_game->localPlayer.Position.XPosition-MOVE_PLAYER_AMOUNT, .YPosition = p_game->localPlayer.Position.YPosition}, .p_game = p_game, .playerServerId = 1};
			MSPC_MoveServerPlayerCommandParameters serverParamRight = {.coordinates = {.XPosition = p_game->localPlayer.Position.XPosition+MOVE_PLAYER_AMOUNT, .YPosition = p_game->localPlayer.Position.YPosition}, .p_game = p_game, .playerServerId = 1};
			coordTouch = touchTaskOnInterrupt();  // x = [0], y == [1]
        
            if(coordTouch[0] < 230 && coordTouch[1] < 80)                                 
            {    if(coordTouch[0] < 115)
                {
					printf("left");
                    MLPCMD_Execute(localParamsLeft);
					MSPCMD_Execute(serverParamLeft);
                     
                }
                else if(coordTouch[0] > 115)
                {
					printf("right");
                    MLPCMD_Execute(localParamsRight);
					MSPCMD_Execute(serverParamRight);
                }
            }
		}
	}
}