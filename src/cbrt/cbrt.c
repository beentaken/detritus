#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
  float *number;

  double exponent = 1.0 / 3.0;

  scanf("%f", number);
  printf("\nThe cube root of %f is %f\n", *number,
    pow(*number, exponent));

  return(0);
}
