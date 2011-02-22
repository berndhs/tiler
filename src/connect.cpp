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

namespace tiler
{
BlockConn::BlockConn (Block * blk, Bond * thisbnd, Bond * otherbnd)
  :block (blk),
   thisBond (thisbnd),
   otherBond (otherbnd),
   broken (false)
{
}

BlockConn::BlockConn (const BlockConn & other)
  :block (other.block),
   thisBond (other.thisBond),
   otherBond (other.otherBond),
   broken (other.broken)
{
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
} // namespace
