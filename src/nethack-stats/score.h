
#ifndef __SCORE_H
#define __SCORE_H

#include <iostream>

class Score {

	// iostream friends
	friend istream & operator >> (istream &input, Score &score);
	friend ostream & operator << (ostream &output, const Score &score);

	public:
		// Default constructor
		Score(int val = 0);
		void set(int maj);

	private:
		int	value;
};

#endif
