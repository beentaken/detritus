#include <stdio.h>
#include <stdlib.h>

struct node {
  char data;
  struct node *next;
};

void printlist(struct node *list) {
  struct node *curr = list;

  while (curr) {
    putchar(curr->data);
    curr = curr->next;
  }

  putchar('\n');
}

struct node * gen_node(char data) {
  struct node *gen = malloc(sizeof(struct node));

  if (gen) {
    gen->data = data;
    gen->next = NULL;
  }

  return(gen);
}

struct node * remove_node(struct node *list, int index) {
  struct node *curr = list;
  struct node *prev = NULL;

  int i;

  for (i = 0; i < index && curr; i++) {
    prev = curr;
    curr = curr->next;
  }

  if (curr) {
    if (curr->next) {
      if (prev) {
        prev->next = curr->next;
      }

      curr->next = NULL;
    }
  }

  return(curr);
}

void insert_node(struct node *list, struct node *insert, int index) {
  struct node *curr = list;
  struct node *prev = NULL;

  int i;

  for (i = 0; i < index && curr; i++) {
    prev = curr;
    curr = curr->next;
  }

  if (curr) {
    if (curr->next) {
      insert->next = curr->next;
    }
  }

  if (prev) {
    prev->next = insert;
  }
}

void permute(struct node *char_list, int len) {
  static struct node *string = NULL;
  struct node *curr;

  int i;

  if (len > 0) {
    for (i = 0; i < len; i++) {
      if (string == NULL) {
        curr = string = remove_node(char_list, i);
      } else {
        curr = string->next;

        while (curr) {
            curr = curr->next;
        }

        curr = remove_node(char_list, i);
      }

      permute(string, len - 1);

      insert_node(char_list, curr, i);
    }
  } else {
    printlist(string);
  }
}

int main(int argc, char *argv[]) {
  int rtcode = 0;
  int len = 5;
  int i;

  struct node *head;
  struct node *curr;

  head = gen_node(65);
  curr = head;

  for (i = 1; i < len; i++) {
    curr->next = gen_node(65 + i);
    curr = curr->next;
  }

  permute(head, len);

  return(rtcode);
}
