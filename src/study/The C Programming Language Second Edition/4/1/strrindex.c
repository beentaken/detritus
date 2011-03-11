/* $Id: strrindex.c 2 2007-07-19 13:00:48Z pdezwart $ */

int strrindex(char s[], char t[]) {
	int	i, j, k, index;

	/* Set a default value for index */
	index = -1;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++) {
		}

		if (k > 0 && t[k] == '\0')
			index = i;
	}

	return(i);
}
