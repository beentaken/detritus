#include "list.h"
#include <stdlib.h>
#include <stdio.h>

static Node *
nukeNode(Node *node) {
        Node    *next = node->next;

        free(node);

        return(next);
}

static Node *
findNode(char letter, Node *head) {
    while (head) {
        if (letter == head->letter) {
            break;
        } else {
            head = head->next;
        }
    }

    return(head);
}

static Node *
genNode(char letter) {
        Node    *node = malloc(sizeof(Node));

        if(node) {
                node->next = NULL;
                node->letter = letter;
        node->count = 1;
        } else {
                perror("genNode ");
                node = NULL;
        }

        return(node);
}

static Node *
addNode(char letter, Node *head) {
    Node    *temp = genNode(letter);

    temp->next = head;
    head = temp;

    return(head);
}

Node *
addLetter(char letter, Node *head) {
        Node    *node = findNode(letter, head);

        if(node) {
        node->count++;
        } else {
        head = addNode(letter, head);
        }
    
    return(head);
}

void
pruneNodes(Node *head) {
        while (head) {
                head = nukeNode(head);
        }
}

int
listLength(Node *head) {
    int count = 0;

    while (head) {
        count++;
        head = head->next;
    }

    return(count);
}
