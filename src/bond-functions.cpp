#include "bond-functions.h"


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
#include "bond.h"
#include "tiler-math.h"

namespace tiler
{

double
MatchBond_None (const Bond & thisEnd, const Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return 0.0;
}

double
MatchBond_Ionic (const Bond & thisEnd, const Bond & otherEnd)
{
  if (thisEnd.Type() == otherEnd.Type() 
      && thisEnd.Type() == Bond_Ionic) {
    double v1 = thisEnd.Remaining ();
    double v2 = otherEnd.Remaining ();
    if (SameSign (v1,v2)) {
      return - qMax (qAbs (v1), qAbs (v2));
    } else {
      return qMin (qAbs (v1), qAbs (v2));
    }
  }
  return 0.0;
}

double
MatchBond_Covalent (const Bond & thisEnd, const Bond & otherEnd)
{
  if (thisEnd.Type() == otherEnd.Type()
      && thisEnd.Type() == Bond_Covalent) {
    double v1 = thisEnd.Remaining ();
    double v2 = otherEnd.Remaining ();
    if (SameSign (v1,v2)) {
      return - qMax (qAbs (v1), qAbs (v2));
    } else {
      return qMin (qAbs (v1), qAbs (v2));
    }
  }
  return 0.0;
}

double
MatchBond_Metallic (const Bond & thisEnd, const Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return 0.0;
}

double
MatchBond_Aromatic (const Bond & thisEnd, const Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return 0.0;
}

bool
ConnectBond_None (Bond & thisEnd, Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return false;
}

bool
ConnectBond_Ionic (Bond & thisEnd, Bond & otherEnd)
{
  double exchange = qMin (qAbs (thisEnd.Remaining()), 
                          qAbs (otherEnd.Remaining()));
  Bond * from (0);
  Bond * to (0);
  if (thisEnd.Remaining() < 0.0) {
    from = &otherEnd;
    to = &thisEnd;
  } else {
    from = &thisEnd;
    to = &otherEnd;
  }
  to->AddRemaining (exchange);
  from->AddRemaining (-exchange);
  return true;
}

bool
ConnectBond_Covalent (Bond & thisEnd, Bond & otherEnd)
{
  double exchange = qMin (qAbs (thisEnd.Remaining()), 
                          qAbs (otherEnd.Remaining()));
  Bond * from (0);
  Bond * to (0);
  if (thisEnd.Remaining() < 0.0) {
    from = &otherEnd;
    to = &thisEnd;
  } else {
    from = &thisEnd;
    to = &otherEnd;
  }
  to->AddRemaining (exchange);
  from->AddRemaining (-exchange);
  return true;
}

bool
ConnectBond_Metallic (Bond & thisEnd, Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return false;
}

bool
ConnectBond_Aromatic (Bond & thisEnd, Bond & otherEnd)
{
  Q_UNUSED (thisEnd)
  Q_UNUSED (otherEnd)
  return false;
}

} // namespace
