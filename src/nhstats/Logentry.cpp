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

#include "Logentry.h"

Logentry::Logentry() {
  points = 0;
  deathdnum = 0;
  deathlev = 0;
  maxlvl = 0;
  hp = 0;
  maxhp = 0;
  deaths = 0;
  deathdate = 99999999;
  birthdate = 99999999;
  uid = "NUL";
  role = "NUL";
  race = "NUL";
  gender = "NUL";
  alignment = "NUL";
  name = "NUL";
  reason = "NUL";
}

Logentry::~Logentry() {
}

ostream & operator << (ostream &os, const Logentry &log) {
  os << log.version << ' ' << log.points << ' ' << log.deathdnum << ' '
    << log.deathlev << ' ' << log.maxlvl << ' ' << log.hp << ' '
    << log.maxhp << ' ' << log.deaths <<  ' ' << log.deathdate
    << ' ' << log.birthdate << ' ' << log.uid << ' ' << log.role
    << ' ' << log.race << ' ' << log.gender << ' ' << log.alignment
    << ' ' << log.name << ',' << log.reason;

  return(os);
}

istream & operator >> (istream &is, Logentry &log) {
  is >> log.version >> log.points >> log.deathdnum >> log.deathlev
    >> log.maxlvl >> log.hp >> log.maxhp >> log.deaths
    >> log.deathdate >> log.birthdate >> log.uid >> log.role
    >> log.race >> log.gender >> log.alignment;

  getline(is, log.name, ',');
  log.name.erase(0,1);
  getline(is, log.reason);

  return(is);
}

bool operator < (const Logentry &lhs, const Logentry &rhs) {
  bool rtcode;

  if (lhs.points == rhs.points) {
    rtcode = lhs.name < rhs.name;
  } else {
    rtcode = lhs.points > rhs.points;
  }

  return(rtcode);
}
