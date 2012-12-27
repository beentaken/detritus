#include <stdio.h>

void squeeze(char *s, char *mask) {
  int i, j;

  for (i = j = 0; s[i] != '\0'; i++) {
    int count, test;

    test = 0;

    for (count = 0; mask[count] != '\0'; count++) {
      if (s[i] == mask[count]) {
        test = 1;
        break;
      }
    }

    if (!test) {
      s[j++] = s[i];
    }
  }

  s[j] = '\0';
}

int main(int argc, char *argv[]) {
  if (argc == 3) {
    printf("String:\t%s\tMask:\t%s\n", argv[1], argv[2]);
    squeeze(argv[1], argv[2]);
    printf("Result:\t%s\n", argv[1]);
  } else {
    return(1);
  }

  return(0);
}
