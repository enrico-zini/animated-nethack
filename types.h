#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Vector2f;

typedef struct {
    int x;
    int y;
} Vector2i;

typedef struct {
    Vector2f current_pos;
    Vector2i tile_pos;
    Vector2i dir;
} Movable;

#endif
