#!/bin/bash

# Amuse/annoy with random words at random intervals.
# Great for breaking the ice at parties.
# Needs `say`, try on a Mac.

declare -a words

i=0

for word in `cat /usr/share/dict/words`
do
  words[$i]="$word"
  let i+=1
done

while true
do
  index=$(($RANDOM*$RANDOM%$i))
  word="${words[$index]}"
  sleepy=$RANDOM
  echo "Sleeping for $sleepy to say $word"
  sleep $RANDOM
  say "$word"
done
