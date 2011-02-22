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
  :theId (idCount++),
   radius (0.0)
{
}

qreal
Shape::Radius () const
{  
  return radius;
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
        qreal dist = QVector3D (x,y,z).length();
        if (dist > radius) {
          radius = dist;
        }
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
Shape::paintGL (const QColor & color, const QColor & edgeColor) const
{
  qDebug () << "Shape::paintGL id " << theId << " sides " << theSides.count();
  glPushMatrix ();
  for (int p=0; p<theSides.count(); p++) {
    theSides.at(p).paintGL (color, edgeColor);
  }
  glPopMatrix ();
}


} // namespace
