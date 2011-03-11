
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "ass2.h"

char		*scratch = NULL;
struct bnode	*btree = NULL;
int		btree_comp_count = 0;
int		btree_depth = 0;
int		curr_depth = 0;

int
lexical_comparison(char *a, char *b) {
	int	rtcode;

	return((rtcode = strcasecmp(b, a)) ? rtcode : strcmp(b, a));
}

void
bnode_delete(struct bnode *node) {
	if (node) {
		if (node->data) {
			free(node->data);
		} else {
			fprintf(stderr,
				"bnode_delete(warning): node->data == NULL\n");
		}

		if (node->left || node->right) {
			/* Recursively delete the btree */
			bnode_delete(node->left);
			bnode_delete(node->right);
		}

		free(node);
	}
}

void
bnode_print(struct bnode *node) {
	if (node && node->data) {
		printf("%s ", node->data);
	}
}

void
btree_traverse(struct bnode *btree, void(*btfunc_ptr)(struct bnode *)) {
	struct bnode	*curr = btree;
	if (curr) {
		curr_depth++;
		btree_traverse(curr->left, btfunc_ptr);
		btfunc_ptr(curr);
		btree_traverse(curr->right, btfunc_ptr);
		curr_depth--;
	} else if (curr_depth > btree_depth) {
		btree_depth = curr_depth;
	}
}

int
btree_comp(struct bnode *a, struct bnode *b) {
	btree_comp_count++;
	return(lexical_comparison(a->data, b->data));
}

/* The root bnode is returned unless an error occoured, then a NULL is
	returned */

struct bnode *
btree_insert(struct bnode *root, struct bnode *leaf) {
	struct bnode	*curr = root;
	int		comparison = 0;

	do {
		if (!curr) {
			root = leaf;
		} else {
			comparison = btree_comp(curr, leaf);

			if (comparison > 0) {
				if (curr->right) {
					curr = curr->right;
				} else {
					curr->right = leaf;
					curr = NULL;
				}
			} else if (comparison < 0) {
				if (curr->left) {
					curr = curr->left;
				} else {
					curr->left = leaf;
					curr = NULL;
				}
			} else {
				/* Ok, undefined as to what to do here */
				fprintf(stderr,
					"btree_add: duplicate node found:\t%s\t(not added)\n", leaf->data);

				/* A leak occours if the node is already found.
					This cheap hack fixes that but it's not
					such a good idea.

					A better solution would be to notify
					the calling function. */
				bnode_delete(leaf);
				break;
			}
		}
	} while (curr);

	return(root);
}

struct bnode *
bnode_create(char *data) {
	struct bnode	*node = (struct bnode *)malloc(sizeof(struct bnode));

	if (node) {
		node->data = (char *)strdup(data);
		node->left = NULL;
		node->right = NULL;
	}

	return(node);
}
		
int
permute(char *stuff, char *temp, int len) {
	int	i;
	int	stuff_len = strlen(stuff);
	int	rtcode = 0;

	struct bnode	*node;

	if (len > 0) {
		for (i = 0; i < stuff_len; i++) {
			/* Check to see if the current character has been
				used */
			if (stuff[i] > 0) {
				/* It hasn't, let's use it */
				temp[0] = stuff[i];
				stuff[i] *= -1;

				/* Recurse for the rest of the string */
				rtcode += permute(stuff, temp + 1, len - 1);

				/* Set things back to the way they were */
				temp[0] = '\0';
				stuff[i] *= -1;
			}
		}

	} else {
		node = bnode_create(scratch);

		if (node) {
			bnode_print(node);
			if ((btree = btree_insert(btree, node)) == NULL) {
				rtcode = 1;
			}
		} else {
			perror("permute: ");
			rtcode = 1;
		}
	}

	return(rtcode);
}

int
main(int argc, char *argv[]) {
	int	rtcode = 0;
	int	len = 0;
	char	*stuff = NULL;

	stuff = (char *)calloc(getpagesize(), sizeof(char));

	scanf("%s %d", stuff, &len);

	scratch = (char *)calloc(len + 1, sizeof(char));

	if (scratch && stuff) {
		rtcode = permute(stuff, scratch, len);

		putchar('\n');
		btree_traverse(btree, bnode_print);
		printf("\nNumber of comparisons during btree creation:\t%d\n",
			btree_comp_count);
		printf("\nBinary tree depth:\t%d\n", btree_depth);
		
		free(stuff);
		free(scratch);
		bnode_delete(btree);
	} else {
		perror("main: ");
		rtcode = 1;
	}

	return(rtcode);
}
