#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include "vector.c"

#define numTicks 1000 // max ~= 1000
double turn = 0.1;
double mag = 1e2 / numTicks;
double grav = 1e2 / numTicks;

double randNum() {
	return ((double) rand()) / RAND_MAX * 2 - 1;
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	float aspect = ((float) width) / ((float) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.01 * aspect, 0.01 * aspect, -0.01, 0.01, 0.01, 1000);
}

struct Camera {
	Vector pos;
	double turnX, turnY;
	int canTurn;
	double mouseX, mouseY;
} camera;

void updateViewMatrix() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(camera.turnX, 1, 0, 0);
	glRotatef(camera.turnY, 0, 1, 0);
	glTranslatef(camera.pos.x, camera.pos.y, camera.pos.z);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) camera.canTurn = 0;
}

void motion(int x, int y) {
	if (camera.canTurn) {
		camera.turnY += turn * (camera.mouseX - x);
		camera.turnX += turn * (camera.mouseY - y);
		camera.turnX = camera.turnX > 90? 90: 
		               camera.turnX < -90? -90: camera.turnX;
		updateViewMatrix();
	}
	else camera.canTurn = 1;
	camera.mouseX = x;
	camera.mouseY = y;
}

struct Tick {
	Vector pos, vel, acc;
	double mag;
} ticks[numTicks];

double last;
void physics() {
	double delta = (clock() - last) / CLOCKS_PER_SEC;
	last = clock();

	for (uint i = 0; i < numTicks; ++i) {
		ticks[i].acc = (Vector) {0, 0, 0};
		for (uint j = 0; j < numTicks; ++j) {
			if (i != j) {
				Vector diff = sub(ticks[i].pos, ticks[j].pos);
				double r = length(diff);
				if (r > 0.1) ticks[i].acc = add(ticks[i].acc, 
					scale(normalize(diff), 
						(mag * ticks[i].mag * ticks[j].mag - grav) / 
						pow(r, 2)
					)
				);
			}
		}
	}

	for (uint i = 0; i < numTicks; ++i) {
		ticks[i].vel = add(ticks[i].vel, scale(ticks[i].acc, delta));
		ticks[i].pos = add(ticks[i].pos, scale(ticks[i].vel, delta));
	}

	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (int i = 0; i < numTicks; ++i) {
		glColor3f(ticks[i].mag, 0, -ticks[i].mag);
		glVertex3f(ticks[i].pos.x, ticks[i].pos.y, ticks[i].pos.z);
	}
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	srand(time(NULL));

	for (int i = 0; i < numTicks; ++i) {
		ticks[i].pos = (Vector) {randNum() * 10, randNum() * 10, randNum() * 10};
		ticks[i].mag = rand() > (((float) RAND_MAX) / 2)? 1.0: -1.0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 1000);
	glutCreateWindow("Magneticks");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1, 1, 1, 1);
	glPointSize(5);
	camera.pos.z = -5;
	updateViewMatrix();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(physics);

	last = clock();
	glutMainLoop();
}
