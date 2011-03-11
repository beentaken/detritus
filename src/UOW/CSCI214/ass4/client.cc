// iterative single-process program without sockets

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <set>
#include <sys/timeb.h>

typedef unsigned int u_int;

u_int factorial(u_int n);
set <string> permute(string word);
bool is_palindrome(string word);

int main(int argc, char **argv) {
	struct timeb		start, stop;

	const char		words_file[] = "/usr/share/dict/words";

	const u_int		data_file_len = 20;

	char			data_file[data_file_len] = "data.txt";

	set <string>		anagram, words, palindromes, permutations;

	set <set <string> >	anagrams;

	set <string>::const_iterator	p;

	ifstream	ifs;
	string		word;

	ftime(&start);

	switch (argc) {
		case	1:	break;

		case	2:	strcpy(data_file, argv[1]);
				break;

		default:	cerr << "usage: client <filename>" << endl;
				return(1);
	}
		
	ifs.open(words_file);

	while (ifs.good()) {
		string::iterator	q;

		ifs >> word;

		q = word.begin();

		while (q != word.end()) {
			*q = tolower(*q);
			q++;
		}

		words.insert(word);
	}

	ifs.close();

	cout << "Processing the file " << data_file << endl;

	ifs.open(data_file);

	while (ifs.good()) {
		string::iterator	q;

		anagram.clear();
		permutations.clear();

		ifs >> word;

		q = word.begin();

		while (q != word.end()) {
			*q = tolower(*q);

			if (!(isalnum(*q) || *q == '\'' || *q == '-')) {
				word.erase(q);
			} else {
				q++;
			}
		}

		if (is_palindrome(word)) {
			palindromes.insert(word);
		}

		permutations = permute(word);

		set_intersection(permutations.begin(), permutations.end(), words.begin(), words.end(), insert_iterator<set <string> >(anagram, anagram.begin()));

		if (anagram.size() > 1) {
			anagrams.insert(anagram);
		}
	}

	ifs.close();

	if (!palindromes.empty()) {
		cout << "Palindromes: ";

		p = palindromes.begin();

		while (true) {
			cout << *p;

			p++;

			if (p != palindromes.end()) {
				cout << ", ";
			} else {
				break;
			}
		}

		cout << endl;
	}

	if (!anagrams.empty()) {
		set <set <string> >::const_iterator	q;

		cout << "Anagrams:";

		q = anagrams.begin();

		while (true) {
			set <string>	foo = *q;

			p = foo.begin();

			while (p != foo.end()) {
				if (palindromes.find(*p) == palindromes.end()) {
					cout << ' ' << *p;
				}

				p++;
			}

			q++;

			if (q != anagrams.end()) {
				cout << ',';
			} else {
				break;
			}
		}

		cout << endl;
	}

	ftime(&stop);

	cout << "Processing time was " << stop.time - start.time << '.' << stop.millitm - start.millitm << " seconds." << endl;

	return 0;
}

// Calculate a factorial given n expressed as an unsigned integer
u_int factorial(u_int n) {
	u_int	result = 1;

	for (u_int i = 2; i <= n; i++) {
		result *= i;
	}

	return result;
}

// Generate a the set of permutations for a word
set <string> permute(string word) {
	set <string>	permutations;

	u_int		length = word.length();
	u_int		num_perms = factorial(length) - 1;

	permutations.insert(word);

	for (u_int i = 0; i < num_perms; i++) {
		if (next_permutation(word.begin(), word.end())) {
			permutations.insert(word);
		}
	}

	return permutations;
}

bool is_palindrome(string word) {
	bool	rt_code = true;

	if (word.length() > 1) {
		string::const_iterator	b, e;

		b = word.begin();
		e = word.end() - 1;

		while (rt_code && b < e) { 
			if (*b != *e) {
				rt_code = false;
			} else {
				b++;
				e--;
			}
		}
	} else {
		rt_code = false;
	}

	return rt_code;
}

