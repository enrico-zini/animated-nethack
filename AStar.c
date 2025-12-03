#include "AStar.h"

Path AStar_getPath(bool **grid, Vector2i *from, Vector2i *to) {

    int valid_cells = 123; // loop and count number of valid cells

    Cell open_list[valid_cells];
    Cells open_cells = {
        .count = 0,
        .list = open_list
    };

    Cell closed_list[valid_cells];
    Cells closed_cells = {
        .count = 0,
        .list = closed_list
    };

    Cell start = {
        .g = 0,
        .h = 0, // apply manhattan
        .position = *from,
        .parent = NULL
    };

    open_cells.list[open_cells.count] = start;
    open_cells.count++;
    

    while (open_cells.count > 0) {
        // order open list and get lowest g + h
        // remove lowest from open_list
        // add it to the closed_list
        // for each neighbor calculate the g and f, store the parent and add to the open_list
    }

}