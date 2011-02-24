#include "poly.h"


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
#include <QDebug>

namespace tiler
{

Poly::Poly ()
{
}

void
Poly::AddPoint (const QVector3D & point)
{
  points += point;
}

void
Poly::AddPoint (qreal x, qreal y, qreal z)
{
  points += QVector3D (x,y,z);
}

int
Poly::Corners () const
{
  return points.count();
}

void
Poly::paintGL (const QColor & color, const QColor & edgeColor) const
{
  glPushMatrix ();
  int nc = points.count();
  glColor3f (color.redF(), color.greenF(), color.blueF());
  GLfloat oldWidth;
  glGetFloatv (GL_LINE_WIDTH, &oldWidth);
  glLineWidth (3.0);
  glBegin (GL_TRIANGLES);
  for (int c=0; c<nc; c++) {
    glVertex3f (points.at(c).x(), points.at(c).y(), points.at(c).z());
  }
  glEnd ();
  glColor3f (edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());
  glBegin (GL_LINES);
  for (int c=0; c<nc; c++) {
    glVertex3f (points.at(c).x(), points.at(c).y(), points.at(c).z());
  }
  glVertex3f (points.at(0).x(), points.at(0).y(), points.at(0).z());
  glEnd ();
  glFlush();
  glLineWidth (oldWidth);
  glPopMatrix ();
}

} // namespace
