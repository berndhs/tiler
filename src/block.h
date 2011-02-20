#ifndef TILER_BLOCK_H
#define TILER_BLOCK_H


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

#include "tiler-types.h"
#include "bond.h"
#include "shape.h"
#include <QVector3D>
#include <QQuaternion>

namespace tiler
{

class Block
{
public:

  Block ();
  Block (const Block & other);

  int Id () { return theId; }

  Bond & BondSite (const QVector3D & direction);

  QVector3D Position () const;
  QQuaternion Orientation () const;

  void SetPosition (const QVector3D & newPos);
  void SetOrientation (const QQuaternion & newOrient);

  void Move (const QVector3D & delta);
  void Rotate (AxisType axis, float degrees);
  void Rotate (const QVector3D & axis, float degrees);

  void SetShape (const Shape & s);
  void SetShape (const QString & filename);

  void paintGL ();

private:

  typedef struct {
            QVector3D   direction;
            Bond        bond;
         } BondSlot;

  typedef QList <BondSlot> BondList;

  int           theId;
  Bond          noBond;
  BondList      bonds;

  QVector3D     position;
  QQuaternion   orientation;

  Shape         shape;

  static int    idCount;
};

} // namespace

#endif
