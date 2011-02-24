#ifndef TILER_CONNECT_H
#define TILER_CONNECT_H


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

#include <QList>
#include <QPair>
#include <QMap>
#include "block.h"
#include "bond.h"

class QVector3D;

namespace tiler
{

class BlockBond 
{
public:

  BlockBond (Block * blk, Bond * bnd);
  BlockBond (const BlockBond & other);
  BlockBond ();

  int      Id () const;
  Block  * BlockPtr () const;
  Bond   * BondPtr () const;

private:

  int      id;
  Block   *block;
  Bond    *bond;

  static int nextId;

};

class BlockConnect
{
public:

  BlockConnect ();
  BlockConnect (double valence);
  BlockConnect (const BlockConnect & other);

  int        Id ();

  double     Valence ();

  int        AddBond (const BlockBond & bb);  // return index
  int        Count ();
  QList<int> BondList ();
  BlockBond  Bond (int index);
  void       RemoveBond (int index);

  void       Break ();

  void       paintGL ();

private:

  int                    id;
  double                 strength;
  QMap <int, BlockBond>  bonds;

  static int nextId;

};

//typedef QMap <int, BlockConnect> BlockConnectMap;

class BlockConnectMap : public QMap <int, BlockConnect>
{
public:

  bool contains (int key);
  void Remove (int key);
};

} // namespace


#endif
