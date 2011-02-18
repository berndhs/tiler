#ifndef TILER_SHAPE_H
#define TILER_SHAPE_H


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

#include <QVector3D>
#include <QList>
#include <QColor>

#include "poly.h"

namespace tiler
{
class Shape 
{
public:
  
  Shape ();

  bool Load (const QString & filename);

  void SetSize (qreal size);
  void SetPosition (const QVector3D & position);
  void SetColor (const QColor & color);

  QVector3D Position () const;
  qreal     Size () const;
  QColor    Color () const;

  int Id () { return theId; }

  void paintGL () const;

private:

  QList <Poly>  theSides;
  QVector3D     thePos;
  qreal         theSize;
  QColor        theColor;
  int           theId;

  static int    idCount;
  
};

} // namespace

#endif