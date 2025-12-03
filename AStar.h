#ifndef ASTAR_H
#define ASTAR_H

#include "types.h"

typedef struct Cell {
    int g;
    int h;
    Vector2i position;
    struct Cell *parent;
} Cell;

typedef struct {    
    int count;
    Vector2i *nodes;
} Path;

typedef struct {
    int count;
    Cell* list;
} Cells;

Path AStar_getPath(Vector2i **grid, Vector2i *from, Vector2i *to);

#endif