// $Id: MapOfSets.h 2 2007-07-19 13:00:48Z pdezwart $

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


#ifndef _MAPOFSETS_H
#define _MAPOFSETS_H

#include <map>
#include <set>
#include <string>

/*!	MapOfSets

	A class which contains a map of sets, which can create a new set if the map doesn't have a set describing that key yet and can add another entry to an already existing set associated with a particular key.
*/
template <class T, class P> class MapOfSets {
	public:
		typedef	typename map <T, multiset <P> >::iterator iterator;
		typedef	typename map <T, multiset <P> >::const_iterator const_iterator;
		typedef typename set <P>::const_iterator set_iterator;

		/*!	insert()

			Insert a new value in to the MapOfSets, if key doesn't exist, create the new set, otherwise, put it in to the set with that key.

			\param	key	The key to use.
			\param	value	The templated value.
		*/
		void insert(const T key, P value) {
			iterator	p = _map.find(key);

			pair <T, multiset <P> >	_pair;

			if (p == _map.end()) {
				multiset <P>	_set;

				_set.insert(value);

				_pair.first = key;
				_pair.second = _set;
				_map.insert(_pair);
			} else {
				p->second.insert(value);
			}
		}

		/*!	begin()

			Get me a constant iterator to the internal map.

			\return		A constant iterator.
		*/
		const_iterator begin(void) const {
			return (_map.begin());
		}

		/*!	end()

			Get the iterator that is the end of the internal map.

			\return		The end iterator.
		*/
		const_iterator end(void) const {
			return (_map.end());
		}

	private:
		/*!	_map

			The internal STL map using a string for the key and a templated set for the value.
		*/
		map <const T, multiset <P> >	_map;
};

#endif
