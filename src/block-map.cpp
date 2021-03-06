#include "block-map.h"


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

BlockMap::BlockMap ()
{
  Clear ();
}

void
BlockMap::Insert (int key, Block * block)
{
  if (block == 0) {
    return;
  }
  if (blocks.contains (key)) {
    blocks.remove (key);
    keys.remove (block);
    RemoveDims (block);
  }
  blocks[key] = block;
  keys[block] = key;
  InsertDims (block);
}

void
BlockMap::Remove (Block * block)
{
  int key;
  if (keys.contains(block)) {
    key = keys[block];
    keys.remove (block);
    blocks.remove (key);
  }
  RemoveDims (block);
}

bool
BlockMap::Contains (int key) const
{
  return blocks.contains (key);
}

bool
BlockMap::Contains (Block * block) const
{
  return keys.contains (block);
}

Block*
BlockMap::GetBlock (int key)
{
  if (blocks.contains(key)) {
    return blocks[key];
  } else {
    return 0;
  }
}

void
BlockMap::BlockMoved (Block * block)
{
  RemoveDims (block);
  InsertDims (block);
}

void
BlockMap::Clear ()
{
  blocks.clear ();
  keys.clear ();
  xIndex.clear ();
  yIndex.clear ();
  zIndex.clear ();
}

void
BlockMap::RemoveDims (Block * block)
{
  RemoveDim (xIndex, block);
  RemoveDim (yIndex, block);
  RemoveDim (xIndex, block);
}

void
BlockMap::RemoveDim (DimMapType & index, Block * block)
{
  DimMapType::iterator it;
  for (it=index.begin(); it!= index.end(); it++) {
    if (it->second == block) {
      index.erase (it);
    }
  }
}

void
BlockMap::InsertDims (Block * block)
{
  if (!block) {
    return;
  }
  InsertDim (xIndex, block->Position().x(), block);
  InsertDim (yIndex, block->Position().y(), block);
  InsertDim (zIndex, block->Position().z(), block);
}

void
BlockMap::InsertDim (DimMapType & index, qreal key, Block * block)
{
  if (!block) {
    return;
  }
  QVector3D pos (block->Position());
  index.insert (DimPair (key, block));
}

void
BlockMap::FindNeighbors (const QVector3D    & origin,
                               qreal          maxDistance,
                               BlockPtrSet  & results) const
{
  results.clear ();
  BlockPtrSet  partial;
  FindNeighbors (xIndex, origin.x() - maxDistance, origin.x() + maxDistance,
                 partial);
  BlockPtrSet common (partial);
  FindNeighbors (yIndex, origin.y() - maxDistance, origin.y() + maxDistance,
                 partial);
  common.intersect (partial);
  
  FindNeighbors (zIndex, origin.z() - maxDistance, origin.z() + maxDistance,
                 partial);
  common.intersect (partial);
  BlockPtrSet::iterator it;
  for (it=common.begin(); it!=common.end(); it++) {
    if ((origin - (*it)->Position()).length() < maxDistance) {
      results.insert (*it);
    }
  }
}

void
BlockMap::FindNeighbors (const DimMapType  & index,
                         qreal         min,
                         qreal         max,
                         BlockPtrSet & result) const
{
  result.clear();
  DimMapType::const_iterator lower = index.lower_bound (min);
  DimMapType::const_iterator chase;
  for (chase = lower; chase != index.end(); chase++) {
    if (chase->first > max) {
      break;
    }
    result.insert (chase->second);
  }
}

} // namespace
