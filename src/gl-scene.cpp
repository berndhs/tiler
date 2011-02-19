#include "gl-scene.h"


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

#include <QDebug>

namespace tiler
{

GLScene::GLScene (QWidget * parent)
  :QGLWidget (parent)
{
  qDebug () << "GLScene::GLScene constructor";
  qDebug () << "         parent " << parent;    
  show ();
}

void
GLScene::Init ()
{
  initializeGL ();
}

int
GLScene::LoadShape (const QString & filename,
                    const QVector3D & position,
                          qreal size)
{
  Shape * s = new Shape;
  s->Load (filename);
  s->SetSize (size);
  s->SetPosition (position);
  shapes[s->Id()] = s;
  return s->Id();
}

void
GLScene::initializeGL ()
{
  qDebug () << "GLScene::initializeGL";
  glClearColor (red, green, blue, 0.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel (GL_FLAT);

}

void
GLScene::resizeGL (int width, int height)
{
  qDebug () << "GLScene::resizeGL " << width << height;
  glClearColor (red, green, blue, 0.0);
  glViewport(0, 0, GLint(width), GLint(height));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  glFrustum(-50.0, 50.0, -50.0, 50.0, 10, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void
GLScene::SetRGB (float r, float g, float b)
{
  red = r; 
  green = g; 
  blue = b; 
  qDebug () << "GLScene:: SetRGB " << red << green << blue;
}

void
GLScene::Resize ()
{
  resizeGL (width(), height());
}

void
GLScene::Paint ()
{
  paintGL ();
}

void
GLScene::paintGL ()
{
  qDebug () << "GLScene::paint GL";  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
  glMatrixMode(GL_MODELVIEW);
  gluLookAt (0,0,1, 0,0,10, 0,-1,0);
  Shape s0;
  s0.Load (":/shapes/turn.dat");
  s0.SetColor (Qt::yellow);
  s0.SetSize (4.0);
  s0.SetPosition (QVector3D(-5,0,15.1));
  glPushMatrix ();
  glRotatef (90, 0,0,1);
  s0.paintGL ();
  glPopMatrix ();
  int nShapes = shapes.count();
  qDebug () << "      shape count " << nShapes;
  QMap<int,Shape*>::iterator sit;
  for (sit=shapes.begin(); sit!=shapes.end(); sit++) {
    sit.value()->paintGL ();
  }
}

void
GLScene::MoveShape (int shapeId, const QVector3D & translate)
{
  if (shapes.contains (shapeId)) {
    Shape * s = shapes[shapeId];
    s->SetPosition (s->Position () + translate);
  }
}
  
void 
GLScene::RotateShape (int shapeId, qreal degrees, AxisType axis)
{
  if (shapes.contains (shapeId)) {
    Shape * s = shapes[shapeId];
    s->SetRotation (axis, s->Rotation (axis) + degrees);
  }
}

} // namespace
