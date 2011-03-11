<?php

	require('common.php');

	preamble("Othello league standings");

	$query = pg_query($pg_conn, "SELECT * FROM csci399_ass2;");

	if (pg_num_rows($query)) {

		print	"\t<TABLE BORDER=\"2\" RULES=\"GROUPS\" FRAME=\"sides\""
		.	"SUMMARY=\"Othello League Standings\" ALIGN=\"center\">\n"
		.	"\t\t<CAPTION>League Standings</CAPTION>\n"
		.	"\t\t<THEAD>\n"
		.	"\t\t\t<TR>\n"
		.	"\t\t\t\t<TH>Name:</TH>\n"
		.	"\t\t\t\t<TH>Played:</TH>\n"
		.	"\t\t\t\t<TH>Won:</TH>\n"
		.	"\t\t\t</TR>\n"
		.	"\t\t<TBODY>\n";

		while ($row = pg_fetch_assoc($query)) {
			print	"\t\t\t<TR>\n"
			.	"\t\t\t\t<TD>" . $row['name'] . "</TD>\n"
			.	"\t\t\t\t<TD ALIGN=\"RIGHT\">" . $row['played'] . "</TD>\n"
			.	"\t\t\t\t<TD ALIGN=\"RIGHT\">" . $row['won'] . "</TD>\n"
			.	"\t\t\t</TR>\n";

		}

		print "\t</TABLE>\n";
	}

	print	"<P><A HREF=\"othello.php\">Return to main page</A>\n";

	postamble();
?>
