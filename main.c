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

typedef struct Coord2D {
	float x;
	float y;
} Coord2D;

typedef struct Player {
	Coord2D pos;
	Coord2D start;
	Coord2D dir;
} Player;

Player player = {{0, 0}, {0, 0}, {0, 0}};

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

void animatePlayer() {
	player.pos.x = player.start.x + (player.dir.x * animation_counter/ANIMATION_STEPS);
	player.pos.y = player.start.y + (player.dir.y * animation_counter/ANIMATION_STEPS);
	animation_counter++;
	if (animation_counter == ANIMATION_STEPS) { // ANIMATION END
		player.start.x += player.dir.x;
		player.start.y += player.dir.y;

		player.pos.x = player.start.x;
		player.pos.y = player.start.y;
	}
}

void drawPlayer() {
    glColor3f(WHITE);
    float x = player.pos.x * TILE_SIZE;
    float y = player.pos.y * TILE_SIZE;
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
    	animatePlayer();
    }
    
    drawPlayer();

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

void take_input(int key, int x, int y) {
	if (animation_counter > 0 && animation_counter < ANIMATION_STEPS) {
		return; // IN THE MIDDLE OF ANIMATION
	}
	
    switch (key) {
        case GLUT_KEY_UP:
            player.dir.x = 0.0f;
            player.dir.y = 1.0f;
            break;
        case GLUT_KEY_DOWN:
            player.dir.x = 0.0f;
            player.dir.y = -1.0f;
            break;
        case GLUT_KEY_LEFT:
            player.dir.x = -1.0f;
            player.dir.y = 0.0f;
            break;
        case GLUT_KEY_RIGHT:
            player.dir.x = 1.0f;
            player.dir.y = 0.0f;
            break;
        default:
            return;
    }
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
