/*
 * player.h
 *
 * Created: 11-03-2021 11:43:07
 *  Author: victo
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include <stdint.h>

#include "TFTdriver.h"
//TODO: Instead of depending on TFTdriver here, the player could use HEX colors, and in the TFT driver a HEX to RGB converter could be implemented.
#include "coordinate.h"

typedef enum PlayerOrientation
{
	PLAYER_HORIZONTAL = 0,
	PLAYER_VERTICAL,
} PlayerOrientation;

typedef struct Player
{
    RGB color;
    Coordinate Position;
	PlayerOrientation orientation;
	int ServerAssignedPlayerId;
	PlayerOrientation latestRenderedOrientation;
	Coordinate latestRenderedPosition;
	int hasBeenRenderedYet;
	int width;
	int height;
} Player;

/**
 * @brief Initialize the player struct. Places the player in the middle of the playing field.
 *
 * @param p_player A pointer to the player struct which must be initialized
 * @param color The color to initialize the player as.
 */
void PLYR_InitPlayer(Player *p_player, RGB color, int width, int height);

/**
 * @brief Moves the player to the right
 *
 * @param p_player
 */
void PLYR_MoveRight(Player *p_player);

/**
 * @brief Moves the player to the left
 *
 * @param p_player
 */
void PLYR_MoveLeft(Player *p_player);

/**
 * @brief Method for setting the Player Position for the given player. Updates the previous position along with current position.
 *
 * @param p_player Pointer to the player whose position should be set.
 * @param newPosition The new position of the player. A boundary check is performed, and will set the position to the lower/upper boundary if the given parameter is outside the boundaries.
 */
void PLYR_SetPlayerPosition(Player *p_player, Coordinate newPosition);

#endif /* PLAYER_H_ */