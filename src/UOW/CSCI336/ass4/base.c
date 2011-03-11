/* 
	Author:		Peter Nathaniel Theodore de Zwart
	Student ID:	9840642

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include "Ctransform3.h"

/* Using homogeneous coordinates */
#define	NUM_COORDINATES	4

typedef	double point[NUM_COORDINATES];
typedef int line[2];

int _num_points = 0;
int _num_lines = 0;
point *_points = NULL;
line *_lines = NULL;

int	_maxdim;
int	_x1, _y1, _x2, _y2;

void quit(int rtcode) {
	if (_points) {
		free(_points);
	}

	if (_lines) {
		free(_lines);
	}

	exit(rtcode);
}

void keyboard(unsigned char key, int x, int y) {
	int	redisplay = 1;

	switch (key) {
		case	'q':
		case	'Q':	quit(0);

		default:	redisplay = 0;
	}

	if (redisplay) {
		glutPostRedisplay();
	}
}

void mouse(int btn, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (btn == GLUT_LEFT_BUTTON) {
		} else if (btn == GLUT_RIGHT_BUTTON) {
		}
	}
}

void DrawLines(void) {
	int	i;

	for (i = 0; i < _num_lines; i++) {
		point	p = { 0. };

		glBegin(GL_LINES);

		transform2dv(_points[_lines[i][0]], p);
		glVertex2dv(p);
		transform2dv(_points[_lines[i][1]], p);
		glVertex2dv(p);

		glEnd();
	}
}

void display(void) {
	glViewport(_x1, _y1, _maxdim, _maxdim);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT);

	DrawLines();

	glViewport(_x2, _y2, _maxdim, _maxdim);

	DrawLines();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (w < h) {
		if (w < (h >> 1)) {
			_maxdim = w;
		} else {
			_maxdim = h >> 1;
		}

		_x1 = (w - _maxdim) >> 1;
		_y1 = (((h >> 1) - _maxdim) >> 1) + (h >> 1);
		_x2 = _x1;
		_y2 = _y1 - (h >> 1);
	} else {
		if (h < (w >> 1)) {
			_maxdim = h;
		} else {
			_maxdim = w >> 1;
		}

		_x1 = ((w >> 1) - _maxdim) >> 1;
		_y1 = (h - _maxdim) >> 1;
		_x2 = _x1 + (w >> 1);
		_y2 = _y1;
	}

	display(); 
}
		
void load(char *filename) {
	FILE	*file = NULL;

	file = fopen(filename, "r");

	if (file) {
		fscanf(file, "%d", &_num_points);

		_points = (point *) calloc(_num_points, sizeof(point));
		if (_points) {
			_lines = (line *) calloc(_num_points * 3, sizeof(line));

			if (_lines) {
				int	i;

				for (i = 0; i < _num_points; i++) {
					fscanf(file, "%lf %lf %lf", &_points[i][0], &_points[i][1], &_points[i][2]);
					_points[i][3] = 1.;
				}

				for (i = 0; i < _num_points * 3; i++) {
					int	a, b;

					fscanf(file, "%d %d", &a, &b);

					if (a == -1 && b == -1) {
						break;
					} else {
						_lines[i][0] = a;
						_lines[i][1] = b;
					}
				}

				_num_lines = i;
			} else {
				perror("main()");
				quit(4);
			}
		} else {
			perror("main()");
			quit(3);
		}

		fclose(file);
	} else {
		perror(filename);
		quit(2);
	}
}

int main(int argc, char** argv) {
	int	init_x, init_y;

	double	origin[4] = { 0, 0, 0, 1 };
	double	up[4] = { 0, 1, 0, 1 };
	double	eye[4] = { 0, 0, 0, 1 };

	double	D = 2;

	if (argc < 5) {
		fprintf(stderr, "%s\n%s\n%s\n%s\n%s\n%s\n",
			"Error:\tIncorrect number of arguments",
			"Usage:\tass4 file x y z",
			"\tfile\tData file containing the scene",
			"\tx\tEye point x ordinate",
			"\ty\tEye point y ordinate",
			"\tz\tEye point z ordinate");
		
		quit(1);
	} else {
		load(argv[1]);

		eye[0] = atof(argv[2]);
		eye[1] = atof(argv[3]);
		eye[2] = atof(argv[4]);
	}

	init_x = 600;
	init_y = 300;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(init_x, init_y);
	glutCreateWindow("demo");
	glutDisplayFunc(display);

/*	attributes of lines, background, etc */
	glClearColor(0., 0., 0., 1.);		/* black background */
	glColor3f(1., 1., 1.);			/* draw in white */
	glShadeModel(GL_FLAT);

	reshape(init_y, init_x);
	
	init();
	view(eye, origin, up, D);
	perspective(D);

	glutReshapeFunc(reshape); 
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
}
