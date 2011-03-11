\def\RCS$#1:#2${\expandafter\def\csname RCS#1\endcsname{#2}}

\font\tf=cmr20
\font\ti=cmti20
\font\tb=cmb20

\centerline{\tf CSCI337}
\vskip 24 pt
\centerline{\ti ``Organisation of programming languages''}
\vskip 24 pt
\centerline{\tf Assignment 2}
\vskip 24 pt
\centerline{\tf Implimentation of {\ti ``animal''} in {\tb CWEB}}
\vskip 24 pt
\centerline{Pete de Zwart, 9840642}
\vskip 12 pt
\centerline{\sevenrm \RCSId}

@* Introduction. In CSCI337: Organisation of Programming languages, we have
been challenged to implement an old computer game called ``animal'' in a
variety of programming language styles. This attempt will be completed using
{\bf CWEB} or literate {\tt C}.

@ ``animal'' is a game where the computer attempts to guess what animal a
player is thinking about. It does this by keeping track of previously learned
animals and what questions can be used to differentiate between them. In
essence it has a binary decision tree where the branches are questions and the
leaves are animals. As each branch splits in two directions, one is
traditionally the {\it yes} branch and the other is the {\it no} branch.

The game is not intelligent as it can not make inferences about certain classes
of animals, for example, it can not differentiate all animals that can fly from
those that can not. It can only tell the difference between the current two
branches of choice. There may be two sub-trees which have animals of the same
class but which it has learned through two different lines of questioning.

Therefore a very simple approach to how the animal data is stored can be used
to reduce the complexity of the program. Both questions and animals can be
stored using the same data structure. Animals can be differentiated between
questions by the fact that their pointers to the sub-branches are NULL.

@<Animal structure@>=
typedef struct _animal {
	struct _animal	*y;
	struct _animal	*n;
	char	*s;
} animal;

@* Game flow. The game flows in the following form, first the player is asked
if they want to load a data set from a previous invocation of the game.
Otherwise a default data set is constructed in memory. Then the game progresses
in to the playing state, essentially a loop wherein the game proceeds to
interrogate the player to determine the animal they are thinking of.

After each session of interrogation, the player is asked if they wish to play
again, if not the game exits the loop. Once out of the loop of play, the player
is asked if they wish to save the data from the current session to a file, to
be used in a fashion previously elucidated.

@c
#include <stdio.h>
#include <stdlib.h>

@<Animal...@>@;

@<Global data@>@;

@<Function declarations@>@;

int main(void) {
	register int	i;
	@<Data of main@>@;

	@<Load data set@>@;

	for (;;) {
		@<Interrogate player@>@;

		printf("Thank you. Play again? ");

		if (!(@<Was that a yes?@>))
			break;
	}

	@<Save data set@>@;

	@<Clean up dynamic memory@>@;
	return 0;
}

@ Player interrogation is a simple loop where the player is asked a question for
each iteration. If the |current| |@<Animal structure@>| is a branch, then ask
the player the question contained therein. If the answer to the query is a yes
then move down the yes branch and conversely if the answer is a no, move down
the no branch. Otherwise, we must be at a leaf node so |@<Enquire about the
player's animal@>| and terminate the interrogation loop.

@<Interrogate...@>=
current = root;

for (;;) {
	if (current->y) {
		printf("%s ", current->s);

		if (@<Was...@>) {
			@<Descend down yes branch@>@;
		}@+ else {
			@<Descend down no branch@>@;
		}
	}@+ else {
		@<Enquire...@>@;
		break;
	}
}

@ Now that we are using |current| and |root| pointers for the binary decision
tree, they need to be insertEd in to the |@<Data...@>|.

@<Data...@>=
animal	*current = NULL;
animal	*root = NULL;

@* Animal Inquiry. Now that the flow of the game has been established, the
method of getting the information from the player about the animal they are
thinking about needs to be established.

We know that once we are at a leaf node in the decision tree that we have an
animal that we can ask the player about. If the player is not crafty enough to
have thought of an original animal from the perspective of the game, then we
have won. Otherwise, we need to get the new animal from the player and an
appropriate question to differentiate between the new animal and the current
animal. Then the question needs to be grafted on the tree and references to the
two animals placed accordingly to their elicited response.

@<Enquire...@>=
printf("Is your animal ");
@<Print article for the current animal@>@;
printf("%s? ", current->s);

if (@<Was...@>) {
	printf("Excellent, I win.\n");
}@+ else {
	@<Obtain animal information from the player@>@;
	@<Graft on new branch@>@;
}

@ Here we have to get the new animal data, the differentiation question and for
the new animal and what the answer to that question is for the new animal.

Firstly lets allocate some buffer space for grabbing input from the player.

@d
buff_len 4096

@<Global...@>+=
char	buff[buff_len];

@ We will also need another |@<Animal...@>| pointer for the new animal and the
associated question.

