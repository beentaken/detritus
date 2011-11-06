#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int *fds = 0x0;
    int *fd = 0x0;
    int *end = 0x0;

    int splits = 0;

    int rtcode = 0;

    int     i = 0;

    char    buff[1024] = { '\0' };

    if (argc == 2) {
        splits = atoi(argv[1]);

        fds = (int *)calloc(splits, sizeof(int));
        end = fds + splits - 1;

        if (fds) {
            for (i = 0; i < splits; i++) {
                sprintf(buff, "%d", i);
                fds[i] = open(buff, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

                if (fds[i] == -1) {
                    perror("main:open");
                    rtcode = 3;
                    
                    goto end;
                }
            }

            fd = fds;
            while ((rtcode = read(0, buff, 1)) == 1) {
                if (isalpha((int)buff[0])) {
                    if(write(*fd, buff, 1) == 1) {
                        if (fd == end) {
                            fd = fds;
                        } else {
                            fd++;
                        }
                    } else {
                        perror("main:write");
                        rtcode = 6;
                        goto end;
                    }
                }
            }

            if (rtcode != 0) {
                perror("main:read");
                rtcode = 5;
            }
            
            fd = fds;
            while (fd != end) {
                if (close(*fd)) {
                    perror("main:close");
                    rtcode = 4;
                } else {
                    fd++;
                }
            }

            free(fds);
        } else {
            perror("main:calloc");
            rtcode = 2;
        }
    } else {
        fprintf(stderr, "You must specify the number of splits.\n");
        rtcode = 1;
    }

end:
    return(rtcode);
}
