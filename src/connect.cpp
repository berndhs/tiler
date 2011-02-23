#include "connect.h"


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
#include "block.h"
#include "bond.h"
#include <QtOpenGL>

namespace tiler
{
BlockConn::BlockConn (Block * blk, Bond * thisbnd, Bond * otherbnd)
  :block (blk),
   thisBond (thisbnd),
   otherBond (otherbnd),
   broken (false)
{
  qDebug () << " create BlockConn  " << this << block << thisBond << otherBond;
}

BlockConn::BlockConn (const BlockConn & other)
  :block (other.block),
   thisBond (other.thisBond),
   otherBond (other.otherBond),
   broken (other.broken)
{
  qDebug () << " copy BlockConn  " << this << block << thisBond << otherBond;
}

Block *
BlockConn::OtherBlock ()
{
  return block;
}

Bond *
BlockConn::OtherBond ()
{ 
  return otherBond;
}

Bond *
BlockConn::ThisBond ()
{ 
  return otherBond;
}

bool
BlockConn::Broken ()
{
  return broken;
}

void
BlockConn::Break ()
{
  broken = true;
}

void
BlockConn::paintGL (const QVector3D & origin)
{
  if (broken) {
    return;
  }
  QVector3D line = origin - block->Position();
  //line *= 2.5;

  glPushMatrix ();
  GLfloat oldWidth;
  glGetFloatv (GL_LINE_WIDTH, &oldWidth);
  glLineWidth (5.0);
  glColor3f (1.0, 0.0, 1.0);
  glBegin (GL_LINES);
  glVertex3f (0,0,0);
  glVertex3f (line.x(), line.y(), line.z());
  glEnd ();
  glLineWidth (oldWidth);
  glPopMatrix ();
  
}

} // namespace
