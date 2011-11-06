#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

struct _Node {
    char    *data;
    struct _Node    *next;
} typedef Node;

Node * genNode(char *data) {
    Node    *node = malloc(sizeof(Node));

    if(node) {
        node->next = NULL;
        node->data = strdup(data);
    } else {
        perror("genNode ");
        node = NULL;
    }

    return(node);
}

Node * nukeNode(Node *node) {
    Node    *next = node->next;

    free(node->data);
    free(node);

    return(next);
}

void pruneNodes(Node *head) {
    while (head) {
        head = nukeNode(head);
    }
}

int main(int argc, char **argv) {
    const int   buff_len = 4096;
    int count = 0;

    char    excuses_file[] = ".bofh.txt";
    char    *buffer = calloc(buff_len, sizeof(char));

    FILE    *fh = NULL;

    struct timeb    time;

    unsigned int finetime;

    Node    *head = NULL; 
    Node    *node = NULL;

    /* Get the time */
    ftime(&time);

    finetime = time.time * 1000 + time.millitm;

    srand(finetime);

    if (!buffer) {
        perror("main ");
        return(1);
    }

    strcpy(buffer, getenv("HOME"));
    strcat(buffer, "/");
    strcat(buffer, excuses_file);

    fh = fopen(buffer, "r");

    if (!fh) {
        perror(buffer);
        return(2);
    }

    count = strlen(buffer);

    while (count) {
        buffer[count--] = '\0';
    }

    while(fgets(buffer, buff_len, fh)) {
        count++;
        node = head;

        head = genNode(buffer);
        head->next = node;
    }

    count = rand() % count;

    node = head;
    while(count) {
        node = node->next;
        count--;
    }

    printf("%s", node->data);

    pruneNodes(head);
    fclose(fh);
    free(buffer);
    return(0);
}
