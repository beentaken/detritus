/* $Id: UseDB.java 2 2007-07-19 13:00:48Z pdezwart $ */

import java.sql.*;
import java.io.*;

/**
	Procedural program for interacting with a simple SQL database of
	Players and scores with one match per line.
*/
public class UseDB {
	private static final String	USER = "pntdz31";
	private static final String	PASS = "REDACTED";
	private static final String	dbDriverName = "oracle.jdbc.driver.OracleDriver";
	private static final String	dbURL = "jdbc:oracle:thin:@wraith:1521:csci";

	private static Connection	dbConnection;

	private static Statement	aStatement;

	private static final int	NAME_LEN = 32;

	/**
		Main function.
		@arg argv	Argument Vector.
	*/
	public static void main(String argv[]) {
		boolean	control = true;
		int	option = 0;

		connectToDatabase(USER, PASS);

		while (control) {

			// Display the menu
			menu();

			option = get_int(System.in);
			// Flow through to the desired option
			switch (option) {
				case 1:	add();
					break;

				case 2:	list();
					break;

				case 3:	OnePlayer();
					break;

				case 4:	TwoPlayer();
					break;

				case 5:	control = false;
					break;

				default:	System.out.println("Invalid selection: " + option);
						break;
			}
		}

		closedown();

	}

	private static String get_name(InputStream is) {
		char []	name = new char[NAME_LEN];

		char	tmp = ' ';

		int	i = 0;

		// Clean the array
		for (i = 0; i < NAME_LEN; i++) {
			name[i] = '\0';
		}

		// Read up to NAME_LEN
		for (i = 0; i < NAME_LEN; i++) {
			try {
				tmp = (char)is.read();
			} catch (IOException e) {
				e.printStackTrace();
				System.exit(1);
			}

			if (tmp != '\n') {
				name[i] = (char)tmp;
			} else {
				break;
			}
		}

		// Snarf up to the newline
		if (tmp != '\n') {
			do {
				try {
					tmp = (char)is.read();
				} catch (IOException e) {
					e.printStackTrace();
					System.exit(1);
				}
			} while (tmp != '\n');
		}

		return(new String(name));
	}
		
	private static int get_int(InputStream is) {
		int	number = 0;
		int	sign = 1;
		int	tmp = 0;

		// Mark the current position in the stream
		is.mark(2);
	
		// Check for an optional sign
		try {
			tmp = is.read();
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}

		switch (tmp) {
			case '-':	sign = -1;
					break;

			case '+':	sign = 1;
					break;

			default:	try {
						is.reset();
					} catch (IOException e) {
						e.printStackTrace();
						System.exit(1);
					}

					break;
		}

		// Start parsing the ASCII int
		// Caveat emptor, the first non numerical character will be munched.
		while (true) {
			try {
				tmp = is.read();
			} catch (IOException e) {
				e.printStackTrace();
				System.exit(1);
			}

			if (tmp >= '0' && tmp <= '9') {
				number *= 10;
				number += tmp - '0';
			} else {
				break;
			}
		}

		return (number * sign);
	}

	private static void menu() {
		System.out.print(
			"1:\tAdd data\n" +
			"2:\tList data\n" +
			"3:\tOne player request\n" +
			"4:\tTwo player request\n" +
			"5:\tQuit.\n" +
			"Selection? "
		);
	}

	private static void connectToDatabase(String User, String Pass) {
		try {
			Class.forName (dbDriverName);

			dbConnection = DriverManager.getConnection(dbURL, User, Pass);

			aStatement = dbConnection.createStatement();
		} catch(Exception e) {
			System.out.println("Failed to connect to database because:");
			System.out.println(e);
			System.exit(1);
		}
	}

	private static ResultSet queryDB(String query) {
		ResultSet	rtval = null;

		try {
			rtval = aStatement.executeQuery(query);
		} catch (Exception e) {
			System.out.println("SQL Query failed: " + e);
		}

		return(rtval);
	}

	private static void closedown() {
		try {
			aStatement.close();
			dbConnection.close();
		} catch(Exception e) {
			System.out.println("Close down failed because " + e);
		}
	}

	private static void add() {
		String	Player1;
		String	Player2;

		String 	SQL_Update;

		int	Score1;
		int	Score2;

		System.out.print("Enter the name of Player 1: ");
		Player1 = get_name(System.in);

		System.out.print("Enter the name of Player 2: ");
		Player2 = get_name(System.in);

		System.out.print("Enter Player 1's Score: ");
		Score1 = get_int(System.in);

		System.out.print("Enter Player 2's Score: ");
		Score2 = get_int(System.in);

		SQL_Update = new String("INSERT INTO PLAYERS VALUES ( " +
					"\'" + Player1 + "\'" + ", " +
					"\'" + Player2 + "\'" + ", " +
					Score1 + ", " +
					Score2 + " " +
					")");

		queryDB(SQL_Update);
	}

