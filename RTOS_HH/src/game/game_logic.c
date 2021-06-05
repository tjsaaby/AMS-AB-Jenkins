/*
 * game_logic.c
 *
 * Created: 08-04-2021 13:02:59
 *  Author: victo
 */

#include "game_logic.h"

#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "TFTdriver.h"
#include "render.h"

#define NO_OF_SCOREBOARDS 0
#define NO_OF_DRAWABLES GAME_NO_OF_PLAYERS + NO_OF_SCOREBOARDS
#define INITIAL_PLAYER_WIDTH 10
#define INITIAL_PLAYER_HEIGHT 30

void InitPlayersAfterServerConnection(Game *p_game, int localPlayerId)
{
	if (localPlayerId < 0 || localPlayerId > GAME_NO_OF_PLAYERS - 1)
	{
		printf("Invalid local player ID %d", localPlayerId);
		return;
	}
	int playerIds[GAME_NO_OF_PLAYERS] = {0, 1, 2, 3};
	int serverPlayerIds[GAME_NO_OF_SERVER_PLAYERS];

	//Create the list of server player Ids by copying the player id's and removing the local player id'
	int serverPlayerIdsInArray = 0;
	for (int i = 0; i < GAME_NO_OF_PLAYERS; i++)
	{
		if (playerIds[i] != localPlayerId)
			serverPlayerIds[serverPlayerIdsInArray++] = playerIds[i];
	}

	//Assign the id's to the players, they will be necessary later in the function because colors, positions and orientations are determined by player ids.
	p_game->localPlayer.ServerAssignedPlayerId = localPlayerId;
	for (int i = 0; i < GAME_NO_OF_SERVER_PLAYERS; i++)
	{
		p_game->serverPlayers[i].ServerAssignedPlayerId = serverPlayerIds[i];
	}

	//Player array which references the players in the game so they can be looped over
	Player *players[GAME_NO_OF_PLAYERS] =
		{
			[0] = &p_game->localPlayer,
			[1] = &p_game->serverPlayers[0],
			[2] = &p_game->serverPlayers[1],
			[3] = &p_game->serverPlayers[2],
		};

	RGB player_colors[GAME_NO_OF_PLAYERS] =
		{
			[0] = COLOR_Blue(),
			[1] = COLOR_Red(),
			[2] = COLOR_Cyan(),
			[3] = COLOR_Green(),
		};
	//Initial coordinates depending on local player id
	int middle = MAP_WIDTH / 2 - INITIAL_PLAYER_HEIGHT / 2;
	Coordinate initial_coords[GAME_NO_OF_PLAYERS][GAME_NO_OF_PLAYERS] =
	{
		[0] = {
			[0] = {.XPosition = MAP_WIDTH, middle},
			[1] = {.XPosition = middle, .YPosition = MAP_WIDTH},
			[2] = {.XPosition = 0, .YPosition = middle},
			[3] = {.XPosition = middle, .YPosition = 0},
		},
		[1] = {
			[0] = {.XPosition = middle, .YPosition = 0},
			[1] = {.XPosition = MAP_WIDTH, middle},
			[2] = {.XPosition = middle, .YPosition = MAP_WIDTH},
			[3] = {.XPosition = 0, .YPosition = middle},
		},
		[2] = {
			[0] = {.XPosition = 0, .YPosition = middle},
			[1] = {.XPosition = middle, .YPosition = 0},
			[2] = {.XPosition = MAP_WIDTH, middle},
			[3] = {.XPosition = middle, .YPosition = MAP_WIDTH},
		},
		[3] = {
			[0] = {.XPosition = middle, .YPosition = MAP_WIDTH},
			[1] = {.XPosition = 0, .YPosition = middle},
			[2] = {.XPosition = middle, .YPosition = 0},
			[3] = {.XPosition = MAP_WIDTH, middle},
		},
	};
	PlayerOrientation orientations[GAME_NO_OF_PLAYERS][GAME_NO_OF_PLAYERS] =
		{
			[0] = {
				[0] = PLAYER_HORIZONTAL,
				[1] = PLAYER_VERTICAL,
				[2] = PLAYER_HORIZONTAL,
				[3] = PLAYER_VERTICAL,
			},
			[1] = {
				[0] = PLAYER_VERTICAL,
				[1] = PLAYER_HORIZONTAL,
				[2] = PLAYER_VERTICAL,
				[3] = PLAYER_HORIZONTAL,
			},
			[2] = {
				[0] = PLAYER_HORIZONTAL,
				[1] = PLAYER_VERTICAL,
				[2] = PLAYER_HORIZONTAL,
				[3] = PLAYER_VERTICAL,
			},
			[3] = {
				[0] = PLAYER_VERTICAL,
				[1] = PLAYER_HORIZONTAL,
				[2] = PLAYER_VERTICAL,
				[3] = PLAYER_HORIZONTAL,
			}};

	for (int i = 0; i < GAME_NO_OF_PLAYERS; i++)
	{
		int id = players[i]->ServerAssignedPlayerId;
		players[i]->orientation = orientations[localPlayerId][id];
		players[i]->color = player_colors[id];
		PLYR_SetPlayerPosition(players[i], initial_coords[localPlayerId][id]);
	}
}

