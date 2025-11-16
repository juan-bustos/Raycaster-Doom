#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2

const int HORIZONTAL_WINDOW_DIMENSION = 1024;
const int VERTICAL_WINDOW_DIMENSION = 512;
const double PLAYER_MOVEMENT = 0.1;
const int PLAYER_START_POSITION = 300;
const int MAP_DIMENSIONS = 8;
const int MAP_SIZE = 64;

int mapX = MAP_DIMENSIONS, mapY = MAP_DIMENSIONS, mapS = MAP_SIZE;
// clang-format off
int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

// clang-format on
float px, py, pdx, pdy, pa; // player position

void drawMap2D() {
  int x, y, xo, yo;
  for (y = 0; y < mapY; y++) {
    for (x = 0; x < mapX; x++) {
      if (map[y * mapX + x] == 1) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      }
      xo = x * mapS;
      yo = y * mapS;
      glBegin(GL_QUADS);
      glVertex2i(xo + 1, yo + 1);
      glVertex2i(xo + 1, yo + mapS - 1);
      glVertex2i(xo + mapS - 1, yo + mapS - 1);
      glVertex2i(xo + mapS - 1, yo + 1);
      glEnd();
    }
  }
}

void drawRays3D() {
  int ray, mx, my, mapPosition, depthOfField;
  float rayX, rayY, rayAngle, xOffset, yOffset;

  rayAngle = pa;
  for (ray = 0; ray < 1; ray++) {

    // Check Horizontal Lines
    depthOfField = 0;
    float aTan = -1 / tan(rayAngle);
    if (rayAngle > PI) {
      rayY = (((int)py >> 6) << 6) - 0.0001;
      rayX = (py - rayY) * aTan + px;
      yOffset = -64;
      xOffset = -yOffset * aTan;
    }

    if (rayAngle < PI) {
      rayY = (((int)py >> 6) << 6) + 64;
      rayX = (py - rayY) * aTan + px;
      yOffset = 64;
      xOffset = -yOffset * aTan;
    }

    if (rayAngle == 0 || rayAngle == PI) {
      rayX = px;
      rayY = py;
      depthOfField = 8;
    }

    while (depthOfField < 8) {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mapPosition = my * mapX + mx;

      // if we hit a wall, break out of this loop
      if (mapPosition > 0 && mapPosition < mapX * mapY && map[mapPosition] == 1) {
        depthOfField = 8;
      } else {
        rayX += xOffset;
        rayY += yOffset;
        depthOfField += 1;
      }
      glColor3f(0, 1, 0);
      glLineWidth(1);
      glBegin(GL_LINES);
      glVertex2i(px, py);
      glVertex2i(rayX, rayY);
      glEnd();
    }

    // Check Vertical Lines
    depthOfField = 0;
    float nTan = -tan(rayAngle);
    // Looking Left
    if (rayAngle > P2 && rayAngle < P3) {
      rayX = (((int)px >> 6) << 6) - 0.0001;
      rayY = (px - rayX) * nTan + py;
      xOffset = -64;
      yOffset = -xOffset * nTan;
    }

    // Looking Right
    if (rayAngle < P2 || rayAngle > P3) {
      rayX = (((int)px >> 6) << 6) + 64;
      rayY = (px - rayX) * nTan + py;
      xOffset = 64;
      yOffset = -xOffset * nTan;
    }

    if (rayAngle == 0 || rayAngle == PI) {
      rayX = px;
      rayY = py;
      depthOfField = 8;
    }

    while (depthOfField < 8) {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mapPosition = my * mapX + mx;

      // if we hit a wall, break out of this loop
      if (mapPosition > 0 && mapPosition < mapX * mapY && map[mapPosition] == 1) {
        depthOfField = 8;
      } else {
        rayX += xOffset;
        rayY += yOffset;
        depthOfField += 1;
      }
      glColor3f(1, 0, 0);
      glLineWidth(1);
      glBegin(GL_LINES);
      glVertex2i(px, py);
      glVertex2i(rayX, rayY);
      glEnd();
    }
  }
}

void buttons(unsigned char key, int x, int y) {
  if (key == 'a') {
    pa -= PLAYER_MOVEMENT;
    if (pa < 0) {
      pa += 2 * PI;
    }
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
  }
  if (key == 'd') {
    pa += PLAYER_MOVEMENT;
    if (pa > 2 * PI) {
      pa -= 2 * PI;
    }
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
  }
  if (key == 'w') {
    px += pdx;
    py += pdy;
  }
  if (key == 's') {
    px -= pdx;
    py -= pdy;
  }
  glutPostRedisplay();
}

void drawPlayer() {
  glColor3f(1, 1, 0); // this is yellow
  glPointSize(8);
  glBegin(GL_POINTS);
  glVertex2i(px, py);
  glEnd();

  glLineWidth(3);
  glBegin(GL_LINES);
  glVertex2i(px, py);
  glVertex2i(px + pdx * 5, py + pdy * 5);
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawMap2D();
  drawRays3D();
  drawPlayer();
  glutSwapBuffers();
}

void init() {
  glClearColor(0.3, 0.3, 0.3, 0);
  gluOrtho2D(0, HORIZONTAL_WINDOW_DIMENSION, VERTICAL_WINDOW_DIMENSION, 0);
  px = PLAYER_START_POSITION;
  py = PLAYER_START_POSITION;
  pdx = cos(pa) * 5;
  pdy = sin(pa) * 5;
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(HORIZONTAL_WINDOW_DIMENSION, VERTICAL_WINDOW_DIMENSION);
  glutCreateWindow("Follow along tutorial");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(buttons);
  glutMainLoop();
}
