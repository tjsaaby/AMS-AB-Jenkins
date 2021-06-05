/*
 * PlayerBar.c
 *
 * Created: 11-03-2021 11:42:57
 *  Author: victo
 */

#include "player.h"
#include "TFTdriver.h"

#define LOWER_LEFT_RIGHT_POSITION_BOUND 0
#define UPPER_LEFT_RIGHT_POSITION_BOUND 230
#define MOVE_PLAYER_AMOUNT 20


void PLYR_InitPlayer(Player *p_player, RGB color, int width, int height) {
    p_player->color = color;
    //Place the player in the middle
    Coordinate initialPosition = {.XPosition = UPPER_LEFT_RIGHT_POSITION_BOUND / 2, .YPosition = 0};
	p_player->hasBeenRenderedYet = 0;
    p_player->Position = initialPosition;
    p_player->width = width;
    p_player->height = height;
}
/************************************************************************/
/*    The move right and move left functions only affect the X axis,
      and are only intented to be used for the local player. For
      players handled by the server the position can be set using the
      SetPlayerPosition function which can change the y coordinates */
/************************************************************************/
void PLYR_MoveRight(Player *p_player) {
    Coordinate position = p_player->Position;
    position.XPosition = position.XPosition + MOVE_PLAYER_AMOUNT;
    PLYR_SetPlayerPosition(p_player, position);
}

void PLYR_MoveLeft(Player *p_player) {
    Coordinate position = p_player->Position;
    position.XPosition = position.XPosition - MOVE_PLAYER_AMOUNT;
    PLYR_SetPlayerPosition(p_player, position);
}

void PLYR_SetPlayerPosition(Player *p_player, Coordinate newPosition) {
    if(newPosition.XPosition < LOWER_LEFT_RIGHT_POSITION_BOUND)
        newPosition.XPosition = LOWER_LEFT_RIGHT_POSITION_BOUND;
    else if (newPosition.XPosition > UPPER_LEFT_RIGHT_POSITION_BOUND)
        newPosition.XPosition = UPPER_LEFT_RIGHT_POSITION_BOUND;
    if (newPosition.YPosition < LOWER_LEFT_RIGHT_POSITION_BOUND)
        newPosition.YPosition = LOWER_LEFT_RIGHT_POSITION_BOUND;
    else if (newPosition.YPosition > UPPER_LEFT_RIGHT_POSITION_BOUND)
        newPosition.YPosition = UPPER_LEFT_RIGHT_POSITION_BOUND;
    p_player->Position = newPosition;
}
