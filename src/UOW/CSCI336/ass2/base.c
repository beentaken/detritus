#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#define	_NUM_ORDINATES	4
#define _NUM_LINES	100
#define	_ROTATION	5

typedef struct _rect {
	double	points[_NUM_ORDINATES];
	struct _rect	*next;
} Rect;

enum	_clipping_types {
		_unclipped,
		_clipped,
		_blanked
	};

enum	_line_types {
		_normal,
		_rotate,
		_starburst
	};

enum	_ordinates {
		_x1,
		_y1,
		_x2,
		_y2
	};

enum	_colours {
		_white,
		_red,
		_green,
		_blue,
		_cyan,
		_magenta,
		_yellow
	};

float _colour[7][3] = {
		{ 1., 1., 1. },
		{ 1., 0., 0. },
		{ 0., 1., 0. },
		{ 0., 0., 1. },
		{ 0., 1., 1. },
		{ 1., 0., 1. },
		{ 1., 1., 0. }
	};

double _lines[_NUM_LINES][_NUM_ORDINATES];

int	_ww = 500, _wh = 500, _line = 0, _making_clipper = 0, _making_line = 0;
int	_line_type;
int	_clipping_type;

/* GRUMBLE GRUMBLE strict ANSI GRUMBLE GRUMBLE */
const double	_rads = _ROTATION * 3.14159265358979323846 / 180;
double	_cos_t;
double	_sin_t;

Rect	*head = NULL;

int ClipTest(double p, double q, double *u1, double *u2) {
	double	r = 0.;
	int	cliptest = 1;

	if (p < 0) {
		r = q / p;

		if (r > *u2) {
			cliptest = 0;
		} else if (r > *u1) {
			*u1 = r;
		}
	} else if (p > 0) {
		r = q / p;

		if (r < *u1) {
			cliptest = 0;
		} else if (r < *u2) {
			*u2 = r;
		}
	} else if (q < 0) {
		cliptest = 0;
	}

	return(cliptest);
}

void ClipLine(double x1, double y1, double x2, double y2, Rect *clip_rect) {
	double	u1, u2, dx, dy, xLeft, xRight, yBottom, yTop;

	assert(clip_rect);

	xLeft = clip_rect->points[_x1];
	xRight = clip_rect->points[_x2];
	yBottom = clip_rect->points[_y1];
	yTop = clip_rect->points[_y2];

	u1 = 0;
	u2 = 1;

	dx = x2 - x1;

	if (ClipTest(-dx, x1 - xLeft, &u1, &u2)) {
		if (ClipTest(dx, xRight - x1, &u1, &u2)) {
			dy = y2 - y1;

			if (ClipTest(-dy, y1 - yBottom, &u1, &u2)) {
				if (ClipTest(dy, yTop - y1, &u1, &u2)) {
					if (u2 < 1) {
						x2 = x1 + u2 * dx;
						y2 = y1 + u2 * dy;
					}

					if (u1 > 0) {
						x1 = x1 + u1 * dx;
						y1 = y1 + u1 * dy;
					}

					/* Now draw the line from (x1, y1) to (x2, y2) */
					glColor3fv(_colour[_red]);

					glBegin(GL_LINES);
						glVertex2f(x1, y1);
						glVertex2f(x2, y2);
					glEnd();
				}
			}
		}
	}
}

