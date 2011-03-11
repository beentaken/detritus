<?php
	require('common.php');
	require('computer.php');

	$fin = FALSE;
	$message = NULL;
	$board = NULL;
	$expiry = time() + 3600 * 24 * 7;

	if (array_key_exists('othello', $_COOKIE)) {
		$board = $_COOKIE['othello'];

		if (array_key_exists('square', $_POST)) {
			$index = $_POST['square'];

			$computer = new Computer($board);

			if (!$computer->move($index)) {
				$message = $computer->message;
			} elseif ($computer->yourawinner !== NULL) {
				$fin = TRUE;
				$expiry = 1;

				$query = pg_query($pg_conn, "UPDATE csci399_ass2 "
					. "SET played = played + 1 "
					. "WHERE name = '" . $user . "';");

				$query = pg_query($pg_conn, "UPDATE csci399_ass2 "
					. "SET played = played + 1 "
					. "WHERE name = 'computer';");

					if ($computer->yourawinner) {
					$query = pg_query($pg_conn, "UPDATE csci399_ass2 "
						. "SET won = won + 1 "
						. "WHERE name ='" . $user . "';");
				} else { 
					$query = pg_query($pg_conn, "UPDATE csci399_ass2 "
						. "SET won = won + 1 "
						. "WHERE name ='computer';");
				}

				$message = $computer->message;
			}

			$board = $computer->getboard();
		}
	} else {
		$board = 'eeeeeeee'
			.'eeeeeeee'
			.'eeeeeeee'
			.'eeewbeee'
			.'eeebweee'
			.'eeeeeeee'
			.'eeeeeeee'
			.'eeeeeeee';
	}

	setcookie('othello', $board, $expiry);

	preamble("Playing a game of Othello");

	print	"\t<FORM ACTION=\"game.php\" METHOD=\"post\">\n"
	.	"\t<TABLE BORDER=\"2\" RULES=\"GROUPS\" FRAME=\"sides\""
	.	"SUMMARY=\"Playing Othello\" ALIGN=\"center\">\n";

	if ($message) {
		print "\t\t<CAPTION>" . $message . "</CAPTION>\n";
	} else {
		print "\t\t<CAPTION>Your turn</CAPTION>\n";
	}

	print	"\t\t<THEAD>\n"
	.  	"\t\t<TBODY>\n";

	$n_rows = 8;
	$n_cols = 8;
	$b = "black.png";
	$w = "white.png";
	$e = "blank.png";

	for ($i = 0; $i < $n_rows; $i++) {
		print "\t\t\t<TR>\n";

		for ($j = 0; $j < $n_cols; $j++) {
			$index = $i * $n_cols + $j;

			if ($fin) {
				print "\t\t\t\t<TD><IMG SRC=\"" . ${$board{$index}}
					. "\" NAME=\"square\" VALUE=\"" . $index . "\"></TD>\n";
			} else {
				print "\t\t\t\t<TD><INPUT TYPE=\"IMAGE\" SRC=\""
					. ${$board{$index}} . "\" NAME=\"square\" VALUE=\""
					. $index . "\"></TD>\n";
			}
		}

		print "\t\t\t</TR>\n";
	}

	if ($fin) {
		print	"\t\t\t<TR>\n"
		.	"\t\t\t<TD ALIGN=\"CENTER\" COLSPAN=\"" . (int) ($n_cols / 2)
		.	"\"><A HREF=\"game.php\">Play again?</A></TD>\n"
		.	"\t\t\t<TD ALIGN=\"CENTER\" COLSPAN=\"" . (int) ($n_cols / 2)
		.	 "\"><A HREF=\"league.php\">See the league?</A></TD>\n"
		.	"\t\t\t</TR>\n";
	}

	print "\t\t</TABLE>\n";

	postamble();
?>
