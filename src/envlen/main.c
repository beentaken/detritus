
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
		
int main(int argc, char **argv) {
	char	*envvar = NULL;
		
	while (argc > 1) {
		envvar = getenv(argv[--argc]);

		if (envvar) {
			printf("%s\t%d\n", argv[argc], strlen(envvar));
		}
	}
					
	return(0);
}
