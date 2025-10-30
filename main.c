#include <GL/glut.h>
#include <stdio.h>

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

// ANIMATION
int animation_counter = ANIMATION_STEPS;

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

void animate() {
	player.current_pos.x = (player.tile_pos.x + (float)player.dir.x * animation_counter/ANIMATION_STEPS);
	player.current_pos.y = (player.tile_pos.y + (float)player.dir.y * animation_counter/ANIMATION_STEPS);

	npc.mov.current_pos.x = (npc.mov.tile_pos.x + (float)npc.mov.dir.x * animation_counter/ANIMATION_STEPS);
	npc.mov.current_pos.y = (npc.mov.tile_pos.y + (float)npc.mov.dir.y * animation_counter/ANIMATION_STEPS);
	animation_counter++;
	if (animation_counter == ANIMATION_STEPS) { // ANIMATION END
		player.tile_pos.x += player.dir.x;
		player.tile_pos.y += player.dir.y;

		player.current_pos.x = player.tile_pos.x;
		player.current_pos.y = player.tile_pos.y;
		
		npc.mov.tile_pos.x += npc.mov.dir.x;
		npc.mov.tile_pos.y += npc.mov.dir.y;

		npc.mov.current_pos.x = npc.mov.tile_pos.x;
		npc.mov.current_pos.y = npc.mov.tile_pos.y;

		npc.current_path_idx = (npc.current_path_idx + 1) % 8;
	}
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

    if (animation_counter < ANIMATION_STEPS) {
    	animate();
    }
    
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
	if (animation_counter > 0 && animation_counter < ANIMATION_STEPS) {
		return; // IN THE MIDDLE OF ANIMATION
	}
	
    switch (key) {
        case GLUT_KEY_UP:
            player.dir.x = 0;
            player.dir.y = 1;
            break;
        case GLUT_KEY_DOWN:
            player.dir.x = 0;
            player.dir.y = -1;
            break;
        case GLUT_KEY_LEFT:
            player.dir.x = -1;
            player.dir.y = 0;
            break;
        case GLUT_KEY_RIGHT:
            player.dir.x = 1;
            player.dir.y = 0;
            break;
        default:
            return;
    }
    Vector2i dir = {
    	npc.path[npc.current_path_idx].x - npc.mov.tile_pos.x,
    	npc.path[npc.current_path_idx].y - npc.mov.tile_pos.y
    };
    normalize(&dir);
    npc.mov.dir = dir;
	animation_counter = 0; // ANIMATION START
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(FRAME_TIME_MS, timer, 0);
}

int main(int argc, char** argv) {
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
