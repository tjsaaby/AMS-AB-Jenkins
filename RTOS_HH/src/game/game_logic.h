/*
 * game_logic.h
 *
 * Created: 08-04-2021 13:03:59
 *  Author: victo
 */ 


#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define GAME_NO_OF_PLAYERS 4
#define GAME_NO_OF_SERVER_PLAYERS 3
#define MAP_WIDTH 230

#include "player.h"

typedef struct Game
{
	Player localPlayer;
	Player serverPlayers[GAME_NO_OF_SERVER_PLAYERS];
} Game;


void GAME_InitGame(Game *game);
void GAME_InitGameAfterServerConnection(Game *game, int localPlayerId);


void GAME_RefreshScreenTask(void* pvParameters);

void GAME_MovePlayersTask(void* pvParameters); //Temporary function for testing

void GAME_MonitorUserInput(void* pvParameters);

#endif /* GAME_LOGIC_H_ */