@<Data...@>+=
animal	*newa = NULL;
animal	*question = NULL;

@ Then grab the relevant information from the player.

@<Obtain...@>=
printf("What is your animal? ");
@<Grab a line from the player@>;
@<Convert buffer to lowercase@>@;
@<Extricate animal from the buffer@>@;
printf("What question would distinguish ");
@<Print...@>@;
printf("%s from ", current->s);
@<Swap new and current animal@>@;
@<Print...@>@;
printf("%s?\n", current->s);
@<Swap...@>@;
@<Grab...@>;
@<Convert...@>@;
@<Turn the buffer in to a question@>@;

@* Animal extrication. The delicate job of converting a buffer of characters
in to a newly formed animal requires the pruning of a possible prepended
article, which may seem simple, but what if the player types ``anteater''? If
we used a simple method of removing any word that begun with an 'a' or a 't'
then what would happen to the precious ``antelope'' or the charismatic
``troglodyte''?

Instead we need to have a state machine that will chomp through the buffer and
duplicate the required substring. We also need to create an |@<Animal...@>| on
the heap to store this information.

@<Extricate...@>=
newa = @<Create an |animal|@>@;
i = 0;
if (buff[0] == 'a' || buff[0] == 't') { 
	for (i = 1; i < buff_len; i++) {
		if (buff[i] == '\0') {
			/* We have found the end of the string, so no article */
			i = 0;
			break;
		}@+ else if (buff[i] == ' ') {
			/* OK, found a substring, assume the rest is good */
			i++;
			break;
		}
	}

	if (i == buff_len) {
		i = 0;
	}
}
newa->s = strdup(buff + i);

@ While we have conveniently used strdup with reckless abandon, it would be
decent to let the code be aware of our indiscretions.

@<Global...@>+=
#include <string.h>

@* Riddle me this. How do we convert a buffer in to a question? I'll show you.
First we |@<Create...@>|, then convert the first letter to uppercase, append
a question mark if necessary, ask the Player what the answer to the question
is for the new animal is then appropriately assign the leaf nodes to |->yes|
and |->no|.

@<Turn...@>=
question = @<Create...@>@;
buff[0] = toupper(buff[0]);
i = strlen(buff);
if (buff[i - 1] != '?') {
	buff[i] = '?';
	buff[i + 1] = '\0';
}
question->s = strdup(buff);
printf("For ");
@<Swap...@>@;
@<Print...@>@;
@<Swap...@>@;
printf("%s the answer would be? ", newa->s);
if (@<Was...@>) {
	question->y = newa;
	question->n = current;
}@+ else {
	question->y = current;
	question->n = newa;
}

@* The art of grafting. Here we need to graft the new question branch on to the
binary decision tree. Thus we need to keep track of where we are grafting the
new branch so that it gets placed in the correct position.

@<Data...@>+=
animal	**graft_point = NULL;

@ Descending the tree. Here we need to do some tracking of where we are to
handle insertions of data. Essentially, descending in either direction is the
same.

@ Descending the ``yes'' branch.

@<Descend down yes...@>=
graft_point = &(current->y);
current = current->y;

@ Descending the ``no'' branch.

@<Descend down no...@>=
graft_point = &(current->n);
current = current->n;

@ Now for the graft.

@<Graft...@>=
*graft_point = question;

@* Loading and saving the decision tree. Serialisation is not an option here as
the binary decision tree is jury rigged together using pointers which will loose
their contextual meaning if written out to a file. Thus an alternate method of
external storage needs to be devised. The data that needs to be saved are the
component strings |animal->s| of each |@<Animal...@>|.

@ Before being able to save and load, we need to find out the file name. As
there is already a buffer, we can use that to store the file name. Prompt the
player and grab a line, simple.

@<Find out the filename@>=
printf("Enter saved game file (blank for no file): ");
@<Grab...@>;

@ Let's deal with loading first, in which we need to get the name of a data file
from the user, if they just enter a NULL string, a default decision tree needs
to be generated, otherwise load in the data from the file. Also, the initial
graft point for questions needs to be set to the root of the tree.

@<Load...@>=
@<Find...@>@;
if (buff[0] != '\0') {
	@<Read in data@>@;
}@+ else {@/
	root = @<Create...@>@;
	root->s = strdup("horse");@/
}

graft_point = &root;

@ Saving is a bit simpler, we only need to write out the tree if we are given a
file name, otherwise we can simply do nothing.

@<Save...@>=
@<Find...@>@;
if (buff[0] != '\0') {
	@<Write out data@>@;
}

@ I'm going to be lazy an just implement recursive methods for dealing with
the tree. The recurse functions take a function as one of the arguments so we
can specify what to do when we do recurse.

@<Function...@>=
void inorder(animal *a, void (*f)(animal *)) {
	if (a) {
		inorder(a->y, f);
		f(a);
		inorder(a->n, f);
	}
}

