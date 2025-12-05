#include "AStar.h"
#include <stdlib.h>

int AStar_applyManhattan(Vector2i *from, Vector2i *to) {
    return abs(to->x - from->x) + abs(to->y - from->y);
}

Path AStar_getPath(CollisionGrid *collision_grid, Vector2i *from, Vector2i *to) {
    const int ROWS = collision_grid->rows;
    const int COLUMNS = collision_grid->columns;

    int valid_cells = 0;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (collision_grid->grid[r][c]) valid_cells++;
        }
    }

    Cell *open_mem[valid_cells];
    Cells open_list = {
        .size = valid_cells,
        .count = 0,
        .list = open_mem
    };

    bool is_closed[ROWS][COLUMNS];
    memset(is_closed, false, sizeof(is_closed));

    bool is_open[ROWS][COLUMNS];
    memset(is_open, false, sizeof(is_open));

    // x = column
    // y = row
    Cell cell_grid[ROWS][COLUMNS];
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Cell *cell = &cell_grid[r][c];
            cell->g = __INT_MAX__;
            cell->h = 0;
            cell->parent = NULL;
            cell->position = (Vector2i){c, r};
        }
    }

    Cell *start = &cell_grid[from->y][from->x];
    start->g = 0;
    start->h = AStar_applyManhattan(from, to);
    start->parent = NULL;

    open_list.list[open_list.count++] = start;
    is_open[from->y][from->x] = true;

    while (open_list.count > 0) {

        int min_f = __INT_MAX__;
        int min_f_index = 0;

        for (int i = 0; i < open_list.count; i++) {
            Cell *c = open_list.list[i];
            int f = c->g + c->h;
            if (f < min_f) {
                min_f = f;
                min_f_index = i;
            }
        }

        Cell *current = open_list.list[min_f_index];

        open_list.list[min_f_index] = open_list.list[open_list.count - 1];
        open_list.count--;

        int row = current->position.y;
        int col = current->position.x;

        is_open[row][col] = false;
        is_closed[row][col] = true;

        if (row == to->y && col == to->x) {
            return (Path){0}; // TODO: reconstruct
        }

        if (row - 1 >= 0 && collision_grid->grid[row - 1][col] && !is_closed[row - 1][col]) { // NORTH
            Cell *n = &cell_grid[row - 1][col];
            int tentative_g = current->g + 1;
            if (tentative_g < n->g) {
                n->g = tentative_g;
                n->h = AStar_applyManhattan(&n->position, to);
                n->parent = current;
                if (!is_open[row - 1][col]) {
                    open_list.list[open_list.count++] = n;
                    is_open[row - 1][col] = true;
                }
            }
        }

        if (row + 1 < ROWS && collision_grid->grid[row + 1][col] && !is_closed[row + 1][col]) { // SOUTH
            Cell *n = &cell_grid[row + 1][col];
            int tentative_g = current->g + 1;
            if (tentative_g < n->g) {
                n->g = tentative_g;
                n->h = AStar_applyManhattan(&n->position, to);
                n->parent = current;
                if (!is_open[row + 1][col]) {
                    open_list.list[open_list.count++] = n;
                    is_open[row + 1][col] = true;
                }
            }
        }

        if (col - 1 >= 0 && collision_grid->grid[row][col - 1] && !is_closed[row][col - 1]) { // WEST
            Cell *n = &cell_grid[row][col - 1];
            int tentative_g = current->g + 1;
            if (tentative_g < n->g) {
                n->g = tentative_g;
                n->h = AStar_applyManhattan(&n->position, to);
                n->parent = current;
                if (!is_open[row][col - 1]) {
                    open_list.list[open_list.count++] = n;
                    is_open[row][col - 1] = true;
                }
            }
        }

        if (col + 1 < COLUMNS && collision_grid->grid[row][col + 1] && !is_closed[row][col + 1]) { // EAST
            Cell *n = &cell_grid[row][col + 1];
            int tentative_g = current->g + 1;
            if (tentative_g < n->g) {
                n->g = tentative_g;
                n->h = AStar_applyManhattan(&n->position, to);
                n->parent = current;
                if (!is_open[row][col + 1]) {
                    open_list.list[open_list.count++] = n;
                    is_open[row][col + 1] = true;
                }
            }
        }
    }

    return (Path){0};
}
