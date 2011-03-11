/* 
	Author:		Peter Nathaniel Theodore de Zwart
	Student ID:	9840642


	Originally by Peter Castle, ammendments made to allow the code to
	compile and so that it fits in with my coding style.
*/

#ifndef __Ctransform3_h_
#define	__Ctransform3_h_

void init_mat(double[4][4]);
void init();
void translate3d(double, double, double);
void rotatex(double), rotatey(double), rotatez(double);
void scale(double, double, double);
void mult(double[4][4]);
void get_mat(double[4][4]);
void set_mat(double[4][4]);
void transform2dv(double[4], double[4]);
void view(double[4], double[4], double[4], double);
void perspective(double);
void isometric();

#endif
