#ifndef TILER_BOND_H
#define TILER_BOND_H


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

#include <QMap>

namespace tiler
{

enum BondType {
  Bond_None = 0,
  Bond_Ionic,
  Bond_Covalent,
  Bond_Metallic,
  Bond_Aromatic
};

class Bond
{
public:

  Bond ();
  Bond (const Bond & other);
  Bond (BondType type, double val = 0.0);

  int Id () { return theId; }

  BondType Type () const;
  void     SetType (BondType bondType);

  double Value () const;
  double Remaining () const;
  double Bonded () const;
  void   SetValue (double val);
  void   AddRemaining (double diff);

  double ConeAngle () const;
  void   SetConeAngle (double a);

  double MaxLength () const;
  void   SetMaxLength (double max);

  void   Clear ();

  double  Match (const Bond & other) const;
  bool    Connect (Bond & other);

private:

  typedef double (*MatchFunctionPtr) (const Bond &, const Bond &);
  typedef bool (*ConnectFunctionPtr) (Bond &, Bond &);

  typedef QMap <BondType, MatchFunctionPtr > MatchMapType;
  typedef QMap <BondType, ConnectFunctionPtr > ConnectMapType;
  

  int         theId;
  BondType    type;
  double      value;
  double      remaining;
  double      bonded;
  double      maxLength;
  double      coneAngle;

  static   bool            haveMaps;
  static   MatchMapType    matchFunc;
  static   ConnectMapType  connectFunc;

  static   void InitMaps ();

  static int    idCount;

};

} // namespace

#endif
