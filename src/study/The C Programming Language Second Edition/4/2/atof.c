#include <ctype.h>
#include <math.h>
#include "atof.h"

static int getsign(char s[], int *i) {
  int sign, index;

  index = *i;

  /* Get the sign */
  sign = (s[index] == '-') ? -1 : 1;
  if (s[index] == '+' || s[index] == '-') {
    *i += 1;
  }

  return(sign);
}

static double eexponent(char s[], int i) {
  int sign;
  double exponent;

  if (s[i] != '\0' && (s[i] == 'e' || s[i] == 'E')) {
    i++;
    sign = getsign(s, &i);

    for (exponent = 0.0; isdigit(s[i]); i++) {
      exponent = 10.0 * exponent + (s[i] - '0');
    }
    
    return(exp(exponent * sign));
  } else {
    return(1.0);
  }
}

double atof(char s[]) {
  double val, power;
  int i, sign;

  /* Skip the white space */
  for (i = 0; isspace(s[i]); i++) {
  }

  sign = getsign(s, &i);

  for (val = 0.0; isdigit(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
  }

  if (s[i] == '.') {
    i++;
  }

  for (power = 1.0; isdigit(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
    power *= 10.0;
  }

  return((sign * val / power) * eexponent(s, i));
}
