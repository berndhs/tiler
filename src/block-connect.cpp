#include "block-connect.h"


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

int BlockBond::nextId (2000);
int BlockConnect::nextId (5000);

/********
 * BlockBond functions
 ********/

BlockBond::BlockBond (Block * blk, Bond * bnd)
  :id (nextId++),
   block (blk),
   bond (bnd)
{
}

BlockBond::BlockBond (const BlockBond & other)
  :id (other.id),
   block (other.block),
   bond (other.bond)
{
}

BlockBond::BlockBond ()
  :id (-1),
   block (0),
   bond (0)
{
}

int
BlockBond::Id () const
{
  return id;
}

Block *
BlockBond::BlockPtr () const
{
  return block;
}

Bond *
BlockBond::BondPtr () const
{
  return bond;
}

/*********
 * BlockConnect functions
 ********/

BlockConnect::BlockConnect ()
  :id (-1),
   strength (0)
{
}
BlockConnect::BlockConnect (double valence)
  :id (nextId),
   strength (valence)
{
}

BlockConnect::BlockConnect (const BlockConnect & other)
  :id (other.id),
    strength (other.strength),
    bonds (other.bonds)
{
}

int
BlockConnect::Id ()
{
  return id;
}

double
BlockConnect::Valence ()
{
  return strength;
}

int
BlockConnect::AddBond (const BlockBond & bb)
{
  bonds[bb.Id()] = bb;
  return bb.Id();
}

int
BlockConnect::Count ()
{
  return bonds.count();
}

BlockBond
BlockConnect::Bond (int index)
{
  if (bonds.contains (index)) {
    return bonds[index];
  } else {
    return BlockBond ();
  }
}

void
BlockConnect::RemoveBond (int index)
{
  if (bonds.contains (index)) {
    bonds.remove (index);
  }
}

void
BlockConnect::paintGL ()
{
  QMap<int, BlockBond>::iterator  it;
  Block    * block;
  QVector3D  pos;
  glPushMatrix ();
  GLfloat oldWidth;
  glGetFloatv (GL_LINE_WIDTH, &oldWidth);
  glLineWidth (4.0);
  glColor3f (1.0, 0.0, 1.0);

  glBegin (GL_LINES);

  for (it=bonds.begin(); it!= bonds.end(); it++) {
    QVector3D pos;
    block = it->BlockPtr ();
    if (block) {
      pos = block->Position();
      glVertex3f (pos.x(), pos.y(), pos.z());
   }
  }

  glEnd ();

  glLineWidth (oldWidth);
  glPopMatrix ();
  
}

QList<int>
BlockConnect::BondList ()
{
  QList<int> list;
  QMap<int, BlockBond>::iterator  it;
  for (it=bonds.begin(); it!= bonds.end(); it++) {
    list.append (it.key());
  }
  return list;
}

/**********
 * BlockConnectMap functions
 ********/

bool
BlockConnectMap::contains (int key)
{
  return find (key) != end();
}

void
BlockConnectMap::Remove (int key)
{
  iterator chase = find (key);
  if (chase != end()) {
    erase (chase);
  }
}

} // namespace
