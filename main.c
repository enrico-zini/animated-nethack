#include <GL/glut.h>
#include <stdio.h>

#define TILE_SIZE 32
#define GRID_WIDTH 512
#define GRID_HEIGHT 512
#define ANIMATION_STEPS 50

int windowWidth = GRID_WIDTH;
int windowHeight = GRID_HEIGHT;

typedef struct Coord2D {
	float x;
	float y;
} Coord2D;

Coord2D player_position = {0, 0};
Coord2D player_direction = {0, 0};

int animating = 0;
int animation_counter = 1;

void drawGrid() {
    glColor3f(1.0f, 1.0f, 1.0f);
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

void drawPlayer() {
    glColor3f(1.0f, 1.0f, 1.0f);
   	if (animating) {
   		float x = (player_position.x + (player_direction.x * animation_counter/ANIMATION_STEPS)) * TILE_SIZE;
   		float y = (player_position.y + (player_direction.y * animation_counter/ANIMATION_STEPS)) * TILE_SIZE;
	   	glBegin(GL_QUADS);
	        glVertex2i(x, y);
	        glVertex2i(x + TILE_SIZE, y);
	        glVertex2i(x + TILE_SIZE, y + TILE_SIZE);
	        glVertex2i(x, y + TILE_SIZE);
   		glEnd();
   		animation_counter++;
   		if (animation_counter > ANIMATION_STEPS) {
   			animating = 0;
   			animation_counter = 1;
   			player_position.x += player_direction.x;
    		player_position.y += player_direction.y;
    		return;
   		}
   	} else {
   		glBegin(GL_QUADS);
   		   	float x = player_position.x * TILE_SIZE;
   			float y = player_position.y * TILE_SIZE;
	        glVertex2i(x, y);
	        glVertex2i(x + TILE_SIZE, y);
	        glVertex2i(x + TILE_SIZE, y + TILE_SIZE);
	        glVertex2i(x, y + TILE_SIZE);
  		glEnd();
   	}
   	glutPostRedisplay();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef((windowWidth - GRID_WIDTH) / 2.0f, (windowHeight - GRID_HEIGHT) / 2.0f, 0);

    drawGrid();
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

    glutPostRedisplay();
}

void take_input(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: 
			player_direction.x = 0.0f;
			player_direction.y = 1.0f;
			break;
		case GLUT_KEY_DOWN:
			player_direction.x = 0.0f;
			player_direction.y = -1.0f;		
			break;
		case GLUT_KEY_LEFT:
			player_direction.x = -1.0f;
			player_direction.y = 0.0f;		
			break;
		case GLUT_KEY_RIGHT:
			player_direction.x = 1.0f;
			player_direction.y = 0.0f;
			break;
	}
	animating = 1;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Animated Nethack");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(take_input);

    glutMainLoop();
    return 0;
}
