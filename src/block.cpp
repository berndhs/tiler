#include "block.h"


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

#include <QtOpenGL>

namespace tiler
{

int Block::idCount (1);

Block::Block ()
  :theId (idCount++),
   position (QVector3D (0,0,0)),
   orientation (QQuaternion()),
   color (Qt::red)
{
  noBond.SetType (Bond_None);
}

Block::Block (const Block & other)
  :theId (other.theId),
   noBond (other.noBond),
   bonds (other.bonds),
   position (other.position),
   orientation (other.orientation),
   color (other.color)
{
}

Bond &
Block::BondSite (const QVector3D & direction)
{
  int nb = bonds.count();
  for (int b=0; b<nb; b++) {
    if ( qFuzzyCompare (bonds.at(b).direction, direction) ) {
      return bonds[b].bond;
    }
  }
  return noBond;
}

QVector3D
Block::Position () const
{
  return position;
}

QQuaternion
Block::Orientation () const
{
  return orientation;
}

QColor
Block::Color () const
{
  return color;
}

void
Block::SetColor (const QColor & col)
{
  color = col;
}

void
Block::SetPosition (const QVector3D & newPos)
{
  position = newPos;
}

void
Block::SetOrientation (const QQuaternion & newOrient)
{
  orientation = newOrient;
  orientation.normalize ();
}

void
Block::Move (const QVector3D & delta)
{
  position += delta;
}

void
Block::Rotate (AxisType axis, float degrees)
{
  switch (axis) {
  case Axis_X:
    orientation *= QQuaternion::fromAxisAndAngle (1,0,0,degrees);
    break;
  case Axis_Y:
    orientation *= QQuaternion::fromAxisAndAngle (0,1,0,degrees);
    break;
  case Axis_Z:
    orientation *= QQuaternion::fromAxisAndAngle (0,0,1,degrees);
    break;
  }
  orientation.normalize ();
}

void
Block::Rotate (const QVector3D & axis, float degrees)
{
  orientation *= QQuaternion::fromAxisAndAngle (axis, degrees);
  orientation.normalize ();
}

void
Block::SetShape (const Shape & s)
{
  shape = s;
}

void
Block::SetShape (const QString & filename)
{
  shape.Load (filename);
}

void
Block::paintGL ()
{
  glPushMatrix ();
  glColor3f (color.redF(), color.greenF(), color.blueF());
  glTranslatef (position.x(), position.y(), position.z());
  qreal w = orientation.scalar ();
  qreal angle = 2* acos (w) * (180.0 / M_PI);
  qreal s = sqrt (1.0 - w*w);
  qreal rx(1.0), ry(0), rz(0);
  if (s > 0.001) {
    rx = orientation.x() / s;
    ry = orientation.y() / s;
    rz = orientation.z() / s;
  }
  glRotatef (angle, rx, ry, rz);
  shape.paintGL ();
  glPopMatrix ();
}

} // namespace
