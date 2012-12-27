#include <stdio.h>
#include <stdlib.h>

#define MAXLEN  256

char lower(char x) {
  return((x >= 'A' && x <= 'Z') ? x + ' ' : x);
}

void clean_string(char *string) {
  unsigned short i;

  for (i = 0; i < MAXLEN; i++) {
    string[i] = '\0';
  }
}
    
void escape(char *s, char *t) {
  unsigned short i, j;

  i = 0;
  j = 0;

  while (i < MAXLEN && j < MAXLEN && s[i] != '\0') {
    switch (s[i]) {
      case '\\':  t[j++] = '\\';
                  t[j] = '\\';
                  break;

      case '\t':  t[j++] = '\\';
                  t[j] = 't';
                  break;

      case '\n':  t[j++] = '\\';
                  t[j] = 'n';
                  break;

      default:    t[j] = s[i];
                  break;
    }

    i++;
    j++;
  }
}

void unescape(char *s, char *t) {
  unsigned short  i, j;

  i = 0;
  j = 0;

  while (i < MAXLEN && s[i] != '\0') {
    switch (s[i]) {
      case '\\':  switch(s[++i]) {
                      case  't':  t[j] = '\t';
                                  break;

                      case 'n':   t[j] = '\n';
                                  break;

                      default:    t[j] = s[i];
                      break;
                  }
                  break;

      default:    t[j] = s[i];
                  break;
    }

    i++;
    j++;
  }
}

int main(int argc, char *argv[]) {
  char  escaped[MAXLEN], unescaped[MAXLEN];

  if (argc == 2) {
    clean_string(escaped);
    clean_string(unescaped);

    escape(argv[1], escaped);
    unescape(escaped, unescaped);

    printf("original:\t%s\nescaped:\t%s\nunescaped:\t%s\n", argv[1], escaped, unescaped);
  } else {
    return(1);
  }

  return(0);
}
