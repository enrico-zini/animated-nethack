#include <GL/glut.h>
#include <stdio.h>
#include "AnimationController.h"
#include "types.h"

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
}

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

void display() {
	showFPS();

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef((windowWidth - GRID_WIDTH) / 2.0f, (windowHeight - GRID_HEIGHT) / 2.0f, 0);

    drawGrid();

	AC_animate(&ac, movables, sizeof(movables)/sizeof(Movable *));    
    
    drawTile(&player.current_pos);
    drawTile(&npc.mov.current_pos);

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
	
    Vector2i player_dir = {0, 0};
    switch (key) {
        case GLUT_KEY_UP:    player_dir = (Vector2i){ 0,  1}; break;
        case GLUT_KEY_DOWN:  player_dir = (Vector2i){ 0, -1}; break;
        case GLUT_KEY_LEFT:  player_dir = (Vector2i){-1,  0}; break;
        case GLUT_KEY_RIGHT: player_dir = (Vector2i){ 1,  0}; break;
        default: return;
    }

	// PLAYER MOVEMENT LOGIC
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

int main(int argc, char** argv) {
	ac = AC_init(ANIMATION_STEPS);

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
    return 0;
}
