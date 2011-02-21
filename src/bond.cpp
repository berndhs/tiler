#include "bond.h"


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

#include "bond-functions.h"
#include <QtOpenGL>

namespace tiler
{

bool Bond::haveMaps (false);

Bond::MatchMapType     Bond::matchFunc;
Bond::ConnectMapType   Bond::connectFunc;

Bond::Bond ()
  :type(Bond_None),
   value (0)
{
}

Bond::Bond (const Bond & other)
  :type (other.type),
   value (other.value)
{
}

Bond::Bond (BondType bondType)
  :type(bondType),
   value (0)
{
}

double
Bond::Value () const
{
  return value;
}

double
Bond::Remaining () const
{
  return remaining;
}

void
Bond::SetRemaining (double val)
{
  remaining = val;
}

void
Bond::Clear ()
{
  remaining = value;
}

void
Bond::AddRemaining (double diff)
{
  remaining += diff;
}

BondType
Bond::Type () const
{
  return type;
}

void
Bond::SetType (BondType bondType)
{
  type = bondType;
}

void
Bond::SetValue (double val)
{ 
  value = val;
}

double
Bond::Match (const Bond & other) const
{
  if (!haveMaps) {
    InitMaps ();
  }
  if (matchFunc.contains (type)) {
    return matchFunc[type] (*this, other);
  }
  return 0.0;
}

bool
Bond::Connect (Bond & other)
{
  if (!haveMaps) {
    InitMaps ();
  }
  if (connectFunc.contains (type)) {
    return connectFunc[type] (*this, other);
  }
  return false;
}

void
Bond::InitMaps ()
{
  matchFunc.clear ();
  matchFunc[Bond_None] = MatchBond_None;
  matchFunc[Bond_Ionic] = MatchBond_Ionic;
  matchFunc[Bond_Covalent] = MatchBond_Covalent;
  matchFunc[Bond_Metallic] = MatchBond_Metallic;
  matchFunc[Bond_Aromatic] = MatchBond_Aromatic;

  connectFunc.clear ();
  connectFunc[Bond_None] = ConnectBond_None;
  connectFunc[Bond_Ionic] = ConnectBond_Ionic;
  connectFunc[Bond_Covalent] = ConnectBond_Covalent;
  connectFunc[Bond_Metallic] = ConnectBond_Metallic;
  connectFunc[Bond_Aromatic] = ConnectBond_Aromatic;
  
  haveMaps = true;
}

} // namespace
