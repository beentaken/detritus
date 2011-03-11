/* $Id: passwd.h 2 2007-07-19 13:00:48Z pdezwart $ */

#ifndef __PASSWD_H
#define __PASSWD_H

#define PASSWD_LEN      	13
#define PASSWD_CLEAR_LEN	8

char *
gen_passwd(char *plaintext);

int
check_passwd(char *plaintext, char *passwd);

#endif
