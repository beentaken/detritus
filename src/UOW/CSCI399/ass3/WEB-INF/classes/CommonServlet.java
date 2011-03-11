import java.io.*;
import java.util.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;

/**
	The CommonServlet.

	This servelet will handle adding new patients to the database.

	@version $Revision: 1.13 $
	@author $Author: dezwart $
*/
public class CommonServlet extends HttpServlet {

	/**
		The database driver
	*/
	private static final String	dbdriver = "oracle.jdbc.driver.OracleDriver";

	/**
		The database uniform resource indicator
	*/
	private static final String	dburi = "jdbc:oracle:thin:@wraith:1521:csci";


	/**
		The date format we will use for input and output with a user.
	*/
	protected static final String	dfmt = "dd/MM/yyyy";

	/**
		The SQL date format.
	*/
	protected static final String	sqldfmt = "dd-MMM-yyyy";

	/**
		A connection to the database, waiting to be initialised.
	*/
	private Connection	dbconn;

	/**
		The DB statement that we will be using.
	*/
	private Statement	statement;

	/**
		Execute a SQL query and return the ResultSet

		@parameter query The query to execute.
		@return The ResultSet or null.
	*/
	protected ResultSet queryDB(String query) {
		ResultSet	results = null;

		try {
			results = statement.executeQuery(query);
		} catch (Exception e) {
			System.out.println("Query Failed:");
			e.printStackTrace();
		}

		return results;
	}

	/**
		Common HTML markup generator. Preamble.

		Creates a string of the common HTML preamble.
		
		@parameter title The title of the page.
		@return The generated String.
	*/
	protected String html_preamble(String title) {
		String	preamble = new String(
                        "<html>\n" +
                        "<head>\n" +
                        "<title>" + title + "</title>\n" +
                        "<link type='text/css' rel='stylesheet' href='style.css'>\n" +
                        "</head>\n" +
                        "<body>\n" +
                        "<h1>" + title + "</h1>"
                );

		return preamble;
	}

	/**
		Common HTML markup generator. Postamble.

		Creates a string of the common HTML postamble.

		@return The generated String.
	*/
	protected String html_postamble() {
		String postamble = new String();

		postamble = "\t<br>\n" +
				"\t<hr noshade size='3'>\n" +
				"\t<a href='index.html'>Return to the index</a>\n" +
				"</body>\n" +
				"</html>";

		return postamble;
	}

	/**
		Create a table row header, using the error class if that row
		was not formed correctly.

		@parameter header The name of the table header.
		@parameter error Do we need to use the error div.
		@return A String representing the table row.
	*/
	protected String table_row_header(String header, boolean error) {
		String	row_header = new String();

		row_header += "\t\t\t\t<th>";

		if (error) {
			row_header += "<div class='error'>" + header + "</div>";
		} else {
			row_header += header;
		}

		row_header += "</th>\n";

		return row_header;
	}

	/**
		Destroy method to release any resources.

		Should release the connection to the database.
	*/
	public void destroy() {
		if (dbconn != null) {
			try {
				dbconn.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/**
		Initialisaton function.

		This will attempt to get the database username and password
		from the settings in web.xml then initialise the connection to
		the database.
	*/
	public void init() throws ServletException {
		String	dbuser = getInitParameter("dbuser");
		String	dbpass = getInitParameter("dbpass");

		try {
			Class.forName(dbdriver);
			dbconn = DriverManager.getConnection(dburi, dbuser, dbpass);
			statement = dbconn.createStatement();
		} catch (Exception e) {
			System.out.println("Failed to connect to database");
			e.printStackTrace();
		}
	}
}
