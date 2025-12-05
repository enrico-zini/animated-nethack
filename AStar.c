#include "AStar.h"
#include <stdlib.h>
#include <limits.h>

int AStar_applyManhattan(Vector2i *from, Vector2i *to) {
    return abs(to->x - from->x) + abs(to->y - from->y);
}

void addToOpen(Cells *open_list, Cell *start, int rows, int columns, bool is_open[rows][columns], bool is_closed[rows][columns]) {
    open_list->list[open_list->count++] = start;
    is_open[start->position.y][start->position.x] = true;
    is_closed[start->position.y][start->position.x] = false;
}

void removeFromOpen(Cells *open_list, int removed_idx, int rows, int columns,  bool is_open[rows][columns], bool is_closed[rows][columns]) {
    Cell *removed = open_list->list[removed_idx];
    open_list->list[removed_idx] = open_list->list[--open_list->count];
    is_open[removed->position.y][removed->position.x] = false;
    is_closed[removed->position.y][removed->position.x] = true;
}

int getLowestF(Cells *open_list) {
    int min_f = INT_MAX;
    int min_f_index = 0;
    for (int i = 0; i < open_list->count; i++) {
        Cell *c = open_list->list[i];
        int f = c->g + c->h;
        if (f < min_f) {
            min_f = f;
            min_f_index = i;
        }
    }
    return min_f_index;
}

void reconstructPath(Path *path, Cell *target) {
    Cell *it = target;
    while (it->parent != NULL) {        
        printf("%d,%d\n", it->position.x, it->position.y);
        path->nodes[path->count++] = it->position;
        it = it->parent;
    }
}

void AStar_getPath(Path *path, const CollisionGrid *collision_grid, Vector2i *from, Vector2i *to) {
    const int ROWS = collision_grid->rows;
    const int COLUMNS = collision_grid->columns;

    Cell *max_open[ROWS * COLUMNS];
    Cells open_list = {
        .size = ROWS * COLUMNS,
        .count = 0,
        .list = max_open
    };

    bool is_closed[ROWS][COLUMNS];
    bool is_open[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            is_closed[i][j] = false;
            is_open[i][j] = false;
        }
    }

    // x = column
    // y = row
    Cell cell_grid[ROWS][COLUMNS];
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Cell *cell = &cell_grid[r][c];
            cell->g = INT_MAX;
            cell->h = 0;
            cell->parent = NULL;
            cell->position = (Vector2i){c, r};
        }
    }

    Cell *start = &cell_grid[from->y][from->x];
    start->g = 0;
    start->h = AStar_applyManhattan(from, to);
    addToOpen(&open_list, start, ROWS, COLUMNS, is_open, is_closed);

    while (open_list.count > 0) {

        int min_f_index = getLowestF(&open_list);
        Cell *current = open_list.list[min_f_index];        
        int row = current->position.y;
        int col = current->position.x;
        if (row == to->y && col == to->x) {
            reconstructPath(path, current);
            return;
        }

        removeFromOpen(&open_list, min_f_index, ROWS, COLUMNS, is_open, is_closed);

        if (row - 1 >= 0 && collision_grid->grid[row - 1][col] && !is_closed[row - 1][col]) { // NORTH
            Cell *n = &cell_grid[row - 1][col];
            int tentative_g = current->g + 1;
            if (tentative_g < n->g) {
                n->g = tentative_g;
                n->h = AStar_applyManhattan(&n->position, to);
                n->parent = current;
                if (!is_open[row - 1][col]) {
                    addToOpen(&open_list, n, ROWS, COLUMNS, is_open, is_closed);
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
                    addToOpen(&open_list, n, ROWS, COLUMNS, is_open, is_closed);
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
                    addToOpen(&open_list, n, ROWS, COLUMNS, is_open, is_closed);
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
                    addToOpen(&open_list, n, ROWS, COLUMNS, is_open, is_closed);
                }
            }
        }
    }

}
