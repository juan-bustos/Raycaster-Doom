#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

const int HORIZONTAL_WINDOW_DIMENSION = 1024;
const int VERTICAL_WINDOW_DIMENSION = 512;
const char WINDOW_NAME[] = "Follow along RayCaster Tutorial";
const int PLAYER_MOVEMENT = 5;
const int PLAYER_START_POSITION = 300;
const int MAP_DIMENSIONS = 8;
const int MAP_SIZE = 64;

int mapX = MAP_DIMENSIONS, mapY=MAP_DIMENSIONS, mapS=MAP_SIZE;
int map[] = {
1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,
};

float px,py; //player position

void buttons(unsigned char key, int x, int y) {
  if (key == 'a') { px -= PLAYER_MOVEMENT; }
  if (key == 'd') { px += PLAYER_MOVEMENT; }
  if (key == 'w') { py -= PLAYER_MOVEMENT; }
  if (key == 's') { py += PLAYER_MOVEMENT; }
  glutPostRedisplay();
}

void drawPlayer() {
  glColor3f(1, 1, 0); //this is yellow
  glPointSize(8);
  glBegin(GL_POINTS);
  glVertex2i(px,py);
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawPlayer();
  glutSwapBuffers();
}

void init() {
  glClearColor(0.3,0.3,0.3,0);
  gluOrtho2D(0,HORIZONTAL_WINDOW_DIMENSION,VERTICAL_WINDOW_DIMENSION,0);
  px=PLAYER_START_POSITION;
  py=PLAYER_START_POSITION;
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(HORIZONTAL_WINDOW_DIMENSION, VERTICAL_WINDOW_DIMENSION);
  glutCreateWindow(WINDOW_NAME);
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(buttons);
  glutMainLoop();
}
