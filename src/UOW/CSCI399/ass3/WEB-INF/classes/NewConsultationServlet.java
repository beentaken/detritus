import java.io.*;
import java.util.*;
import java.util.Date;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.text.SimpleDateFormat;

/**
	The NewConsultationServlet.

	This servelet will handle adding new consultations to the database.

	@version $Revision: 1.14 $
	@author $Author: dezwart $
*/
public class NewConsultationServlet extends CommonServlet {
	/**
		The doGet method for this servelet.

		This should output the empty consultation adding form.

		@parameter	request	The request object from the servlet container.
		@parameter	response The response object from the servlet container.
	*/
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();

		response.setContentType("text/html");

		out.println(html_preamble("New Consultation Form"));

		out.println(create_form("", "", "", 0., true));

		out.println(html_postamble());
	}

	/**
		The doPost method for the NewConsultationServlet.

		Does a lot of input checking and if there were problems, shows
		the form again, otherwise, it will process the request.

		@parameter request The request object from the servlet container.
		@parameter response The response object from the servlet container.
	*/
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();
		String		surname = new String();
		String		initials = new String();
		String		date = new String();
		SimpleDateFormat	sdf = new SimpleDateFormat(dfmt);
		Date		 thedate = null;

		int		clean = 0;
		double		weight = 0.;
		boolean		ready = true;

		response.setContentType("text/html");

		out.println(html_preamble("New Consultation Form"));

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

		date = request.getParameter("date");

		if (date == null || date.length() == 0) {
			out.println("\t<p>No date entered.</p>");
			date = new String();
			ready = false;
		} else {
			try {
				thedate = sdf.parse(date);
			} catch (Exception e) {
				out.println("Error in parsing the date: ");
				e.printStackTrace(out);
				date = "";
				ready = false;
			}
		}

		String foo = request.getParameter("weight");

		if (foo == null || foo.length() == 0) {
			out.println("\t<p>No Weight entered.</p>");
			ready = false;
		} else {
			try {
				weight = Double.parseDouble(foo);

				if (weight < 1. || weight > 1000.) {
					out.println("\t<p>Weight out of range (1. < weight <= 1000.).</p>");
					weight = 0.;
					ready = false;
				}
			} catch (Exception e) {
				weight = 0.;
				out.println("\t<p>Double parse error for Weight:");
				e.printStackTrace(out);
				ready = false;
			}
		}

		if (ready) {
			String		query = new String("SELECT ID FROM patients WHERE surname = '" + surname + "' AND initials = '" + initials + "'");
			String		temp = null;
			ResultSet	rs = queryDB(query);
			int		id = 0;
			boolean		successful = true;

			try {
				if (rs.next()) {
					temp = rs.getString("ID");
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
				SimpleDateFormat	sqlsdf = new SimpleDateFormat(sqldfmt);

				query = "INSERT INTO consultations (patient_id, consultation_date, weight) VALUES (" + id + ",'" + sqlsdf.format(thedate).toString() + "', " + weight + ")";

				if (queryDB(query) == null) {
					successful = false;
					out.println("\t<div class='error'><h2>DB query error</h1></div>");
				} else {
					out.println("\t<p>New consultation added to the database for patient id = " + id + "</p>");
				}
			}

			if (!successful) {
				
				out.println(create_form(surname, initials, date, weight, false));
			}
		} else {
			out.println(create_form(surname, initials, date, weight, false));
		}

		out.println(html_postamble());
	}

	/**
		Create the new consultation form.

		Using null values for the argument, and a -1 for clean, will
		mean to create a blank form.

		Otherwise, use the argument as default values and flag the ones
		without defaults as errorneous.

		@parameter surname The surname of the consultation.
		@parameter initials The initials of the consultation.
		@parameter date The date of the consultation.
		@parameter weight The weight of the consultation.
		@parameter clean boolean to show if we need a clean form.
		@return A String with the table HTML markup.
	*/
	private String create_form(String surname, String initials, String date, double weight, boolean clean) {
		String	TR = new String("\t\t\t<tr>\n");
		String	CTR = new String("\t\t\t</tr>\n");
		String	form = new String (
			"\t<form action='newconsultation' method='post'>\n" +
			"\t\t<table align='center' border='2'>\n" +
			"\t\t\t<caption>Please enter the new consultation data</caption>\n" +
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
			table_row_header("Weight", !clean && weight < 1);

		if (weight < 1) {
			form += "\t\t\t\t<td><input type='text' name='weight' size='3' maxsize='4' />\n";
		} else {
			form += "\t\t\t\t<td><input type='text' name='weight' size='3' maxsize='4' value='" + weight + "' />\n";
		}

		form +=	CTR +
			TR +
			table_row_header("Date: " + dfmt, !clean && date.length() == 0) +
			"\t\t\t\t<td><input type='text' name='date' size='10' maxsize='20' value='" + date + "' />\n" +
			CTR +
			TR +
			"\t\t\t\t<td colspan='2' align='center'>\n" +
			"\t\t\t\t\t<input type='submit' value=' Submit ' />\n" +
			CTR +
			"\t\t</table>";

		return form;
	}
}
