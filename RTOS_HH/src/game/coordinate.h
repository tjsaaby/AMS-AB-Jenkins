#pragma once
#include <stdint.h>

typedef struct Coordinate {
    int XPosition;
    int YPosition;
} Coordinate;

void COORD_SetCoordinate(Coordinate *coordinate, int x, int y);
