:-dynamic dogs/1.
:-assert(dogs([])).
:-go.

% append([],X,X).
% append([A|B],Y,[A|W]):-append(B,Y,W).

prepend(X,[],[X]).
prepend(X,Y,[X|Y]).

% adddog(X):-dogs(L),retract(dogs(L)),append(L,[X],M),assert(dogs(M)).

adddog(X):-dogs(L),retract(dogs(L)),prepend(X,L,M),assert(dogs(M)).

isdog(X):-dogs(L),nl,match(X,L).

match(X,[]):-write('No, '),write(X),write(' is not a dog.'),nl.
match(X,[Y|_]):-X=Y,write('Yes, '),write(X),write(' is a dog.'),nl.
match(X,[Y|T]):-not X=Y,match(X,T).

go:-add_loop,test_loop.

add_loop:-fmt_read("%s",X,_),add_rep(X).

add_rep(X):-X=stop,!.
add_rep(X):-not X=stop,adddog(X),add_loop.

test_loop:-fmt_read("%s",X,_),test_rep(X).

test_rep(X):-X=stop,!.
test_rep(X):-not X=stop,isdog(X),test_loop.
