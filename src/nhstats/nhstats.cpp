// $Id: nhstats.cpp 2 2007-07-19 13:00:48Z pdezwart $

/*
nhstats, a Nethack statistics CGI program.
Copyright (C) 2005  Pete de Zwart <dezwart@froob.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <set>
#include <vector>
#include <fstream>
#include "Logentry.h"
#include "MapOfSets.h"

const int	num_maps = 8;

enum	_maps {
		_uid,
		_role,
		_race,
		_gender,
		_alignment,
		_version,
		_reason,
		_name
	};

string	_map_names[num_maps] = {
		"User ID",
		"Role",
		"Race",
		"Gender",
		"Alignment",
		"Version",
		"Reason",
		"Name"
	};

int main(int argc, char **argv) {
	MapOfSets <string, Logentry>	maps[num_maps];

	Logentry	logentry;

	ifstream	ifs;

	int	i;

	string	logfile = "/var/games/nethack/logfile";

	ifs.open(logfile.c_str());

	if (ifs.good()) {
		ifs >> logentry;
		while (!ifs.eof()) {
			if (logentry.get_reason() != "quit") {
				maps[_role].insert(logentry.get_role(), logentry);
				maps[_race].insert(logentry.get_race(), logentry);
				maps[_gender].insert(logentry.get_gender(), logentry);
				maps[_alignment].insert(logentry.get_alignment(), logentry);
				maps[_name].insert(logentry.get_name(), logentry);
				maps[_reason].insert(logentry.get_reason(), logentry);
				maps[_uid].insert(logentry.get_uid(), logentry);
				maps[_version].insert(logentry.get_version(), logentry);
			}

			ifs >> logentry;
		}

		ifs.close();

		// HTML Preamble
		cout << "Content-Type: text/html; charset=ISO-8859-1\n"
			<< "\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "\t<title>Nethack Statistics</title>\n"
			<< "\t<style>\n"
			<< "\t\tbody\t{ font-family: monospace; color: #00C000; background: #000000; }\n"
			<< "\t</style>\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "\t<h1>Nethack statistics</h1>\n"
			<< "\t<br>\n"
			<< "\t<p>$Id: nhstats.cpp 2 2007-07-19 13:00:48Z pdezwart $</p>\n"
			<< "\t<br>\n"
			<< "\t<table border='2' rules='groups' frame='hsides' summary='Nethack table of statistics'>\n"
			<< "\t\t<caption>Nethack table of statistics</caption>\n"
			<< "\t\t<colgroup align='left'>\n"
			<< "\t\t<colgroup align='left'>\n"
			<< "\t\t<colgroup align='right'>\n"
			<< "\t\t<colgroup align='right'>\n"
			<< "\t\t<colgroup align='right'>\n"
			<< "\t\t<colgroup align='right'>\n";

		for (i = 0; i < num_maps; i++) {
			MapOfSets <string, Logentry>::const_iterator	p = maps[i].begin();

			string	tr = "\t\t<tr>\n";
			string	trc = "\t\t<tr style='background: #003000'>\n";
			string	td = "\t\t\t<td>";
			string	tdr = "\t\t\t<td align='right'>";
			string	etd = "</td>\n";
			string	etr = "\t\t</tr>\n";

			int	color_row = 0;

			cout << "\t\t<thead>\n"
				<< tr
				<< "\t\t\t<th>" << _map_names[i] << "</th>\n"
				<< "\t\t\t<th>Leader</th>\n"
				<< "\t\t\t<th>High score</th>\n"
				<< "\t\t\t<th>Version</th>\n"
				<< "\t\t\t<th>Count</th>\n"
				<< "\t\t\t<th>Average score</th>\n"
				<< etr
				<< "\t\t<tbody>\n";

			while (p != maps[i].end()) {
				MapOfSets <string, Logentry>::set_iterator	s = p->second.begin();
				int	total = 0;

				if (color_row) {
					cout << trc;
					color_row = 0;
				} else {
					cout << tr;
					color_row = 1;
				}

				cout << td << p->first << etd
					<< td << s->get_name() << etd
					<< tdr << s->get_points() << etd
					<< tdr << s->get_version() << etd
					<< tdr << p->second.size() << etd;

				while (s != p->second.end()) {
					total += s->get_points();
					s++;
				}

				cout << tdr << total / p->second.size() << etd
					<< etr;

				p++;
			}
		}

		// HTML Postamble
		cout << "\t</table>\n"
			<< "</body>\n"
			<< "</html>" << endl;

	} else {
		cerr << "Error opening: " << logfile << endl;
	}

	return(0);
}
