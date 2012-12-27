#include <stdio.h>

unsigned long factorial(unsigned long n) {
  unsigned long total = 1;

  while (n) {
    total *= n--;
  }

  return total;
}

int main(int argc, char **argv) {
  unsigned long num_choices = 0;

  scanf("%lu", &num_choices);

  printf("%lu\n",factorial(num_choices));

  return 0;
}
