<?php
	// $Id: auth.php 2 2007-07-19 13:00:48Z pdezwart $

	$unauth_reason = NULL;

	function www_unauth() {
		header('WWW-Authenticate: Basic realm="Othello"');
		header('HTTP/1.0 401 Unauthorized');
	}

	if (!isset($_SERVER['PHP_AUTH_USER'])) {
		www_unauth();
		exit;
	} else {
		$user = $_SERVER['PHP_AUTH_USER'];
		$pg_conn = pg_connect("dbname=stuff user=www password=REDACTED");
		if ($pg_conn) {
			$query = pg_query($pg_conn, "SELECT * FROM csci399_ass2 WHERE name = '"
				. $user . "';");

			if (pg_num_rows($query)) {
				$row = pg_fetch_assoc($query);

				if ($row['passwd'] !== $_SERVER['PHP_AUTH_PW']) {
					www_unauth();
					$unauth_reason = "Password is incorrect.";
				}
			} else {
				www_unauth();
				$unauth_reason = "Username is incorrect.";
			}
		} else {
			www_unauth();
			$unauth_reason = "Couldn't connect to the database.";
		}
	}
?>
