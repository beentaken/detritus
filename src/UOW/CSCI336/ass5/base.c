/* 
	Author:		Peter Nathaniel Theodore de Zwart
	Student ID:	9840642

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

typedef float point3[4];

/* initial tetrahedron */

point3 v[]= {
		{0.0, 0.0, 1.0},
		{0.0, 0.942809, -0.33333},
		{-0.816497, -0.471405, -0.333333},
		{0.816497, -0.471405, -0.333333}
	};

point3 eye = {0.0, 0.0, 1.25};

int n;
int mode;
int rotating;
GLfloat aspect;

GLfloat light_pos[] = {-1.0, 1.0, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess = 100.0;

/* display one triangle using a line loop for wire frame, a single normal for constant shading, or three normals for interpolative shading */
void triangle(point3 a, point3 b, point3 c) {
	if (mode == 0)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_POLYGON);

	if (mode == 1)
		glNormal3fv(a);

	if (mode == 2)
		glNormal3fv(a);

	glVertex3fv(a);

	if (mode == 2)
		glNormal3fv(b);

	glVertex3fv(b);

	if(mode == 2)
		glNormal3fv(c);

	glVertex3fv(c);

	glEnd();
}

void Normal(point3 p) {
	float	d = 0.0;
	int	i;

	/* normalize a vector */

	for(i = 0; i < 3; i++)
		d += p[i]*p[i];

	d = sqrt(d);

	if(d > 0.0)
		for(i = 0; i < 3; i++)
			p[i] /= d;
}

/* triangle subdivision using vertex numbers righthand rule applied to create outward pointing faces */
void divide_triangle(point3 a, point3 b, point3 c, int m) {
	point3	v1, v2, v3;
	int	j;

	if (m > 0) {
		for (j = 0; j < 3; j++)
			v1[j] = a[j] + b[j];

		Normal(v1);

		for(j = 0; j < 3; j++)
			v2[j] = a[j] + c[j];

		Normal(v2);

		for (j = 0; j < 3; j++)
			v3[j] = b[j] + c[j];

		Normal(v3);

		divide_triangle(a, v1, v2, m-1);
		divide_triangle(c, v2, v3, m-1);
		divide_triangle(b, v3, v1, m-1);
		divide_triangle(v1, v3, v2, m-1);
	} else {
		/* draw triangle at end of recursion */
		triangle(a, b, c);
	}
}

/* Apply triangle subdivision to faces of tetrahedron */
void tetrahedron(int m) {
	divide_triangle(v[0], v[1], v[2], m);
	divide_triangle(v[3], v[2], v[1], m);
	divide_triangle(v[0], v[3], v[1], m);
	divide_triangle(v[0], v[2], v[3], m);
}

/* Displays all three modes, side by side */
void display(void) {
	static float object_angle = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluPerspective(45, aspect, 1, -1);
	gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	if (rotating) {
		object_angle += 5;
	}

	if (rotating == 3) {
		glRotatef(object_angle, 0, 1, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

		tetrahedron(n);
	} else if (rotating == 2) {
		tetrahedron(n);

		glRotatef(object_angle, 0, 1, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	} else if (rotating == 1) {
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

		glRotatef(object_angle, 0, 1, 0);

		tetrahedron(n);
	} else {
		tetrahedron(n);

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	int     redisplay = 1;

	if (key >= '0' && key <= '9') {
		n = key - '0';
	} else {
		switch (key) {
			case	'p':	mat_shininess -= 5;
					break;

			case	'P':	mat_shininess += 5;
					break;

			case	'm':
			case	'M':	if (rotating == 3) {
						rotating = 1;
					} else if (rotating == 2) {
						rotating = 0;
						glutIdleFunc(NULL);
					} else if (rotating == 1) {
						rotating = 3;
					} else {
						rotating = 2;
						glutIdleFunc(display);
					}
					break;

			case	'A':
			case	'a':	if (rotating == 3) {
						rotating = 2;
					} else if (rotating == 2) {
						rotating = 3;
					} else if (rotating == 1) {
						rotating = 0;
						glutIdleFunc(NULL);
					} else {
						rotating = 1;
						glutIdleFunc(display);
					}
					break;

			case	'r':	light_diffuse[0] -= 0.1;
					break;

			case	'R':	light_diffuse[0] += 0.1;
					break;

			case	'g':	light_diffuse[1] -= 0.1;
					break;

			case	'G':	light_diffuse[1] += 0.1;
					break;

			case	'b':	light_diffuse[2] -= 0.1;
					break;

			case	'B':	light_diffuse[2] += 0.1;
					break;

			case	'w':	
			case	'W':	mode = 0;
					break;

			case	'f':
			case	'F':	mode = 1;
					break;

			case	's':
			case	'S':	mode = 2;
					break;

			case    'q':
			case    'Q':    exit(0);

			case	'_':
			case	'-':	if (n > 0) {
						n--;
					}
					break;

			case	'=':
			case	'+':	n++;
					break;

			default:        redisplay = 0;
		}
	}

	if (redisplay) {
		glutPostRedisplay();
	}
}

void myReshape(int w, int h) {
	aspect = w/h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w, 4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-4.0 * (GLfloat) w / (GLfloat) h, 4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	display();
}

void myinit() {
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

	/* set up ambient, diffuse, and specular components for light 0 */

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	/* define material proerties for front face of all polygons */

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0);  /* enable light 0 */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */

	glClearColor (0.0, 0.0, 0.0, 1.0);
	glColor3f (1.0, 1.0, 1.0);
}

int main(int argc, char **argv) {
	n = 0;
	mode = 1;
	aspect = 1.0;
	rotating = 0;

	if (argc == 4) {
		eye[0] = atof(argv[1]);
		eye[1] = atof(argv[2]);
		eye[2] = atof(argv[3]);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("sphere");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
}
