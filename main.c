#include <GL/glut.h>
#include <stdio.h>
#include "AnimationController.h"
#include "types.h"
#include "AStar.h"

#define TILE_SIZE 32
#define GRID_WIDTH 512
#define GRID_HEIGHT 512
#define ANIMATION_STEPS 10
#define FRAME_TIME_MS 16

// COLORS
#define WHITE 1.0f, 1.0f, 1.0f
#define BLACK 0.0f, 0.0f, 0.0f

int windowWidth = GRID_WIDTH;
int windowHeight = GRID_HEIGHT;

CollisionGrid collision_grid;

typedef struct { 
	Movable mov; 
	Vector2i path[8]; 
	size_t current_path_idx; 
} Npc;

Npc npc = {
    .mov = {{10.0f, 10.0f}, {10, 10}, {0, 0}},
    .path = {
    	{10, 10}, {11, 10}, {12, 10}, {12, 9}, {12, 8}, {11, 8}, {10, 8}, {10, 9}
    },
    .current_path_idx = 0
};

Movable player = {{0.0f, 0.0f}, {0, 0}, {0, 0}};

Movable *movables[] = {&npc.mov, &player};

// ANIMATION
AnimationController ac;

// MEASURES
int last_frame_ms = 0;

void drawTile(Vector2f *v) {
    glColor3f(WHITE);
    float x = v->x * TILE_SIZE;
    float y = v->y * TILE_SIZE;
	glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + TILE_SIZE, y);
		glVertex2i(x + TILE_SIZE, y + TILE_SIZE);
		glVertex2i(x, y + TILE_SIZE);
	glEnd();
}

void drawGrid() {
    glColor3f(WHITE);
    glBegin(GL_LINES);
    for (int x = 0; x <= GRID_WIDTH; x += TILE_SIZE) {
        glVertex2i(x, 0);
        glVertex2i(x, GRID_HEIGHT);
    }
    for (int y = 0; y <= GRID_HEIGHT; y += TILE_SIZE) {
        glVertex2i(0, y);
        glVertex2i(GRID_WIDTH, y);
    }
    glEnd();
    for (int i = 0; i < collision_grid.rows; i++) {
        for (int j = 0; j < collision_grid.columns; j++) {
            if (collision_grid.grid[i][j]) {
                Vector2f v = {.x = j, .y = collision_grid.columns - i - 1};
                drawTile(&v);
            }
        }
    }
}

void showFPS() {
    int current_frame_ms = glutGet(GLUT_ELAPSED_TIME);
    float delta = (current_frame_ms - last_frame_ms) / 1000.0f;
    last_frame_ms = current_frame_ms;
    if (delta > 0.0f) {
    	char title[64];
    	sprintf(title, "Animated Nethack - FPS: %.1f", 1.0f / delta);
    	glutSetWindowTitle(title);
    }
}

void drawScreen() {
    drawGrid();
    size_t movable_count = sizeof(movables)/sizeof(Movable *);
    for (size_t i = 0; i < movable_count; i++) {
        drawTile(&movables[i]->current_pos);
    }
}

void display() {
	showFPS();

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef((windowWidth - GRID_WIDTH) / 2.0f, (windowHeight - GRID_HEIGHT) / 2.0f, 0);

    AC_animate(&ac, movables, sizeof(movables)/sizeof(Movable *));
    drawScreen(); 

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void normalize(Vector2i *v) {
    if (v->x > 0) v->x = 1;
    else if (v->x < 0) v->x = -1;
    else v->x = 0;

    if (v->y > 0) v->y = 1;
    else if (v->y < 0) v->y = -1;
    else v->y = 0;
}

void take_input(int key, int x, int y) {
	if (AC_isAnimating(&ac)) {
		return;
	}

	// PLAYER MOVEMENT LOGIC
    Vector2i player_dir = {0, 0};
    switch (key) {
        case GLUT_KEY_UP:    player_dir = (Vector2i){ 0,  1}; break;
        case GLUT_KEY_DOWN:  player_dir = (Vector2i){ 0, -1}; break;
        case GLUT_KEY_LEFT:  player_dir = (Vector2i){-1,  0}; break;
        case GLUT_KEY_RIGHT: player_dir = (Vector2i){ 1,  0}; break;
        default: return;
    }
    player.dir = player_dir;

    // NPCS MOVEMENT LOGIC
    npc.current_path_idx = (npc.current_path_idx + 1) % 8;
    Vector2i npc_dir = {
    	npc.path[npc.current_path_idx].x - npc.mov.tile_pos.x,
    	npc.path[npc.current_path_idx].y - npc.mov.tile_pos.y
    };
    normalize(&npc_dir);
    npc.mov.dir = npc_dir;

    AC_startAnimation(&ac);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(FRAME_TIME_MS, timer, 0);
}

CollisionGrid init_grid_from_file(char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        printf("File not found [filename = %s]\n", file_name);
        exit(EXIT_FAILURE);
    }

    int rows, columns;
    fscanf(f, "%d,%d", &rows, &columns);
    bool **grid = malloc(rows * sizeof(bool *));
    for (int i = 0; i < rows; i++) {
        grid[i] = malloc(columns * sizeof(bool));
    }

    int c;
    while ((c = fgetc(f)) != '\n' && c != EOF);

    int row = 0, column = 0;
    while ((c = fgetc(f)) != EOF && row < rows) {
        if (c == '0' || c == '1') {
            grid[row][column] = (c == '1');
            if (grid[row][column]) printf("Collision(x,y): %d,%d\n", column, row);
            column++;
            if (column == columns) {
                column = 0;
                row++;
            }
        }
    }

    fclose(f);

    return (CollisionGrid) {
        .rows = rows,
        .columns = columns,
        .grid = grid
    };
}

int main(int argc, char** argv) {
	ac = AC_init(ANIMATION_STEPS);
    collision_grid = init_grid_from_file("grid.txt");

    Vector2i path_buf[collision_grid.rows * collision_grid.columns];
    Path path = {
        .count = 0,
        .nodes = path_buf
    };
    AStar_getPath(&path, &collision_grid, &(Vector2i){10,15}, &(Vector2i){4,15});
    
    printf("FROM(%d,%d) TO(%d,%d)\n", 10, 15, 4, 15);
    printf("PATH: ");
    for (int i = 0; i < path.count; i++) {
    	printf("[%d,%d] ", path.nodes[i].x, path.nodes[i].y);
    }
    printf("\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Animated Nethack");

    glClearColor(BLACK, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(take_input);
    glutTimerFunc(FRAME_TIME_MS, timer, 0);
    
    glutMainLoop();

    for (int i = 0; i < collision_grid.rows; i++) {
        free(collision_grid.grid[i]);
    }
    
    return 0;
}
