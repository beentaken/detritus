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


#ifndef _LOGENTRY_H
#define _LOGENTRY_H

#include <iostream>
#include <string>

using namespace std;

/*! Logentry

    This class will describe a single line from the Nethack logfile.
*/
class Logentry {
        /*! >>

            Overloaded >> operator for stream extraction.

            \param  is  istream passed by reference.
            \param  log Logentry passed by by reference.
            \return     Reference to the original Output stream for operator cascading.
        */
        friend istream & operator >> (istream &is, Logentry &log);

        /*! <<

            Overloaded << operator for stream insertion.

            \param  os  ostream passed by reference.
            \param  log Constant Logentry passed by by reference.
            \return     Reference to the original Output stream for operator cascading.
        */
        friend ostream & operator << (ostream &os, const Logentry &log);

        /*! <

            Overloaded less than operator. It will compare the points variables of the left hand side to that of the right hand side.

            \param  lhs The left hand side operand.
            \param  rhs The right hand side operand.
            \return     TRUE if lhs.points < rhs.points.
        */
        friend bool operator < (const Logentry &lhs, const Logentry &rhs);

    public:
        /*! Logentry()

            Default constructor for the Logentry class.
        */
        Logentry();

        /*! ~Logentry()

            Destructor for the Logentry class.
        */
        ~Logentry();

        /*! get_version()

            Get the version of nethack this logentry is for.

            \return     The value of the version variable.
        */
        string  get_version(void) const { return version; }

        /*! get_points()

            Get the points this logentry has.

            \return     The value of the points variable.
        */
        long    get_points(void) const { return points; }

        /*! get_uid()

            Get the uid this logentry has.

            \return     The value of the uid variable.
        */
        string  get_uid(void) const { return uid; }

        /*! get_role()

            Get the role this logentry has.

            \return     The value of the role variable.
        */
        string  get_role(void) const { return role; }

        /*! get_race()

            Get the race this logentry has.

            \return     The value of the race variable.
        */
        string  get_race(void) const { return race; }

        /*! get_gender()

            Get the gender this logentry has.

            \return     The value of the gender variable.
        */
        string  get_gender(void) const { return gender; }

        /*! get_alignment()

            Get the alignment this logentry has.

            \return     The value of the alignment variable.
        */
        string  get_alignment(void) const { return alignment; }

        /*! get_name()

            Get the name this logentry has.

            \return     The value of the name variable.
        */
        string  get_name(void) const { return name; }

        /*! get_reason()

            Get the reason this logentry has.

            \return     The value of the reason variable.
        */
        string  get_reason(void) const { return reason; }

    private:
        /*! Version

            The version number of the Nethack binary that created this particular log entry.
        */
        string  version;

        /*! points

            The number of points that this logentry has. This is a calculated value by Nethack to show how good this particluar Nethacker was.
        */
        long    points;

        /*! deathdnum

            Believed to be the particular dungeon that the Nethacker died in.
        */
        int deathdnum;

        /*! deathlev

            The dungeon level that the Nethacker died at.
        */
        int deathlev;

        /*! maxlvl

            The maximum dungeon level that the Nethacker reached.
        */
        int maxlvl;

        /*! hp

            The hit points that the Nethacker had at the point of death.
        */
        int hp;

        /*! maxhp

            The maximum hit points that the Nethacker had at the point of death.
        */
        int maxhp;

        /*! deaths

            The total number of deaths that the Nethacker had.
        */
        int deaths;

        /*! deathdate

            The date the Nethacker died.
        */
        long    deathdate;

        /*! birthdate

            The date the Nethacker was born.
        */
        long    birthdate;

        /*! uid

            The user id of the person playing Nethack.
        */
        string  uid;

        /*! role

            The role of the Nethacker.
        */
        string  role;

        /*! race

            The race of the Nethacker.
        */
        string  race;

        /*! gender

            The gender of the Nethacker.
        */
        string  gender;

        /*! alignment

            The alignment of the Nethacker.
        */
        string  alignment;

        /*! name

            The name of the Nethacker.
        */
        string  name;

        /*! reason

            The reason the Nethacker died.
        */
        string  reason;
};

#endif
