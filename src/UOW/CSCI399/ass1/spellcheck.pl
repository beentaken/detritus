#!/usr/bin/perl

# Handle the difference between Debian GNU/Linux and Solaris
open(WORDS, "/usr/share/dict/words") ||
  open(WORDS, "/usr/dict/words") ||
  die "I have no words and I must scream!\n";

# Get the word and nuke the key
$searchword = <STDIN>;
$searchword =~ s/^word=(.+)$/\1/;

# Lazily search the entire words file, stopping if we find the word
while (<WORDS>) {
  if (/^$searchword$/) {
    $response = "Found word '$searchword', it is spelt correcty.";
    last;
  }
}

# If we haven't found the word, set the response accordingly
if (! $response) {
  $response = "Couldn't find word '$searchword', it must be incorrectly spelt."
}

# Now print our HTML report on the word search
print <<END;
Content-Type: text/html; charset=ISO-8859-1

<html>
  <head><title>Spelling checker</title></head>
  <body>
    <h1>Spelling Checker</h1>
    <p>$response</p>
  </body>
</html>
END
