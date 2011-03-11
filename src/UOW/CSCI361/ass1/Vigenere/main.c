
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define ALPHAS	26

void munge(char *string) {
	char *p = string;

	while (*p != '\0') {
		if (isalpha(*p)) {
			if (string != p) {
				*string = *p;
			}

			string++;
		}

		p++;
	}

	while (*string != '\0') {
		*string++ = '\0';
	}
}

double IC(int *freq) {
	int	i = 0;
	int	count = 0;

	int	cumulative = 0;

	while (i < ALPHAS) {
		count += freq[i];
		i++;
	}

	for (i = 0; i < ALPHAS; i++) {
		cumulative += freq[i] * (freq[i] - 1);
	}

	return ((double)cumulative / (double)(count * (count - 1)));
}

void deleteFrequencies(int **freqs, int period) {
	int i;

	for (i = 0; i < period; i++) {
		if (freqs[i]) {
			free(freqs[i]);
		}
	}

	free(freqs);
}

int ** periodicFrequencies(char *text, int period) {
	int	**freqs = NULL;

	int	i;

	freqs = (int **)calloc(period, sizeof(int *));

	if (freqs) {
		for (i = 0; i < period; i++) {
			freqs[i] = (int *)calloc(ALPHAS, sizeof(int));

			if (!freqs[i]) {
				perror("AverageIC:calloc");
				deleteFrequencies(freqs, period);
				return(NULL);
			}
		}

		i = 0;

		while (*text != '\0') {
			freqs[i][tolower(*text) - 'a']++;

			if (++i == period) {
				i = 0;
			}

			text++;
		}
	} else {
		perror("AverageIC:calloc");
	}

	return(freqs);
}

double AverageIC(char *text, int period) {
	int	**freqs = NULL;
	int	i;

	double	*ICs = NULL;
	double	aveIC = 0.0;

	freqs = periodicFrequencies(text, period);

	if (freqs) {

		ICs = (double *)calloc(period, sizeof(double));

		if (ICs) {
			for (i = 0; i < period; i++) {
				ICs[i] = IC(freqs[i]);
			}
		} else {
			perror("AverageIC:calloc");
		}

		for (i = 0; i < period; i++) {
			aveIC += ICs[i];
		}

		aveIC /= period;

		deleteFrequencies(freqs, period);
	}

	return (aveIC);
}

int findPeriod(char *ctext) {
	int	i = 0, bestperiod = -1;

	static const double englishIC = 0.065;

	double	diff = englishIC * 0.1, bestdiff = diff, currIC = 0.0;


	for(i = 1; i != (bestperiod * 2 + 1); i++) {
		currIC = AverageIC(ctext, i);

		diff = fabs(englishIC - currIC);

		if (diff < bestdiff) {
			bestdiff = diff;
			bestperiod = i;
		}
	}

	return(bestperiod);
}

char * findKey(char *ctext) {
	int	period = 0;

	int	i, j, highFreq;

	int	**freqs = NULL;

	char	*key = NULL;

	period = findPeriod(ctext);

	key = (char *)calloc(period + 1, sizeof(char));

	if (key) {
		freqs = periodicFrequencies(ctext, period);

		if (freqs) {
			for (i = 0; i < period; i++) {
				for (j = 1, highFreq = 0; j < ALPHAS; j++) {
					if (freqs[i][j] > freqs[i][highFreq]) {
						highFreq = j;
					}
				}

				key[i] = 'a' + (ALPHAS + highFreq - 4) % ALPHAS;
			}

			deleteFrequencies(freqs, period);
		}
	} else {
		perror("findKey:calloc");
	}

	return(key);
}

void deCipher(char *ctext, char *key) {
	int	i, j;

	for (i = 0, j = 0; ctext[i] != '\0'; i++) {
		if (isalpha(ctext[i])) {
			ctext[i] = ((ALPHAS + (ctext[i]) - (key[j])) % ALPHAS) + 'a';

			if (key[++j] == '\0') {
				j = 0;
			}
		}
	}
}
	
int main(int argc, char **argv) {
	int	rtcode = 0;

	int	fd = 0;

	int	length = 0;

	char	*ctext = NULL;
	char	*key = NULL;

	if (argc == 2) {
		fd = open(*++argv, O_RDONLY);

		if (fd != -1) {
			length = lseek(fd, 0, SEEK_END);

			if (length != (SEEK_END - 1)) {
				ctext = calloc(length, sizeof(char));

				if (ctext) {
					if (lseek(fd, 0, SEEK_SET) == 0) {
						if (read(fd, ctext, length) == length) {
							munge(ctext);

							key = findKey(ctext);

							printf("Key = %s\n", key);
						} else {
							perror(*argv);
							rtcode = 6;
						}
					} else {
						perror(*argv);
						rtcode = 5;
					}

					free(ctext);
				} else {
					perror("main:calloc");
					rtcode = 4;
				}
			} else {
				perror(*argv);
				rtcode = 3;
			}
				
			if (close(fd)) {
				perror(*argv);
				rtcode = 2;
			} else {
				int keylen = strlen(key);

				while(1) {
					fd = open(*argv, O_RDONLY);

					if (fd != -1) {
						if (read(fd, ctext, length) == length) {
							deCipher(ctext, key);

							printf("%s", ctext);
							
							if (close(fd)) {
								perror(*argv);
								break;
							}

							printf("Enter fudged key:\n");

							if (read(STDIN_FILENO, key, keylen) != keylen) {
								break;
							}
						} else {
							perror(*argv);
							rtcode = 7;
							break;
						}
					} else {
						perror(*argv);
						break;
					}
				}

				free(key);
			}
		} else {
			perror(*argv);
			rtcode = 1;
		}
	}

	return(rtcode);
}
