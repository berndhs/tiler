#include "shape.h"


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

#include <QFile>
#include <QTextStream>
#include <QtOpenGL>

#include <QDebug>

namespace tiler
{

int Shape::idCount (1);

Shape::Shape ()
  :theColor (Qt::green),
   theId (idCount++),
   thePos (QVector3D (0,0,0)),
   theRot (QVector3D (0,0,0))
{
}

bool
Shape::Load (const QString & filename)
{ 
  QFile file (filename);
  bool ok = file.open (QFile::ReadOnly);
  qDebug () << " Shape::Load " << filename << " open " << ok;
  if (ok) {
    QTextStream fstream (&file);
    int nSides;
    fstream >> nSides;
    for (int s=0; s<nSides; s++) {
      Poly poly;
      int nPoints;
      fstream >> nPoints;
      for (int p=0; p<nPoints; p++) {
        qreal x,y,z;
        fstream >> x;
        fstream >> y;
        fstream >> z;
        poly.AddPoint (x,y,z);
      }
      if (nPoints > 2) {
        theSides += poly;
      }
    }
    ok = theSides.count() > 0;
  }
  return ok;
}


void
Shape::SetSize (qreal size)
{
  theSize = size;
}

void
Shape::SetPosition (const QVector3D & position)
{
  thePos = position;
}

void
Shape::SetColor (const QColor & color)
{
  theColor = color;
}

QVector3D
Shape::Position () const
{
  return thePos;
}

qreal
Shape::Size () const
{
  return theSize;
}

qreal
Shape::Rotation (AxisType axis) const
{
  switch (axis) {
  case Axis_X:
    return theRot.x();
    break;
  case Axis_Y:
    return theRot.y();
    break;
  case Axis_Z:
    return theRot.z();
    break;
  default:
    return 0.0;
  }
}

void
Shape::SetRotation (AxisType axis, qreal degrees)
{
  switch (axis) {
  case Axis_X:
    theRot.setX(degrees);
    break;
  case Axis_Y:
    theRot.setY(degrees);
    break;
  case Axis_Z:
    theRot.setZ(degrees);
    break;
  default:
    break;
  }
}

QColor
Shape::Color () const
{
  return theColor;
}

void
Shape::paintGL () const
{
  qDebug () << "Shape::paintGL id " << theId << " sides " << theSides.count();
  qDebug () << "     position " << thePos;
  glPushMatrix ();
  glColor3f (theColor.redF(), theColor.greenF(), theColor.blueF());
  glTranslatef (thePos.x(),thePos.y(),thePos.z());
  glRotatef (theRot.x(), 1,0,0);
  glRotatef (theRot.y(), 0,1,0);
  glRotatef (theRot.z(), 0,0,1);
  glScalef (theSize, theSize, theSize);
  for (int p=0; p<theSides.count(); p++) {
    theSides.at(p).paintGL ();
  }
  glPopMatrix ();
}


} // namespace
