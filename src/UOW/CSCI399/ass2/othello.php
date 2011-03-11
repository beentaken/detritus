<?php
	// $Id: othello.php 2 2007-07-19 13:00:48Z pdezwart $

	require('common.php');

	preamble("Welcome to the league of Othello players");

	print	"\t\t<H2>What would you like to do?</H2>\n"
	.	"\t\t<UL>\n"
	.	"\t\t\t<LI><A HREF=\"league.php\">View league standings.</A></LI>\n"
	.	"\t\t\t<LI><A HREF=\"game.php\">Have a game.</A></LI>\n"
	.	"\t\t</UL>\n";

	postamble();
?>