void InitPlayers(Game *game)
{
	//Init local player
	RGB local_player_color = COLOR_Blue();
	PLYR_InitPlayer(&game->localPlayer, local_player_color, INITIAL_PLAYER_WIDTH, INITIAL_PLAYER_HEIGHT);
	game->localPlayer.orientation = PLAYER_HORIZONTAL;
	//Init server players
	RGB server_player_colors[GAME_NO_OF_SERVER_PLAYERS] =
		{
			[0] = COLOR_Green(),
			[1] = COLOR_Red(),
			[2] = COLOR_Cyan(),
		};

	Coordinate server_player_initial_coords[GAME_NO_OF_SERVER_PLAYERS] =
		{
			[0] = {.XPosition = 50, .YPosition = 50},
			[1] = {.XPosition = 210, .YPosition = 105},
			[2] = {.XPosition = 0, .YPosition = 105}};

	PlayerOrientation orientations[GAME_NO_OF_PLAYERS] =
		{
			[0] = PLAYER_VERTICAL,
			[1] = PLAYER_HORIZONTAL,
			[2] = PLAYER_VERTICAL,
		};
	for (int i = 0; i < GAME_NO_OF_SERVER_PLAYERS; i++)
	{
		PLYR_InitPlayer(&game->serverPlayers[i], server_player_colors[i], INITIAL_PLAYER_WIDTH, INITIAL_PLAYER_HEIGHT);
		game->serverPlayers[i].orientation = orientations[i];
		PLYR_SetPlayerPosition(&game->serverPlayers[i], server_player_initial_coords[i]);
	}
}

void GAME_RefreshScreenTask(void *pvParameters)
{
	Game *game = (Game *)pvParameters;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 10;

	Drawable drawables[NO_OF_DRAWABLES];

	drawables[0] = RNDR_PlayerAsDrawable(&game->localPlayer);
	for (int i = 1; i < GAME_NO_OF_SERVER_PLAYERS + 1; i++)
		drawables[i] = RNDR_PlayerAsDrawable(&game->serverPlayers[i - 1]);

	while (1)
	{
		for (int i = 0; i < NO_OF_DRAWABLES; i++)
		{
			Drawable a = drawables[i];
			RNDR_UnrenderDrawable[a.type](a);
		}
		for (int i = 0; i < NO_OF_DRAWABLES; i++)
		{
			Drawable a = drawables[i];
			RNDR_RenderDrawable[a.type](a);
		}
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

//Deprecated task - only used for showing that players can move
void GAME_MovePlayersTask(void *pvParameters)
{
	Game *game = (Game *)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 10;

	Player *players[GAME_NO_OF_PLAYERS];
	players[0] = &game->localPlayer;
	players[1] = &game->serverPlayers[0];
	players[2] = &game->serverPlayers[1];
	players[3] = &game->serverPlayers[2];

	while (1)
	{
		int i;
		for (i = 0; i < GAME_NO_OF_PLAYERS; i++)
		{
			if (i % 2 == 1)
				PLYR_MoveLeft(players[i]);
			else if (i % 2 == 0)
				PLYR_MoveRight(players[i]);
		}
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void GAME_InitGame(Game *game)
{
	InitPlayers(game);
}

void GAME_InitGameAfterServerConnection(Game *game, int localPlayerId)
{
	InitPlayersAfterServerConnection(game, localPlayerId);
}
