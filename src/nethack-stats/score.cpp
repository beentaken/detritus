#include "score.h"

Score::Score(int val) {
  set(val);
}

void Score::set(int val) {
  value = val;
}

istream & operator >> (istream &input, Score &score) {
  int val;

  input >> val;
  score.set(val);

  return(input);
}

ostream & operator << (ostream &output, const Score &score) {
  output << score.value;

  return(output);
}