	private static void list() {
		String		SQL_Query = new String("SELECT * FROM PLAYERS");

		printResults(queryDB(SQL_Query), "There is nothing to list.");
	}

	private static void printResults(ResultSet results, String error) {
		int	match = 0;

		System.out.println();

		try {
			while (results.next()) {
				System.out.println(
					"Match #" + match++ + "\n" +
					"Player 1:\t" + results.getString("NAME1").trim() + "\t" +
					"score:\t"+ results.getInt("SCORE1") + "\n" +
					"Player 2:\t" + results.getString("NAME2").trim() + "\t" +
					"score:\t" + results.getInt("SCORE2") + "\n"
				);
			}
		} catch (Exception e) {
			System.out.println("SQL Query failed: " + e);
		}

		if (match == 0) {
			System.out.println(error + "\n");
		}
	}

	private static void OnePlayer() {
		boolean	control = true;

		int	option = 0;

		String	name;

		System.out.print("Name to search for: ");

		name = get_name(System.in);

		while (control) { 
			System.out.print(
				"1:\tMax Score\n" +
				"2:\tList data\n" +
				"Selection? "
			);

			option = get_int(System.in);

			switch (option) {
				case 1:		OneMax(name);
						control = false;
						break;

				case 2:		OneList(name);
						control = false;
						break;

				default:	System.out.println("Invalid selection: " + option);
						break;
			}
		}
	}

	private static void OneMax(String name) {
		String	SQL_Query = new String("SELECT * FROM PLAYERS WHERE (NAME1 = '" +
					name + "') OR (NAME2 = '" + name +"')");

		int	max = 0;
		int	tmp = 0;

		boolean	something = false;

		ResultSet	results = queryDB(SQL_Query);

		try {
			while (results.next()) {
				if (results.getString("NAME1") == name) {
					tmp = results.getInt("SCORE1");
				} else {
					tmp = results.getInt("SCORE2");
				}

				if (tmp > max) {
					max = tmp;
					something = true;
				}
			}
		} catch (Exception e) {
			System.out.println("SQL Query failed: " + e);
		}

		if (something) {
			System.out.println("\nThe higest score for " + name + " is: " + max + "\n");
		} else {
			System.out.println("\nThere were no scores for " + name + ".\n");
		}
	}

	private static void OneList(String name) {
		String	SQL_Query = new String("SELECT * FROM PLAYERS WHERE (NAME1 = '" +
					name + "') OR (NAME2 = '" + name +"')");

		printResults(queryDB(SQL_Query), "There were no Games with " + name + ".");
	}

	private static void TwoPlayer() {
		boolean	control = true;

		int	option = 0;

		String	name1, name2;

		System.out.print("First name to search for: ");

		name1 = get_name(System.in);

		System.out.print("Second name to search for: ");

		name2 = get_name(System.in);

		while (control) { 
			System.out.print(
				"1:\tDraws for " + name1 + " and " + name2 + "\n" +
				"2:\tGames between " + name1 + " and " + name2 + "\n" +
				"Selection? "
			);

			option = get_int(System.in);

			switch (option) {
				case 1:		TwoDraws(name1, name2);
						control = false;
						break;

				case 2:		TwoGames(name1, name2);
						control = false;
						break;

				default:	System.out.println("Invalid selection: " + option);
						break;
			}
		}
	}

	private static void TwoDraws(String name1, String name2) {
		String	SQL_Query = new String("SELECT * FROM PLAYERS WHERE (((NAME1 = '" +
					name1 + "') AND (NAME2 = '" + name2 + "')) OR ((NAME1 = '" +
					name2 + "') AND (NAME2 = '" + name1 + "'))) AND (SCORE1 = SCORE2)");

		printResults(queryDB(SQL_Query),"There were no Draws between " + name1 + " and " + name2 + ".");
	}

	private static void TwoGames(String name1, String name2) {
		String	SQL_Query = new String("SELECT * FROM PLAYERS WHERE ((NAME1 = '" +
					name1 + "') AND (NAME2 = '" + name2 + "')) OR ((NAME1 = '" +
					name2 + "') AND (NAME2 = '" + name1 + "'))");

		printResults(queryDB(SQL_Query), "There were no Games between " + name1 + " and " + name2 + ".");
	}
}
