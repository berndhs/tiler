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

void
GLScene::initializeGL ()
{
  qDebug () << "GLScene::initializeGL";
  glClearColor (red, green, blue, 0.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel (GL_FLAT);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
#if 0
  glFrustum (-10, 10, -10, 10, 1.0, 1000.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glEnable(GL_MULTISAMPLE);
#endif

}

void
GLScene::resizeGL (int width, int height)
{
  qDebug () << "GLScene::resizeGL " << width << height;
  glClearColor (red, green, blue, 0.0);
  glViewport(0, 0, GLint(width), GLint(height));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
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
  glLoadIdentity ();    
  glColor3f(1.0, 1.0, 1.0);
  glRectf(-25.0, -25.0, 25.0, 25.0);

}

} // namespace
