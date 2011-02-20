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
  :QGLWidget (parent),
   eyeX (0),
   eyeY (0),
   eyeZ (1),
   focusX (0),
   focusY (0),
   focusZ (10)
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
GLScene::AddBlock (Block *b)
{
  if (b) {
    blocks[b->Id()] = b;
    return b->Id();
  }
  return -1;
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
  glLoadIdentity ();
  gluLookAt (eyeX,eyeY,eyeZ, focusX,focusY,focusZ, 0,-1,0);
  glPushMatrix ();
  glRotatef (90, 0,0,1);
  glPopMatrix ();
  int nBlocks = blocks.count();
  qDebug () << "      block count " << nBlocks;
  QMap<int,Block*>::iterator bit;
  for (bit=blocks.begin(); bit!=blocks.end(); bit++) {
    bit.value()->paintGL ();
  }
  glFlush ();
}


QVector3D
GLScene::Eye () const
{
  return QVector3D (eyeX, eyeY, eyeZ);
}

QVector3D
GLScene::Focus () const
{ 
  return QVector3D (focusX, focusY, focusZ);
}

void
GLScene::SetEye (const QVector3D & newEye)
{
  eyeX = newEye.x();
  eyeY = newEye.y();
  eyeZ = newEye.z();
}

void
GLScene::SetFocus (const QVector3D & newFocus)
{
  focusX = newFocus.x();
  focusY = newFocus.y();
  focusZ = newFocus.z();
}
} // namespace