void BlankLine(double x1, double y1, double x2, double y2, Rect *clip_rect) {
	double	u1, u2, dx, dy, xLeft, xRight, yBottom, yTop;

	if (!clip_rect) {
		/* Now draw the line from (x1, y1) to (x2, y2) */
		glColor3fv(_colour[_blue]);

		glBegin(GL_LINES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		glEnd();
	} else {
		double	x1b, y1b, x2b, y2b;

		xLeft = clip_rect->points[_x1];
		xRight = clip_rect->points[_x2];
		yBottom = clip_rect->points[_y1];
		yTop = clip_rect->points[_y2];

		u1 = 0;
		u2 = 1;

		dx = x2 - x1;
		dy = y2 - y1;

		clip_rect = clip_rect->next;

		if (ClipTest(-dx, x1 - xLeft, &u1, &u2) &&
			ClipTest(dx, xRight - x1, &u1, &u2) &&
			ClipTest(-dy, y1 - yBottom, &u1, &u2) &&
			ClipTest(dy, yTop - y1, &u1, &u2)) {

			if (u2 < 1) {
				x2b = x1 + u2 * dx;
				y2b = y1 + u2 * dy;
				BlankLine(x2, y2, x2b, y2b, clip_rect);
			}

			if (u1 > 0) {
				x1b = x1 + u1 * dx;
				y1b = y1 + u1 * dy;
				BlankLine(x1, y1, x1b, y1b, clip_rect);
			}
		} else {
			BlankLine(x1, y1, x2, y2, clip_rect);
		}
	}
}

void DrawFLine(double x1, double y1, double x2, double y2, int clipping) {
	if (clipping == _clipped) {
		Rect	*curr = NULL;

		if (head) {
			curr = head;

			while (curr) {
				ClipLine(x1, y1, x2, y2, curr);
				curr = curr->next;
			}
		}
	} else if (clipping == _blanked) {
		BlankLine(x1, y1, x2, y2, head);
	} else {
		glColor3fv(_colour[_white]);

		glBegin(GL_LINES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		glEnd();
	}
}

void purge_Rects(void) {
	Rect	*tmp = NULL;

	while (head) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
		
void quit(int rtcode) {
	purge_Rects();
	exit(rtcode);
}

Rect * create_Rect(void) {
	Rect	*rect = (Rect *)calloc(1, sizeof(Rect));

	if (!rect) {
		perror("__LINE__");
		quit(1);
	} else {
		int	i;

		for (i = 0; i < _NUM_ORDINATES; i++) {
			rect->points[i] = -1.;
		}
	}

	return(rect);
}

double * rotate_line(double line[_NUM_ORDINATES]) {
	static double	r_line[_NUM_ORDINATES];

	double m_line[_NUM_ORDINATES];

	/* First, find the midpoint of the line */
	double	mid_x =	(line[_x1] + line[_x2]) / 2.;
	double	mid_y = (line[_y1] + line[_y2]) / 2.;

	/* Create the moved line */
	m_line[_x1] = line[_x1] - mid_x;
	m_line[_y1] = line[_y1] - mid_y;
	m_line[_x2] = line[_x2] - mid_x;
	m_line[_y2] = line[_y2] - mid_y;

	/* Rotate the points */
	r_line[_x1] = m_line[_x1] * _cos_t - m_line[_y1] * _sin_t;
	r_line[_y1] = m_line[_x1] * _sin_t + m_line[_y1] * _cos_t;
	r_line[_x2] = m_line[_x2] * _cos_t - m_line[_y2] * _sin_t;
	r_line[_y2] = m_line[_x2] * _sin_t + m_line[_y2] * _cos_t;

	/* Move the line back */
	r_line[_x1] += mid_x;
	r_line[_y1] += mid_y;
	r_line[_x2] += mid_x;
	r_line[_y2] += mid_y;

	return (r_line);
}

void transform_common(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., (GLdouble) _ww, 0., (GLdouble) _wh);
	glMatrixMode(GL_MODELVIEW);
}

void zero(void) {
	purge_Rects();
	_line = 0;
	_making_clipper = 0;
	_making_line = 0;
}

void myinit (void) {
/*	attributes of lines, background, etc */
	glClearColor(0., 0., 0., 1.);		/* black background */
	glColor3f(1., 1., 1.);			/* draw in white */
	glShadeModel(GL_FLAT);
/*	set up viewing 500 x 500 window with origin lower left */
	glViewport(0, 0, _ww, _wh);
	transform_common();

	_line_type = _normal;
	zero();

	/* Set up the rotation globals */
	_cos_t = cos(_rads);
	_sin_t = sin(_rads);
}
	
void keyboard(unsigned char key, int x, int y) {
	int	redisplay = 1;

	switch (key) {
		case	'q':
		case	'Q':	quit(0);

		case	'z':	zero();
				break;

		case	'r':	if (_line_type == _rotate) {
					_line_type = _normal;
				} else {
					_line_type = _rotate;
				}
				break;

		case	's':	if (_line_type == _starburst) {
					_line_type = _normal;
				} else {
					_line_type = _starburst;
				}
				break;

		case	'd':	_clipping_type = _unclipped;
				break;

		case	'c':	_clipping_type = _clipped;
				break;

		case	'b':	_clipping_type = _blanked;
				break;

		default:	redisplay = 0;
	}

	if (redisplay) {
		glutPostRedisplay();
	}
}

void double_swap(double *a, double *b) {
	double	tmp = *a;
	*a = *b;
	*b = tmp;
}

void mouse (int btn, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (btn == GLUT_LEFT_BUTTON) {
			if (!_making_line) {
				_lines[_line][0] = x;
				_lines[_line][1] = _wh - y;
				_making_line = 1;
			} else {
				_lines[_line][2] = x;
				_lines[_line][3] = _wh - y;
				_line++;
				_making_line = 0;

				glutPostRedisplay();
			}
		} else if (btn == GLUT_RIGHT_BUTTON) {
			Rect	*curr = NULL;

			if (!_making_clipper) {
				curr = create_Rect();

				curr->next = head;
				head = curr;
				head->points[_x1] = x;
				head->points[_y1] = _wh - y;
				_making_clipper = 1;
			} else {
				head->points[_x2] = x;
				head->points[_y2] = _wh - y;

				/* Change the clipping rectangle so that it's
					always of the form (x1, y1) < (x2, y2) */

				if (head->points[_x2] < head->points[_x1]) {
					double_swap(&(head->points[_x2]), &(head->points[_x1]));
				}

				if (head->points[_y2] < head->points[_y1]) {
					double_swap(&(head->points[_y2]), &(head->points[_y1]));
				}
					
				_making_clipper = 0;
				glutPostRedisplay();
			}
		}
	}
}

void display (void) {
	int	i;

	Rect	*curr = head;

	glClear(GL_COLOR_BUFFER_BIT);

	for (i = 0; i < _line; i++) {
		double	*line;
		int	j;

		if (_line_type == _starburst) {
			line = _lines[i];

			for (j = 0; j < (180 / _ROTATION); j++) {
				line = rotate_line(line);
				DrawFLine(line[_x1], line[_y1], line[_x2], line[_y2], _clipping_type);
			}
		} else {
			if (_line_type == _rotate) {
				line = rotate_line(_lines[i]);

				for (j = 0; j < _NUM_ORDINATES; j++) {
					_lines[i][j] = line[j];
				}
			} else {
				line = _lines[i];
			}
			
			DrawFLine(line[_x1], line[_y1], line[_x2], line[_y2], _clipping_type);
		}
	}

	glColor3fv(_colour[_green]);

	/* Check to see if we are in the middle of making Clipping
		rectangle */
	if (curr) {
		if (!_making_clipper) {
			glBegin(GL_LINE_LOOP);
				glVertex2d(curr->points[_x1], curr->points[_y1]);
				glVertex2d(curr->points[_x1], curr->points[_y2]);
				glVertex2d(curr->points[_x2], curr->points[_y2]);
				glVertex2d(curr->points[_x2], curr->points[_y1]);
			glEnd();
		}

		curr = curr->next;
	}

	while (curr) {
		glBegin(GL_LINE_LOOP);
			glVertex2d(curr->points[_x1], curr->points[_y1]);
			glVertex2d(curr->points[_x1], curr->points[_y2]);
			glVertex2d(curr->points[_x2], curr->points[_y2]);
			glVertex2d(curr->points[_x2], curr->points[_y1]);
		glEnd();

		curr = curr->next;
	}

	glFlush();
}

void reshape(int neww, int newh) {
	_wh = newh;
	_ww = neww;
	glViewport(0, 0, _ww, _wh);
	transform_common();
	display(); 
}
		
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(_ww, _wh);
	glutCreateWindow("demo");
	glutDisplayFunc(display);
	myinit();
	glutReshapeFunc(reshape); 
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
}
