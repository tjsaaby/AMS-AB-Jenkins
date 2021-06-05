/*
 * render.c
 *
 * Created: 18-03-2021 20:42:16
 *  Author: victo
 */

#include "render.h"

#include <stdio.h>

#include "TFTdriver.h"
#include "player.h"
#include "scoreboard.h"

RGB BACKGROUND_COLOR = {.Blue = 255, .Green = 255, .Red = 255};

// Private Render function defs
void RenderScoreboard(Drawable a);
void RenderPlayer(Drawable a);
void (*RNDR_RenderDrawable[DRAWABLE_COUNT])(Drawable) = {&RenderScoreboard, &RenderPlayer};

// Private Unrender function defs
void UnrenderScoreboard(Drawable a);
void UnrenderPlayer(Drawable a);
void (*RNDR_UnrenderDrawable[DRAWABLE_COUNT])(Drawable) = {&UnrenderScoreboard, &UnrenderPlayer};

Drawable RNDR_PlayerAsDrawable(const Player* c) {
    return (Drawable){(void*)c, DRAWABLE_PLAYER};
}

Drawable RNDR_ScoreboardAsDrawable(const Scoreboard* d) {
    return (Drawable){(void*)d, DRAWABLE_SCOREBOARD};
}
void RenderScoreboard(Drawable a) {
    Scoreboard* d = (Scoreboard*)a.drawable;
    // Draw red parts of danish flag
    //FillRectangle(140,140,320-140,100,31,0,0);
}
void UnrenderScoreboard(Drawable a) {
    Scoreboard* d = (Scoreboard*)a.drawable;
    // Draw red parts of danish flag
    //FillRectangle(140,140,320-140,100,31,0,0);
}

void RenderPlayer(Drawable a) {
    Player* p = (Player*)a.drawable;
	
    RGB color = {.Blue = p->color.Blue, .Green = p->color.Green, .Red = p->color.Red};
	if(p->orientation == PLAYER_HORIZONTAL)
	{
		TFT_FillRectangle(p->Position.XPosition, p->Position.YPosition, p->width, p->height, color);
		p->latestRenderedOrientation = p->orientation;
		p->latestRenderedPosition = p->Position;
		p->hasBeenRenderedYet = 1;
	}
	else if(p->orientation == PLAYER_VERTICAL)
	{
		TFT_FillRectangle(p->Position.XPosition, p->Position.YPosition, p->height, p->width, color);
		p->latestRenderedOrientation = p->orientation;
		p->latestRenderedPosition = p->Position;
		p->hasBeenRenderedYet = 1;	
	}
	else
		printf("Player orientation is invalid: %d", p->orientation);
}
void UnrenderPlayer(Drawable a) {
    Player* p = (Player*)a.drawable;
	if(p->hasBeenRenderedYet)
	{
		if(p->latestRenderedOrientation == PLAYER_HORIZONTAL)
			TFT_FillRectangle(p->latestRenderedPosition.XPosition, p->latestRenderedPosition.YPosition, p->width, p->height, BACKGROUND_COLOR);
		else if(p->latestRenderedOrientation == PLAYER_VERTICAL)
			TFT_FillRectangle(p->latestRenderedPosition.XPosition, p->latestRenderedPosition.YPosition, p->height, p->width, BACKGROUND_COLOR);
		else
			printf("Player orientation is invalid: %d\r\n", p->orientation);
	}
	else
	{
		printf("Player %d has not been rendered before and therefore it will not be unrendered\r\n", p->ServerAssignedPlayerId);
	}
		
		
	
}
