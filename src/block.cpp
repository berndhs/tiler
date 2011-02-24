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
#include "block-connect.h"
#include <QtOpenGL>

using namespace std;

namespace tiler
{

int              Block::idCount (3001);

Block::Block (BlockConnectMap *conMap)
  :QObject (0),
   theId (idCount++),
   noBond (0),
   position (QVector3D (0,0,0)),
   orientation (QQuaternion()),
   color (Qt::red),
   scale (1.0),
   connections (conMap)
{
}

Block::Block (const Block & other)
  :QObject (0),
   theId (other.theId),
   noBond (other.noBond),
   bonds (other.bonds),
   position (other.position),
   orientation (other.orientation),
   color (other.color),
   scale (other.scale),
   connections (other.connections)
{
}

Bond &
Block::BondSite (const QVector3D & direction)
{
  int nb = bonds.count();
  QVector3D relativeDir = Orientation().rotatedVector(direction).normalized();
  for (int b=0; b<nb; b++) {
    qreal angle = acos (QVector3D::dotProduct (relativeDir, 
                                               bonds.at(b).direction)) 
                  * 180/M_PI;
    if ( angle < bonds.at(b).bond.ConeAngle()
         && !AlmostZero( bonds.at(b).bond.Remaining())) {
      return bonds[b].bond;
    }
  }
  return noBond;
}

void
Block::SetConnectMap (BlockConnectMap * map)
{
  connections = map;
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
  QColor edgeColor (Qt::green);
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
  set<int>::iterator it;
  for (it=ownConnections.begin(); it!=ownConnections.end(); it++) {
    if (BreakConnect (*it)) {
      ownConnections.erase (it);
      if (connections->contains (*it)) {
        connections->Remove (*it);
      }
    }
  }
  // then see which new bonds can form
  int nb = bonds.count();
  for (int b=0; b<nb; b++) {
    if (!AlmostZero (bonds.at(b).bond.Remaining())) {
      emit FreeBond (this, bonds.at(b).direction, &(bonds[b].bond));
    }
  }
}

void
Block::AddConnect (int connId)
{
  ownConnections.insert (connId);
}

void
Block::RemoveConnect (int connId)
{
  qDebug () << " RemoveConnect " << this << connId;
  ownConnections.erase (connId);
}

bool
Block::BreakConnect (int connId)
{
  if (connections->contains (connId)) {
    BlockConnect  conn ((*connections)[connId]);
    QList<int> bondList = conn.BondList();
    int  nb=bondList.count();
    bool breakIt (false);
    for (int b=0; b<nb; b++) {
      BlockBond bb = conn.Bond (bondList.at(b));
      Block * otherBlock = bb.BlockPtr();
      Bond * otherBond = bb.BondPtr();
      if (otherBlock && otherBlock != this) {
        qreal dist = (position - otherBlock->Position()).length();
        breakIt |= (otherBond && dist > otherBond->MaxLength ());
        qDebug () << " BreakConnect dist " << dist << " breakIt " << breakIt;
      }
    }
    if (breakIt) {
      BreakBonds (conn);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void
Block::BreakBonds (BlockConnect & conn)
{
  int cid = conn.Id();
  qDebug () << " BreakBonds " << cid;
  qreal strength = conn.Valence();
  QList<int> bondList = conn.BondList();
  int nb = bondList.count();
  for (int i=0; i<nb; i++) {
    BlockBond bb = conn.Bond (bondList.at(i));
    Block * block = bb.BlockPtr();
    Bond * bond = bb.BondPtr ();
    if (block && block != this) {
      block->RemoveConnect (cid);
    }
    if (bond) {
      bond->AddRemaining (strength);
    }
  }
}

} // namespace
