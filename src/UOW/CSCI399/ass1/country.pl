#!/usr/bin/perl

open(DATA, "Data.txt") ||
  die "I have no Data and I must scream!\n";

while (<DATA>) {
  chomp($_);
  @country = split /, / ;
  %gdp = ($country[5], $country[0], %gdp);
  %popdens = ($country[3] / $country[2], $country[0], %popdens);
}

@lgdp = sort { $a <=> $b } keys %gdp;
@lpopdens = sort { $a <=> $b } keys %popdens;

print "A list of the five countries with the lowest GDP:\n\n";

$count = 5;
until ($count == 0) {
  print $gdp{$lgdp[$count]}, ":\t", $lgdp[$count], " GDP\n";
} continue {
  $count--;
}

print "\nA list of the 5 most sparsely populated countries:\n\n";

$count = 5;
until ($count == 0) {
  print $popdens{$lpopdens[$count]}, ":\t", $lpopdens[$count], " P/Sq. KM\n";
} continue {
  $count--;
}

print "\nA list of the 5 most desely populated countries:\n\n";

# Yeah, this is the cheesy method, extra lasy, taking advantage of how perl
# handles negative array indicies.

$count = -1;
until ($count == -6) {
  print $popdens{$lpopdens[$count]}, ":\t", $lpopdens[$count], " P/Sq. KM\n";
} continue {
  $count--;
}

