#ifndef TILER_GL_SCENE_H
#define TILER_GL_SCENE_H


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

#include <QGLWidget>
#include <QMap>
#include "shape.h"

namespace tiler
{

class GLScene : public QGLWidget 
{
Q_OBJECT

public:

  GLScene (QWidget * parent=0);

  void Init ();
  void SetRGB (float r, float g, float b);

  void Resize ();
  void Paint ();
  int LoadShape (const QString & filename,
                    const QVector3D & position,
                          qreal size);
  void MoveShape (int shapeId, const QVector3D & translate);

protected:

  void initializeGL ();
  void resizeGL (int width, int height);
  void paintGL ();
  
private:

  float    red;
  float    green;
  float    blue;

  QMap <int,Shape*>  shapes;

};

} // namespace

#endif