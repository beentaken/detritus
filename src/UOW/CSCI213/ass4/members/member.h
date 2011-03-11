/* $Id: member.h 2 2007-07-19 13:00:48Z pdezwart $ */

#ifndef __MEMBER_H
#define __MEMBER_H

#include "passwd.h"

#define NAME_LEN	32

typedef struct _Member {
	int	number;
	char	name[NAME_LEN + 1];
	char	surname[NAME_LEN + 1];
	char	gender[NAME_LEN + 1];
	char	email[NAME_LEN * 2 + 1];
	char	passwd[PASSWD_LEN + 1];
} Member;

int set_string(char *dest, char *src, int destlen);

int set_name(Member *member, char *name);

int set_surname(Member *member, char *surname);

int set_email(Member *member, char *email);

int set_passwd(Member *member, char *cleartext);

int set_gender(Member *member, char *gender);

void print_member(Member *member);

#endif
