
#include <unistd.h>
#include <stdlib.h>

#include "member.h"
#include "fileIO.h"

#define BUFFSIZE	256

enum Error_codes {
	Success,
	Undefined_key,
	Illegal_value,
	Undefined_action,
	Buffer_overflow,
	Failed_authentication,
	IO_Error,
	Exit
};

int getDataPair(char *key, char *value) {
	char	*buffer = key;

	char	temp;

	int	i = 0;

	int	rtcode = 1;

	while (rtcode) {
		rtcode = read(0, &temp, 1);

		if (rtcode == 0) {
			temp = -1;
		}

		switch (temp) {

			case '=':	buffer = value;
					key[i] = '\0';
					i = 0;
					break;

			case '&':	

			case -1:	rtcode = 0;
					value[i] = '\0';
					break;
					
			case '\n':	break;


			default:	if (i < BUFFSIZE) {
						buffer[i++] = temp;
					} else {
						rtcode = Buffer_overflow;
					}
					break;
		}
	}

	return(rtcode);
}

int strstrs(char *needle, char **haystack) {
	int	i = 0;
	int	rtcode = -1;

	while(haystack[i][0] != '\0' && rtcode == -1) {
		if (strcmp(needle, haystack[i]) == 0) {
			rtcode = i;
		} else {
			i++;
		}
	}

	return (rtcode);
}

static char term[] =	"";

void delete_member(Member *member) {
	int	number = member->number;

	member->number = -1;

	put(member, number);
}

/*
	The key/value pair with the member number MUST arrive before the passwd pair.
*/

int handle_auth_access(char *key, char *value, void (*action)(Member *)) {
	char	display_passwd[] =	"passwd",
		display_number[] =	"number";

	char	*display_keys[] = {
			display_passwd,
			display_number,
			term
		};

	enum	display_enum {
			passwd,
			number
		};

	int		rtcode;

	static Member	member[1];

	static int	num = -1;

	switch (strstrs(key, display_keys)) {
		case passwd:	rtcode = check_passwd(value, member->passwd);
					
				if (rtcode) {
					rtcode = Failed_authentication;
				} else {
					(*action)(member);
					rtcode = Exit;
				}	

				break;

		case number:	num = atoi(value);

				if (num < 0) {
					rtcode = Illegal_value;
				} else {
					rtcode = get(member, num);
					
					if (rtcode) {
						rtcode = IO_Error;
					}
				}

				break;

		default:	rtcode = Undefined_key;
				break;
	}

	return(rtcode);
}

int processPair(char *key, char *value) {
	char	action_parsing[] =	"parsing",
		action_add[] =		"add",
		action_delete[] =	"delete",
		action_modify[] =	"modify",
		action_display[] =	"display",

		add_name[] =		"name",
		add_surname[] = 	"surname",
		add_gender[] =		"gender",
		add_email[] = 		"email",
		add_passwd[] = 		"passwd";


	char	*action_keys[] = {
			action_parsing,
			action_add,
			action_delete,
			action_modify,
			action_display,
			term
		};

	char	*add_keys[] = {
			add_name,
			add_surname,
			add_gender,
			add_email,
			add_passwd,
			term
		};

	enum		action_enum {
				parsing,
				add,
				delete,
				modify,
				display
			};

	enum		add_enum {
				name,
				surname,
				gender,
				email,
				passwd
			};

	int		value_table[] = { 0, 5, 2, 6, 2 };

	static int	num_values_needed = 0;
	static int	num_values = 0;

	static int	current_action = parsing;
	int		rtcode = 0;

	static Member	member[1];

	switch (current_action) {
		case parsing:	if (strcmp(key, "action") == 0) {
					current_action = strstrs(value, action_keys);

					if (current_action == -1) {
						rtcode = Undefined_action;
					} else {
						num_values_needed =
							value_table[current_action];
					}
				} else {
					rtcode = Undefined_key;
				}

				break;

		case add:	switch (strstrs(key, add_keys)) {
					case name:	rtcode = set_name(member, value);
							break;

					case surname:	rtcode = set_surname(member, value);
							break;

					case gender:	rtcode = set_gender(member, value);
							break;

					case email:	rtcode = set_email(member, value);
							break;

					case passwd:	rtcode = set_passwd(member, value);
							break;

					default:	rtcode = Undefined_key;
							break;
				}

				if (rtcode == 0) {
					if (num_values < num_values_needed - 1) {
						num_values++;
					} else {
						rtcode = allocate_number();
						member->number = rtcode; 

						if (rtcode >= 0) {
							rtcode = put(member, rtcode);

							print_member(member);

							if (rtcode == 0) {
								rtcode = Exit;
							}
						}
					}
				}

				break;

		case delete:	rtcode = handle_auth_access(key, value, &delete_member);
				break;

		case modify:	break;

		case display:	rtcode = handle_auth_access(key, value, &print_member);
				break;


		default:	rtcode = Undefined_action;
				break;
	}

	return(rtcode);
}

int main(int argc, char **argv) {
	char	key[BUFFSIZE] = { '\0' };
	char	value[BUFFSIZE] = { '\0' };

	int	rtcode = 0;

	do {
		if ((rtcode = getDataPair(key, value)) == 0) {
			rtcode = processPair(key, value);
		}
	} while (rtcode == 0);

	if (rtcode == Exit) {
		rtcode = 0;
	}
	
	return(rtcode);
}
