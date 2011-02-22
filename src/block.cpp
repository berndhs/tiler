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
#include "tiler-math.h"
#include "connect.h"
#include <QtOpenGL>

namespace tiler
{

int Block::idCount (1);

Block::Block ()
  :theId (idCount++),
   position (QVector3D (0,0,0)),
   orientation (QQuaternion()),
   color (Qt::red),
   scale (1.0)
{
  noBond.SetType (Bond_None);
}

Block::Block (const Block & other)
  :theId (other.theId),
   noBond (other.noBond),
   bonds (other.bonds),
   position (other.position),
   orientation (other.orientation),
   color (other.color),
   scale (other.scale)
{
  connections = other.connections;
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

void
Block::AddBond (const Bond & bond, const QVector3D & direction)
{
  BondSlot slot;
  slot.direction = direction.normalized();
  slot.bond = bond;
  slot.bond.Clear();
  bonds.append (slot);
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

qreal
Block::Scale () const
{
  return scale;
}

qreal
Block::Radius () const
{
  return shape.Radius() * scale;
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
Block::SetScale (qreal newScale)
{
  scale = newScale;
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
  default:
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
  glScalef (scale, scale, scale);
  QColor edgeColor (color.redF()/2.0, color.greenF()/2.0, color.blueF()/2.0);
  shape.paintGL (color, edgeColor);
  int nb = bonds.count();
  for (int b=0; b<nb; b++) {
    paintBondGL (bonds.at(b).direction * scale);
  }
  glPopMatrix ();
}

void
Block::paintBondGL (const QVector3D & direction)
{
  glPushMatrix ();
  GLfloat oldWidth;
  glGetFloatv (GL_LINE_WIDTH, &oldWidth);
  glLineWidth (3.0);
  glColor3f (1.0, 1.0, 0);
  glBegin (GL_LINES);
  glVertex3f (0,0,0);
  glVertex3f (direction.x(), direction.y(), direction.z());
  glEnd ();
  glLineWidth (oldWidth);
  glPopMatrix ();
}

void
Block::UpdateBonding ()
{
  qDebug () << "Block::UpdateBonding";
  // first see which bonds will break
  QSet <Connect*>::iterator sit;
  qreal mySize = Radius();
  bool brokeSomething (false);
  for (sit=connections.begin(); sit != connections.end(); sit++) {
    Connect *con = *sit;
    Block *other = con->OtherBlock();
    double distance = (position - other->Position()).length()
                      - mySize
                      - other->Radius();
    if (distance > con->OtherBond()->MaxLength() 
       || distance > con->ThisBond()->MaxLength()) {
      BreakConnect (con);
      delete (*sit);
      connections.erase (sit);
    }
  }
  int nb = bonds.count();
  // then see which bonds will form
  for (int b=0; b<nb; b++) {
    qDebug () << "    bond " << b << " remaining " << bonds.at(b).bond.Remaining();
    qDebug () << "   almost zero says " << almostZero (bonds.at(b).bond.Remaining());
    if (!almostZero (bonds.at(b).bond.Remaining())) {
      emit FreeBond (this, bonds.at(b).direction, &(bonds[b].bond));
    }
  }
}

void
Block::BreakConnect (Connect * con)
{
  qDebug () << " BreakConnect " << con;
}

} // namespace
