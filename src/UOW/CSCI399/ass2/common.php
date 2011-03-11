<?php

	require('auth.php');

	function preamble($title) {
		print	"<HTML>\n"
		.	"\t<HEAD>\n"
		.	"\t\t<TITLE>" . $title . "</TITLE>\n"
		.	"\t</HEAD>\n"
		.	"\t<BODY LINK=\"#00FF00\" VLINK=\"#008000\" BGCOLOR=\"#000000\" "
		.	"TEXT=\"#00F000\">\n"
		.	"\t\t<H1>" . $title . ".</H1>\n";
	}

	function postamble() {
		print	"\t</BODY>\n"
		.	"</HTML>\n";
	}
?>
