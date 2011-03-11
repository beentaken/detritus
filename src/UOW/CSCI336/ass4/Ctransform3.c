/* 
	Author:		Peter Nathaniel Theodore de Zwart
	Student ID:	9840642


	Originally by Peter Castle, ammendments made to allow the code to
	compile and so that it fits in with my coding style.
*/

#include <math.h>
#include "Ctransform3.h"

#define PIon180 .0174532925

static double t[4][4];

void init_mat(double a[4][4]) {
	int i, j;
	
	for (i = 0; i < 4; i++) {
		for (j=0; j < 4; j++) {
			a[i][j] = 0.;

			if (i == j) {
				a[i][i] = 1.;
			}
		}
	}
}

void init() {
	init_mat(t);
}

void translate3d(double tx, double ty, double tz) {
      double a[4][4];
      
      init_mat(a);
      a[0][3] = tx;
      a[1][3] = ty;
      a[2][3] = tz;
      mult(a);
}

void rotatex(double angl) {
      double a[4][4], s, c;

      s = sin(angl * PIon180);
      c = cos(angl * PIon180);
      init_mat(a);
      a[1][1] = c;
      a[2][2] = c;
      a[1][2] = -s;
      a[2][1] = s;
      mult(a);
}

void rotatey(double angl) {
      double a[4][4], s, c;

      s = sin(angl * PIon180);
      c = cos(angl * PIon180);
      init_mat(a);
      a[0][0] = c;
      a[2][2] = c;
      a[0][2] = s;
      a[2][0] = -s;
      mult(a);
}

void rotatez(double angl) {
      double a[4][4], s, c;

      s = sin(angl * PIon180);
      c = cos(angl * PIon180);
      init_mat(a);
      a[0][0] = c;
      a[1][1] = c;
      a[0][1] = -s;
      a[1][0] = s;
      mult(a);
}

void scale(double dx, double dy, double dz) {
	double a[4][4];
	
	init_mat(a);
	a[0][0] = dx;
	a[1][1] = dy;
	a[2][2] = dz;
	mult(a);
}

void mult(double a[4][4]) {
	double b[4][4];
	int i, j, k;
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			b[i][j] = 0.;

			for (k = 0; k < 4; k++) {
				b[i][j] += a[i][k] * t[k][j];
			}
		}
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			t[i][j] = b[i][j];
		}
	}
}

void get_mat(double a[4][4]) {
	int i, j;
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			a[i][j] = t[i][j];
		}
	}
}

void set_mat(double a[4][4]) {
	int i, j;
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			t[i][j] = a[i][j];
		}
	}
}

#ifdef __DEBUG
#include <stdio.h>

void showpoint(double p[4]) {
	fprintf(stderr, "(%f, %f, %f, %f)\n", p[0], p[1], p[2], p[3]);
}
#endif

void transform2dv(double x[4], double y[4]) {
	double z[4];
	int j;
	
	for (j = 0; j < 4; j++) {
		z[j] = t[j][0] * x[0] + t[j][1] * x[1] + t[j][2] * x[2]
			+ t[j][3] * x[3];
	}

	y[0] = z[0] / z[3];
	y[1] = z[1] / z[3];
	y[2] = z[2] / z[3];
	y[3] = 1.;

#ifdef __DEBUG
	showpoint(x);
	showpoint(t[0]);
	showpoint(t[1]);
	showpoint(t[2]);
	showpoint(t[3]);
	showpoint(y);
#endif
}

void view(double P1[4], double P2[4], double P3[4], double d) {
	double	M[4][4];
	double	P[4];
	double	D;

	translate3d(P1[0], P1[1], P1[2]);

	transform2dv(P2, P);

	D = sqrt(P[2] * P[2] + P[0] * P[0]);

	init_mat(M);

	M[0][0] = -P[2]/D;
	M[0][2] = P[0]/D;
	M[2][0] = -P[0]/D;
	M[2][2] = -P[2]/D;

	mult(M);

	D = sqrt(P[0] * P[0] + P[1] * P[1] + P[2] * P[2]);

	transform2dv(P2, P);

	init_mat(M);

	M[1][1] = -P[2]/D;
	M[1][2] = P[1]/D;
	M[2][1] = -P[1]/D;
	M[2][2] = -P[2]/D;

	mult(M);

	transform2dv(P3, P);

	D = sqrt(P[0] * P[0] + P[1] * P[1]);

	init_mat(M);

	M[0][0] = P[1]/D;
	M[0][1] = -P[0]/D;
	M[1][0] = P[0]/D;
	M[1][1] = P[1]/D;

	mult(M);

	translate3d(0, 0, d);
}

void perspective(double z) {
	double	foo[4][4];

	init_mat(foo);

	foo[2][2] = -1;

	mult(foo);

	init_mat(foo);

	foo[3][2] = -1/z;

	mult(foo);
}

void isometric() {
}
