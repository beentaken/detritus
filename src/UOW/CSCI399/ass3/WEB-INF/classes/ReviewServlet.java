import java.io.*;
import java.util.*;
import java.util.Date;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.text.SimpleDateFormat;

/**
	The ReviewServlet.

	This servelet will handle reviewing patient consultation data from
	the database.

	@version $Revision: 1.11 $
	@author $Author: dezwart $
*/
public class ReviewServlet extends CommonServlet {
	/**
		The doGet method for this servelet.

		This should output the empty review form.

		@parameter	request	The request object from the servlet container.
		@parameter	response The response object from the servlet container.
	*/
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();

		response.setContentType("text/html");

		out.println(html_preamble("Review Form"));

		out.println(create_form("", "", true));

		out.println(html_postamble());
	}

	/**
		The doPost method for the ReviewServlet.

		Does a lot of input checking and if there were problems, shows
		the form again, otherwise, it will process the request.

		@parameter request The request object from the servlet container.
		@parameter response The response object from the servlet container.
	*/
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();
		String		surname = new String();
		String		initials = new String();

		int		clean = 0;
		boolean		ready = true;

		response.setContentType("text/html");

		out.println(html_preamble("Review Form"));

		surname = request.getParameter("surname");

		if (surname.length() == 0) {
			out.println("\t<p>No Surname entered.</p>");
			ready = false;
		} else if (surname.length() > 32) {
			out.println("\t<p>Surname too long.</p>");
			surname = "";
			ready = false;
		}

		initials = request.getParameter("initials");

		if (initials.length() == 0) {
			out.println("\t<p>No Initials entered.</p>");
			ready = false;
		} else if (initials.length() > 8) {
			out.println("\t<p>Initials too long.</p>");
			initials = "";
			ready = false;
		}

		if (ready) {
			String		query = new String("SELECT * FROM patients WHERE surname = '" + surname + "' AND initials = '" + initials + "'");
			String		temp = null;
			ResultSet	rs = queryDB(query);
			int		id = 0;
			boolean		successful = true;
			double		height = 1;

			try {
				if (rs.next()) {
					temp = rs.getString("ID");
					out.println("<table align='center' border='3'>");
					out.println("<tr>");
					out.println("<th>Surname</th>");
					out.println("<td>" + rs.getString("surname") + "</td>");
					out.println("</tr>");
					out.println("<tr>");
					out.println("<th>Initials</th>");
					out.println("<td>" + rs.getString("initials") + "</td>");
					out.println("</tr>");
					out.println("<tr>");
					out.println("<th>Gender</th>");
					out.println("<td>" + rs.getString("gender") + "</td>");
					out.println("</tr>");
					out.println("<tr>");
					out.println("<th>Age</th>");
					out.println("<td>" + rs.getString("age") + "</td>");
					out.println("</tr>");
					out.println("<tr>");
					out.println("<th>Height</th>");

					height = rs.getDouble("height");

					out.println("<td>" + height + "</td>");
					out.println("</tr>");
					out.println("</table>");
				} else {
					out.println("\t<p>No such person: " + initials + " " + surname + "</p>");
					initials = "";
					surname = "";
					successful = false;
				}
			} catch (Exception e) {
				out.println("A DB error occured:");
				e.printStackTrace(out);
				successful = false;
			}

			if (successful && temp != null) {
				try {
					id = Integer.parseInt(temp);
				} catch (Exception e) {
					out.println("\t<p>Parsing error when attempting to get the patient ID form the database:");
					e.printStackTrace(out);
					successful = false;
				}
			}

			if (successful) {

				query = "SELECT * FROM consultations WHERE patient_id = " + id;

				rs = queryDB(query);
				if (rs == null) {
					successful = false;
					out.println("\t<div class='error'><h2>DB query error</h1></div>");
				} else {
					out.println("<table align='center' border='2'>");
					out.println("<tr>");
					out.println("<th>Date</th>");
					out.println("<th>Weight</th>");
					out.println("<th>BMI</th>");
					out.println("</tr>");
					try {
						while (rs.next()) {
							double	weight = rs.getDouble("weight");
							out.println("<tr>");
							out.println("<td>" + rs.getString("consultation_date") + "</td>");
							out.println("<td>" + weight + "</td>");
							out.println("<td>" + weight / (height * height) + "</td>");
							out.println("</tr>");
						}
					} catch (Exception e) {
						out.println("</table>");
						out.println("A DB error occured:");
						e.printStackTrace(out);
						successful = false;
					}

					out.println("</table>");
				}
			}

			if (!successful) {
				
				out.println(create_form(surname, initials, false));
			}
		} else {
			out.println(create_form(surname, initials, false));
		}

		out.println(html_postamble());
	}

	/**
		Create the review form.

		Using null values for the argument, and a -1 for clean, will
		mean to create a blank form.

		Otherwise, use the argument as default values and flag the ones
		without defaults as errorneous.

		@parameter surname The surname of the consultation.
		@parameter initials The initials of the consultation.
		@parameter clean boolean to show if we need a clean form.
		@return A String with the table HTML markup.
	*/
	private String create_form(String surname, String initials, boolean clean) {
		String	TR = new String("\t\t\t<tr>\n");
		String	CTR = new String("\t\t\t</tr>\n");
		String	form = new String (
			"\t<form action='review' method='post'>\n" +
			"\t\t<table align='center' border='2'>\n" +
			"\t\t\t<caption>Please enter the review data</caption>\n" +
			TR
		);

		// If clean == true, then we just print the normal form
		form += table_row_header("Surname", !clean && surname.length() == 0) +
			"\t\t\t\t<td><input type='text' name='surname' size='16' maxsize='32' value='" + surname +"' />\n" +
			CTR +
			TR +
			table_row_header("Initials", !clean && initials.length() == 0) +
			"\t\t\t\t<td><input type='text' name='initials' size='4' maxsize='8' value='" + initials + "' />\n" +
			CTR +
			TR +
			"\t\t\t\t<td colspan='2' align='center'>\n" +
			"\t\t\t\t\t<input type='submit' value=' Submit ' />\n" +
			CTR +
			"\t\t</table>";

		return form;
	}
}
