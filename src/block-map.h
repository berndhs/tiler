#ifndef TILER_BLOCK_MAP_H
#define TILER_BLOCK_MAP_H


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
#include <QVector3D>
#include <QList>
#include <QMap>
#include <map>

using namespace std;

namespace tiler
{


typedef QSet<Block*>  BlockPtrSet;

class BlockMap
{
public:

  BlockMap ();

  void   Insert (int key, Block * block);
  void   Remove (Block * block);
  bool   Contains (int key) const;
  bool   Contains (Block * key) const;
  Block* GetBlock (int key) ;
  void   BlockMoved (Block * block);

  void   Clear ();

  void FindNeighbors (const QVector3D     & origin,
                            qreal           maxDistance,
                            BlockPtrSet   & results) const;

private:

  typedef QMap<int, Block*>          MapType;
  typedef QMap<Block*, int>          ReverseMapType;
  typedef multimap<qreal, Block*>    DimMapType;
  typedef pair <qreal, Block*>       DimPair;

  void RemoveDims (Block * block);
  void RemoveDim (DimMapType & index, Block * block);
  void InsertDims (Block * block);
  void InsertDim (DimMapType & index, qreal key, Block * block);
  void FindNeighbors (const DimMapType & index, 
                      qreal min,
                      qreal max,
                      BlockPtrSet & result) const;

  MapType              blocks;
  ReverseMapType       keys;

  DimMapType           xIndex;
  DimMapType           yIndex;
  DimMapType           zIndex;
  
};

} // namespace

#endif
