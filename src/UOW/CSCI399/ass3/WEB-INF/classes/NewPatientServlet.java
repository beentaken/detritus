import java.io.*;
import java.util.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;

/**
	The NewPatientServlet.

	This servelet will handle adding new patients to the database.

	@version $Revision: 1.46 $
	@author $Author: dezwart $
*/
public class NewPatientServlet extends CommonServlet {
	/**
		The doGet method for this servelet.

		This should output the empty patient adding form.

		@parameter	request	The request object from the servlet container.
		@parameter	response The response object from the servlet container.
	*/
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();

		response.setContentType("text/html");

		out.println(html_preamble("New Patient Form"));

		out.println(create_form("", "", "", -1, 0.));

		out.println(html_postamble());
	}

	/**
		The doPost method for the NewPatientServlet.

		Does a lot of input checking and if there were problems, shows
		the form again, otherwise, it will process the request.

		@parameter request The request object from the servlet container.
		@parameter response The response object from the servlet container.
	*/
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		PrintWriter	out = response.getWriter();
		String		surname = new String();
		String		initials = new String();
		String		gender = new String();
		int		age = 0;
		double		height = 0.;
		boolean		ready = true;

		response.setContentType("text/html");

		out.println(html_preamble("New Patient Form"));

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

		gender = request.getParameter("gender");

		if (gender == null || gender.length() == 0) {
			out.println("\t<p>No gender selected.</p>");
			gender = new String();
			ready = false;
		} else if (!(gender.equals("male") || gender.equals("female"))) {
			out.println("\t<p>Unknown gender " + gender + ".</p>");
			gender = "";
			ready = false;
		}

		String	foo = request.getParameter("age");

		if (foo.length() == 0) {
			out.println("\t<p>No Age entered</p>");
			ready = false;
		} else {
			try {
				age = Integer.parseInt(foo);

				if (age == -1) {
					out.println("\t<p>No Age entered.</p>");
					age = 0;
					ready = false;
				} else if (age <= 0 || age > 150) {
					out.println("\t<p>Age out of range (0 < age <= 150).</p>");
					age = 0;
					ready = false;
				}
			} catch (Exception e) {
				age = 0;
				out.println("\t<p>Integer parse error for Age:");
				e.printStackTrace(out);
				ready = false;
			}
		}

		foo = request.getParameter("height");

		if (foo == null || foo.length() == 0) {
			out.println("\t<p>No Height entered.</p>");
			ready = false;
		} else {
			try {
				height = Double.parseDouble(foo);

				if (height < 1. || height > 3.5) {
					out.println("\t<p>Height out of range (1. < height <= 3.5).</p>");
					height = 0.;
					ready = false;
				}
			} catch (Exception e) {
				height = 0.;
				out.println("\t<p>Double parse error for Height:");
				e.printStackTrace(out);
				ready = false;
			}
		}

		if (ready) {
			String		query = new String("SELECT MAX(ID) FROM patients");
			String		temp = null;
			ResultSet	rs = queryDB(query);
			int		id = 0;
			boolean		successful = true;

			try {
				if (rs.next()) {
					temp = rs.getString("MAX(ID)");
				}
			} catch (Exception e) {
				out.println("A DB error occured:");
				e.printStackTrace(out);
				successful = false;
			}

			if (temp != null) {
				try {
					id = Integer.parseInt(temp);
					id++;
				} catch (Exception e) {
					out.println("\t<p>Parsing error when attempting to get the maximum patient ID form the database:");
					e.printStackTrace(out);
					successful = false;
				}
			}

			if (successful) {
				query = "INSERT INTO patients (id, surname, initials, age, gender, height) VALUES (" + id + ",'" + surname + "', '" + initials + "', " + age + ", '" + gender.charAt(0) + "', " + height + ")";

				if (queryDB(query) == null) {
					successful = false;
				} else {
					out.println("\t<p>New patient added to the database with id = " + id + "</p>");
				}
			}

			if (!successful) {
				
				out.println(create_form(surname, initials, gender, age, height));
			}
		} else {
			out.println(create_form(surname, initials, gender, age, height));
		}

		out.println(html_postamble());
	}

	/**
		Create the new patient form.

		Using null values for the argument, and a -1 for age, will
		mean to create a blank form.

		Otherwise, use the argument as default values and flag the ones
		without defaults as errorneous.

		@parameter surname The surname of the patient.
		@parameter initials The initials of the patient.
		@parameter gender The gender of the patient.
		@parameter age The age of the patient.
		@parameter height The height of the patient.
		@return A String with the table HTML markup.
	*/
	private String create_form(String surname, String initials, String gender, int age, double height) {
		String	TR = new String("\t\t\t<tr>\n");
		String	CTR = new String("\t\t\t</tr>\n");
		String	form = new String (
			"\t<form action='newpatient' method='post'>\n" +
			"\t\t<table align='center' border='2'>\n" +
			"\t\t\t<caption>Please enter the new patient data</caption>\n" +
			TR
		);

		// If age == -1, then we just print the normal form
		form += table_row_header("Surname", age >= 0 && surname.length() == 0) +
			"\t\t\t\t<td><input type='text' name='surname' size='16' maxsize='32' value='" + surname +"' />\n" +
			CTR +
			TR +
			table_row_header("Initials", age >= 0 && initials.length() == 0) +
			"\t\t\t\t<td><input type='text' name='initials' size='4' maxsize='8' value='" + initials + "' />\n" +
			CTR +
			TR +
			table_row_header("Age", age == 0);

		if (age <= 0) {
			form += "\t\t\t\t<td><input type='text' name='age' size='2' maxsize='3' />\n";
		} else {
			form += "\t\t\t\t<td><input type='text' name='age' size='2' maxsize='3' value='" + age + "' />\n";
		}

		form +=	CTR +
			TR +
			table_row_header("Gender", age >= 0 && gender.length() == 0);

		boolean	male, female;

		male = false;
		female = false;

		if (gender.length() > 0) {
			if (gender.equals("male")) {
				male = true;
			} else if (gender.equals("female")) {
				female = true;
			}

			form += "\t\t\t\t<td><input type='radio' name='gender' value='male' checked='" + male + "' /> Male <input type='radio' name='gender' value='female' checked='" + female + "' /> Female\n";
		} else {
			form += "\t\t\t\t<td><input type='radio' name='gender' value='male' /> Male <input type='radio' name='gender' value='female' /> Female\n";
		}

		form += CTR +
			TR +
			table_row_header("Height", age >= 0 && height < 1);

		if (height < 1) {
			form += "\t\t\t\t<td><input type='text' name='height' size='3' maxsize='4' />\n";
		} else {
			form += "\t\t\t\t<td><input type='text' name='height' size='3' maxsize='4' value='" + height + "' />\n";
		}

		form +=	CTR +
			TR +
			"\t\t\t\t<td colspan='2' align='center'>\n" +
			"\t\t\t\t\t<input type='submit' value=' Submit ' />\n" +
			CTR +
			"\t\t</table>";

		return form;
	}
}
