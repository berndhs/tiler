#ifndef TILER_BOND_MATCH_H
#define TILER_BOND_MATCH_H


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


namespace tiler
{

class Bond;

double MatchBond_None (const Bond & thisEnd, const Bond & otherEnd);
double MatchBond_Ionic (const Bond & thisEnd, const Bond & otherEnd);
double MatchBond_Covalent (const Bond & thisEnd, const Bond & otherEnd);
double MatchBond_Metallic (const Bond & thisEnd, const Bond & otherEnd);
double MatchBond_Aromatic (const Bond & thisEnd, const Bond & otherEnd);

bool ConnectBond_None (Bond & thisEnd, Bond & otherEnd);
bool ConnectBond_Ionic (Bond & thisEnd, Bond & otherEnd);
bool ConnectBond_Covalent (Bond & thisEnd, Bond & otherEnd);
bool ConnectBond_Metallic (Bond & thisEnd, Bond & otherEnd);
bool ConnectBond_Aromatic (Bond & thisEnd, Bond & otherEnd);

template <class T> int Signum (T value) 
  {
    return (value > T(0)) - (value < T(0));
  }

template <class T> bool SameSign (T val1, T val2)
  {
    return Signum (val1) == Signum (val2);
  }

} // namespace

#endif
