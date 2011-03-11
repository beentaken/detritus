#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

typedef struct _polywog {
	struct _polywog	*next;
	int		color_index;
	int		n_vert;
	int		**verticies;
} polywog;

typedef void (*draw_func)(int, int);

int	ww = 500, wh = 500;
int	icoord = 0, isline = 0, psize = 1;
int	color_index = 0;
int	num_verticies = 3;
int	num_verticies_done = 0;

float colour[7][3] = {
		{1., 1., 1.},
		{1., 0., 0.},
		{0., 1., 0.},
		{0., 0., 1.},
		{0., 1., 1.},
		{1., 0., 1.},
		{1., 1., 0.}
	};

polywog *head = NULL;

int	SP_DX = 0;
int	SP_DY = 0;

enum	SP_MODES {
		_NOTHING = 0x0,
		_FLIP = 0x1,
		_SWAP = 0x2
	};

int	SP_MODE = 0;

enum	line_types {
		_gl_lines,
		_bresenham_lines,
		_dda,
		_simple_dda,
		_symmetric_dda
	};

int	line_type = _gl_lines;

void SetPixel(int x, int y) {
	if (SP_MODE & _SWAP) {
		x += y;
		y = x - y;
		x = x - y;
	}

	if (SP_MODE & _FLIP) {
		y *= -1;
	}

	x += SP_DX;
	y += SP_DY;

	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();

	return;
}

void BresLine(int dx, int dy) {
	int	p = dy + dy  - dx;
	int	c1 = dy + dy;
	int	c2 = (dy - dx) << 1;

	int	x, y;
	
	x = y = 0;

	SetPixel(0, 0);

	while (x < dx) {
		x++;
		if (p < 0) {
			p += c1;
		} else {
			y++;
			p += c2;
		}

		SetPixel(x, y);
	}
}

void preLineDraw(int x1, int y1, int x2, int y2, void (*drawline)(int, int)) {
	int	dx, dy;

	SP_MODE = _NOTHING;

	/* Check to see if the points need to be swapped */
	if (x1 > x2) {
		SP_DX = x2;
		SP_DY = y2;
		dx = x1 - SP_DX;
		dy = y1 - SP_DY;
	} else {
		SP_DX = x1;
		SP_DY = y1;
		dx = x2 - SP_DX;
		dy = y2 - SP_DY;
	}

	/* Check to see if a flip is needed */
	if (dy < 0) {
		SP_MODE |= _FLIP;
		dy *= -1;
	}
	
	/* If we are moving in units of Y, change that to X */
	if (dy > dx) {
		SP_MODE |= _SWAP;
		drawline(dy, dx);
	} else {
		drawline(dx, dy);
	}
}

void Bresenham(int x1, int y1, int x2, int y2) {
	preLineDraw(x1, y1, x2, y2, BresLine);
}

void DDA(int dx, int dy) {
	float	e, edy, edx, x, y;

	x = y = 0.;

	e = 1. / sqrt(dx * dx + dy * dy);
	edy = e * dy;
	edx = e * dx;

	SetPixel(0, 0);

	while (x < dx) {
		x += edx;
		y += edy;

		SetPixel((int) (x + 0.5), (int) (y + 0.5));
	}
}

void SimpleDDA(int dx, int dy) {
	float	e = 1./ dx * dy;
	float	y = 0.;

	int	x;

	x = 0;

	SetPixel(0, 0);

	while (x < dx) {
		x++;
		y += e;

		SetPixel((int) (x + 0.5), (int) (y + 0.5));
	}
}

void SymmetricDDA(int dx, int dy) {
	int	n = 1;
	float	x, y, e, edx, edy;

	x = y = 0.;

	while ((2 << n) < dx) {
		n++;
	}

	e = 1. / (2 << n);

	edx = e * dx;
	edy = e * dy;

	SetPixel(0, 0);
	
	while (x < dx) {
		x += edx;
		y += edy;

		SetPixel((int) (x + 0.5), (int) (y + 0.5));
	}
}

polywog * bake_polywog(int n_vert, int color_index) {
	polywog	*poly = (polywog *)calloc(1, sizeof(polywog));

	if (!poly) {
		perror("__LINE__");
		exit(1);
	} else {
		int	i;

		poly->n_vert = -n_vert;
		poly->color_index = color_index;
		poly->verticies = (int **)calloc(n_vert, sizeof(int *));

		for (i = 0; i < n_vert; i++) {
			poly->verticies[i] = (int *)calloc(2, sizeof(int));

			if (!poly->verticies[i]) {
				perror("__LINE__");
				exit(1);
			}
		}

		if (!poly->verticies) {
			perror("__LINE__");
			exit(1);
		}
	}

	return (poly);
}

void eat_polywog(polywog *poly) {
	int	i;

	assert(poly);
	assert(poly->verticies);

	if (poly->n_vert < 0) {
		poly->n_vert = -1 * poly->n_vert;
	}

	for (i = 0; i < poly->n_vert; i++) {
		assert(poly->verticies[i]);

		free(poly->verticies[i]);
	}

	free(poly->verticies);
	free(poly);
}

void consume_polywogs(polywog *list) {
	polywog	*curr;

	while (head) {
		curr = head->next;
		eat_polywog(head);
		head = curr;
	}
}

