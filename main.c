#include <GL/glut.h>

#define TILE_SIZE 32
#define GRID_WIDTH 512
#define GRID_HEIGHT 512

int windowWidth = GRID_WIDTH;
int windowHeight = GRID_HEIGHT;

void drawGrid()
{
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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef((windowWidth - GRID_WIDTH) / 2.0f, (windowHeight - GRID_HEIGHT) / 2.0f, 0);

    drawGrid();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Animated Nethack");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
