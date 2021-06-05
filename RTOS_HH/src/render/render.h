/*
 * render.h
 *
 * Created: 18-03-2021 20:42:05
 *  Author: victo
 */

#ifndef RENDER_H_
#define RENDER_H_
#include "player.h"
#include "scoreboard.h"

//NOTE, MUST MATCH THE ORDER  IN draw_drawable function pointer defined in .c file!
typedef enum DrawableType {
    DRAWABLE_SCOREBOARD = 0,
    DRAWABLE_PLAYER,
    DRAWABLE_COUNT
} DrawableType;

typedef struct Drawable{
    const void* drawable;
    DrawableType type;
} Drawable;

Drawable RNDR_PlayerAsDrawable(const Player* c);

Drawable RNDR_ScoreboardAsDrawable(const Scoreboard* d);

void (*RNDR_RenderDrawable[DRAWABLE_COUNT])(Drawable);
void (*RNDR_UnrenderDrawable[DRAWABLE_COUNT])(Drawable);

#endif /* RENDER_H_ */