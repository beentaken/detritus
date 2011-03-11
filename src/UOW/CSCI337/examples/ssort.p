ssort(X,Y):-nl,permutation(X,Y),sorted(Y).

sorted([]).
sorted([_]).
sorted([X,Y|Z]):-X=<Y,sorted([Y|Z]).

permutation([],[]).
permutation(X,[Y|Z]):-append(U,[Y|V],X),append(U,V,W),permutation(W,Z).

append([],X,X).
append([A|B],Y,[A|W]):-append(B,Y,W).
