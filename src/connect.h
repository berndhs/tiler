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

class QVector3D;

namespace tiler
{
class Block;
class Bond;

class BlockConn
{
public:

  BlockConn (Block * otherblk, Bond * thisbnd, Bond * otherbnd);
  BlockConn (const BlockConn & other);

  Block * OtherBlock ();
  Bond  * OtherBond ();
  Bond  * ThisBond ();

  bool    Broken ();
  void    Break ();

  void    paintGL (const QVector3D & origin);

private:

  Block   *block;
  Bond    *thisBond;
  Bond    *otherBond;

  bool     broken;
};

} // namespace


#endif
