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

int Bond::idCount (1000);

bool Bond::haveMaps (false);

Bond::MatchMapType     Bond::matchFunc;
Bond::ConnectMapType   Bond::connectFunc;

Bond::Bond ()
  :theId (idCount++),
   type(Bond_None),
   value (0),
   remaining (0),
   bonded (0),
   maxLength (1.0),
   coneAngle (45.0)
{
}

Bond::Bond (const Bond & other)
  :theId (other.theId),
   type (other.type),
   value (other.value),
   remaining (other.remaining),
   bonded (other.bonded),
   maxLength (other.maxLength),
   coneAngle (other.coneAngle)
{
}

Bond::Bond (BondType bondType, double val)
  :theId (idCount++),
   type(bondType),
   value (val),
   remaining (val),
   bonded (0),
   maxLength (1.0),
   coneAngle (45.0)
{
}

double
Bond::MaxLength () const
{
  return maxLength;
}

void
Bond::SetMaxLength (double max)
{
  maxLength = max;
}

double
Bond::Value () const
{
  return value;
}

double
Bond::ConeAngle () const
{
  return coneAngle;
}

void
Bond::SetConeAngle (double a)
{
  coneAngle = a;
}

double
Bond::Remaining () const
{
  return remaining;
}

double
Bond::Bonded () const
{
  return bonded;
}

void
Bond::Clear ()
{
  remaining = value;
  bonded = 0;
}

void
Bond::AddRemaining (double diff)
{
  remaining += diff;
  bonded -= diff;
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
  remaining = val;
  bonded = 0;
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