void preorder(animal *a, void (*f)(animal *)) {
	if (a) {
		f(a);
		preorder(a->y, f);
		preorder(a->n, f);
	}
}

void postorder(animal *a, void (*f)(animal *)) {
	if (a) {
		postorder(a->y, f);
		postorder(a->n, f);
		f(a);
	}
}

@ A print function for the recursion would help with saving the data. The number
of animals printed needs to be retained.

@<Function...@>+=
void print_animal(animal *a) {
	fprintf(file, "%s\n", a->s);
	if (a->s[0] >= 'a' && a->s[0] <= 'z') {
		num_animals++;
	}
}

@ Let there be a |num_animals| variable to store the number of animals saved or
loaded from the data file. It will need to be global as there are multiple
functions that need to access it.

@<Global...@>+=
int num_animals;

@ Now we need some sort of global |FILE *| to be used in |print_animal|.

@<Global...@>+=
FILE *file;

@ Writing out of data can now be implemented. Open the file, if unsuccessful,
barf profusely and terminate otherwise do an preorder print of the binary
decision tree, then print out the number of animals that was saved to the file.

@<Write...@>=
file = fopen(buff, "w");

@<Check for fopen error@>@;
num_animals = 0;

preorder(root, print_animal);

printf("(%d Animals saved)\n", num_animals);

@ Reading of the data will have to be done in a different fashion to saving as
there is no tree to recurse. How about a recursive reading function. Scan a
line from the data file, if the line is a question (the first character is
uppercase), then read in the subtrees. Otherwise, read in an |animal| and
increment the animal count.

@<Function...@>+=
animal * rec_read(void) {
	animal	*a = @<Create...@>@;

	fgets(buff, buff_len, file);

	buff[strlen(buff) - 1] = '\0';

	a->s = strdup(buff);

	if (buff[0] >= 'A' && buff[0] <= 'Z') {
		a->y = rec_read();
		a->n = rec_read();
	}@+ else {
		num_animals++;
	}

	return(a);
}

@ We should be able to read in a data file now.

@<Read...@>=
file = fopen(buff, "r");
@<Check...@>@;
num_animals = 0;
root = rec_read();
printf("(%d Animals loaded)\n", num_animals);

@* Miscellaneous code. Here are the sections that describe the various
nefarious code snippets reffered to earlier on.

@ Output a message if the game was unable to open the data file.

@<Check...@>=
if (!file) {
	perror(buff);
	exit(1);
}

@ Grab a line from the user and whack it in to the buffer. Caveat, as this is
used as part of some conditional statement trickery, the trailing statement
terminator has been omitted. Also, |gets()| is being used, so any player
entering more than |buff_len| characters at one time will be hunted down, drawn,
quartered, fed to crocodiles, tarred, fed to crocodiles again, feathered,
then forced to listen to the Hamster Dance for the rest of their existence.
See http://www.hampsterdance.com/wav/gotit1c.wav

@<Grab...@>=
gets(buff)

@ Converting the buffer to lowercase. A simple for loop using the |tolower()|
function which is defined in |ctype.h|.

@<Convert...@>=
for (i = 0; i < buff_len && buff[i] != '\0'; i++) {
	buff[i] = tolower(buff[i]);
}

@ Our global data needs to be updated as we are using definitions found in
|ctype.h|.

@<Global...@>+=
#include <ctype.h>

@ Swapping the new and current |@<Animal...@>| pointers, don't try this at home.

@<Swap...@>=
{
	animal	*temp = newa;
	newa = current;
	current = temp;
}

@ Getting a yes or no answer from the user requires some jiggery pokery.
This is a very nasty piece of work. Note, if the first character is not a |'y'|
or a |'Y'|, we assume the negative. If the player enters ``You must be
joking.'', we believe they meant yes, even though the semantics of their
statement means otherwise.

@<Was...@>=
@<Grab...@> && (tolower(buff[0]) == 'y')

@ One requirement of the assignment is that appropriate articles are printed
for the animals. So any animal that has a vowel for the first letter needs to
be prepended with ``an '' or ``a '' if it begins with a consonant. We are not
correctly dealing with animals that being with a silent ``h'', as they
are supposed to be prepended by the indefinite article ``an'' rather than ``a''.

@<Print...@>=
putchar('a');
switch (current->s[0]) {
	case	'a':@;
	case	'e':@;
	case	'i':@;
	case	'o':@;
	case	'u':	putchar('n');
}
putchar(' ');

@ In all the excitement I forgot to give the game the miracle of life.

@<Create...@>=
(animal *) calloc(1, sizeof(animal));

@ Cleaning up after the animals. Just like real animal, these leave a mess.
Do an postorder traversal of the tree and free up all the dynamic memory.

@<Clean...@>=
postorder(root, (void (*)(animal *))free);

@* Index.