void quit(int rtcode) {
	consume_polywogs(head);

	exit(rtcode);
}

void transform_common(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., (GLdouble) ww / psize, 0., (GLdouble) wh / psize);
	glMatrixMode(GL_MODELVIEW);
}

void myinit (void) {
/*	attributes of lines, background, etc */
	glClearColor(0., 0., 0., 1.);		/* black background */
	glColor3f(1., 1., 1.);			/* draw in white */
	glShadeModel(GL_FLAT);
/*	set up viewing 500 x 500 window with origin lower left */
	glViewport(0, 0, ww, wh);
	transform_common();
}
	
void keyboard(unsigned char key, int x, int y) {
	if (key >= '3' && key <= '9') {
		num_verticies = key - '0';
	}
}

void mouse (int btn, int state, int x, int y) {
	polywog	*poly = NULL;

	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (num_verticies_done == 0) {
			/* We need a new polywog */
			poly = bake_polywog(num_verticies, color_index);
			
			poly->next = head;
			head = poly;
		}

		head->verticies[num_verticies_done][0] = x / psize;
		head->verticies[num_verticies_done][1] = (wh - y) / psize;

		num_verticies_done++;

		if (num_verticies_done == -1 * head->n_vert) {
			num_verticies_done = 0;

			/* OK, change the sign of poly->n_vert so that we know that this polywog is finished */
			head->n_vert = head->n_vert * -1;
			glutPostRedisplay();
		}
	}
}

void middle_menu (int id) {
	color_index = id;
}

void right_menu (int id) {
	switch (id) {
		case 1:	/* increase pixel size */
			psize++;
			break;

		case 2:	/* decrease pixel size */
			if (psize > 1) {
				psize--;
			}
			break;

		case 3:	/* quit */
			quit(0);
			break;

		case 4: /* Clear the window */
			consume_polywogs(head);
			num_verticies_done = 0;
			break;

		case 5:	/* GL_LINES */
			line_type = _gl_lines;
			break;

		case 6:	/* Bresenham lines */
			line_type = _bresenham_lines;
			break;

		case 7:	/* DDA lines */
			line_type = _dda;
			break;

		case 8:	/* SimpleDDA lines */
			line_type = _simple_dda;
			break;

		case 9:	/* SymmetricDDA lines */
			line_type = _symmetric_dda;
			break;
	}

	glPointSize((GLfloat) psize);
	glLineWidth((GLfloat) psize);
	transform_common();
	glutPostRedisplay();
}

void display (void)
{
	polywog	*poly = head;

	glClear(GL_COLOR_BUFFER_BIT);

	while (poly) {
		int	i, id = poly->color_index;
		int	**v = poly->verticies;

		if (poly->n_vert >= 0) {
			glColor3f(colour[id][0], colour[id][1], colour[id][2]);

			if (line_type == _gl_lines) {
				glBegin(GL_LINES);

				for (i = 0; i < poly->n_vert - 1; i++) {
					glVertex2iv(v[i]);
					glVertex2iv(v[i + 1]);
				}

				glVertex2iv(v[i]);
				glVertex2iv(v[0]);

				glEnd();
			} else if (line_type == _bresenham_lines) {
				for (i = 0; i < poly->n_vert - 1; i++) {
					Bresenham(v[i][0], v[i][1], v[i + 1][0], v[i + 1][1]);
				}

				Bresenham(v[i][0], v[i][1], v[0][0], v[0][1]);
			} else {
				draw_func	foo;

				if (line_type == _dda) {
					foo = DDA;
				} else if (line_type == _simple_dda) {
					foo = SimpleDDA;
				} else if (line_type == _symmetric_dda) {
					foo = SymmetricDDA;
				}

				for (i = 0; i < poly->n_vert - 1; i++) {
					preLineDraw(v[i][0], v[i][1], v[i + 1][0], v[i + 1][1], foo);
				}

				preLineDraw(v[i][0], v[i][1], v[0][0], v[0][1], foo);
			}
		}

		poly = poly->next;
	}

	glFlush();
}

void reshape(int neww, int newh) {
	wh = newh;
	ww = neww;
	glViewport(0, 0, ww, wh);
	transform_common();
	display(); 
}
		
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("demo");
	glutDisplayFunc(display);
	glutCreateMenu(middle_menu);
		glutAddMenuEntry("White", 0);
		glutAddMenuEntry("Red", 1);
		glutAddMenuEntry("Green", 2);
		glutAddMenuEntry("Blue", 3);
		glutAddMenuEntry("Cyan", 4);
		glutAddMenuEntry("Magenta", 5);
		glutAddMenuEntry("Yellow", 6);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutCreateMenu(right_menu);
		glutAddMenuEntry("increase pixel size", 1);
		glutAddMenuEntry("decrease pixel size", 2);
		glutAddMenuEntry("GL_LINES", 5);
		glutAddMenuEntry("Bresenham", 6);
		glutAddMenuEntry("DDA", 7);
		glutAddMenuEntry("SimpleDDA", 8);
		glutAddMenuEntry("SymmetricDDA", 9);
		glutAddMenuEntry("clear window", 4);
		glutAddMenuEntry("exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	myinit();
	glutReshapeFunc(reshape); 
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
}
