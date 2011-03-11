/* $Id: member.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <string.h>
#include <stdio.h>

#include "member.h"

/*
	Set the targeted string.

	Returns zero on success.

	Returns a negative value depending on the error:

	-1	Source string is longer than the required length.
*/

int set_string(char *dest, char *src, int destlen) {
	int rtcode = 0;

	/* Sanity check */
	if (strlen(src) > destlen) {
		fprintf(stderr, "Length of src is longer than %d\n", destlen);
		rtcode = -1;
	} else {
		strcpy(dest, src);
	}

	return(rtcode);
}

int set_name(Member *member, char *name) {
	return(set_string(member->name, name, NAME_LEN));
}

int set_surname(Member *member, char *surname) {
	return(set_string(member->surname, surname, NAME_LEN));
}

int set_email(Member *member, char *email) {
	return(set_string(member->email, email, NAME_LEN * 2));
}

/*
	In addition to the set_string error codes:

	-2	cleartext string longer than PASSWD_CLEAR_LEN
*/

int set_passwd(Member *member, char *cleartext) {
	int	rtcode = 0;

	if (strlen(cleartext) <= PASSWD_CLEAR_LEN) {
		rtcode = set_string(member->passwd, gen_passwd(cleartext),
			PASSWD_LEN);
	} else {
		fprintf(stderr, "Cleartext password longer than %d characters",
			PASSWD_CLEAR_LEN);

		rtcode = -2;
	}

	return(rtcode);
}

/*
	To avoid problems with the rare case that someones gender will not be
	Male or Female. Any gender can be specified. However, the front end of
	the CGI will have a list of radio buttons for the gender.
*/

int set_gender(Member *member, char *gender) {
	return(set_string(member->gender, gender, NAME_LEN));
}

void print_member(Member *member) {
	printf("number:\t\t%d\n", member->number);
	printf("name:\t\t%s\n", member->name);
	printf("surname:\t%s\n", member->surname);
	printf("gender:\t\t%s\n", member->gender);
	printf("email:\t\t%s\n", member->email);
}
