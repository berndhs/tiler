#include "tiler.h"

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

#include <QApplication>
#include "deliberate.h"
#include "version.h"
#include "helpview.h"
#include <QSize>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QCursor>

#include "gl-scene.h"
#include "block.h"
#include "bond.h"
#include "tiler-math.h"
#include "block-connect.h"
#include <QtGlobal>

#include <cmath>

using namespace deliberate;

namespace tiler
{

Tiler::Tiler (QWidget *parent)
  :QMainWindow (parent),
   initDone (false),
   app (0),
   configEdit (this),
   helpView (0),
   runAgain (false),
   blockModel (0),
   specialBlock (0),
   moveStep (0.2),
   turnStep (M_PI / 8.0)
{
  mainUi.setupUi (this);
  mainUi.actionRestart->setEnabled (false);
  eye = mainUi.scene->Eye();
  focus = mainUi.scene->Focus();
  mainUi.eyeX->setValue (eye.x());
  mainUi.eyeY->setValue (eye.y());
  mainUi.eyeZ->setValue (eye.z());
  mainUi.focusX->setValue (focus.x());
  mainUi.focusY->setValue (focus.y());
  mainUi.focusZ->setValue (focus.z());
  helpView = new HelpView (this);
  blockModel = new QStringListModel (this);
  mainUi.blockList->setModel (blockModel);
  mainUi.viewControl->show();
  mainUi.blockControl->hide ();
  blockConnections = new BlockConnectMap;
  connect (mainUi.scene, SIGNAL (paintConnectionsGL ()),
           this, SLOT (paintConnectionsGL ()));
  Connect ();
}

void
Tiler::Init (QApplication &ap)
{
  app = &ap;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  Settings().sync();
  initDone = true;
}

bool
Tiler::Again ()
{
  bool again = runAgain;
  runAgain = false;
  return again;
}

bool
Tiler::Run ()
{
  runAgain = false;
  if (!initDone) {
    Quit ();
    return false;
  }
  qDebug () << " Start Tiler";
  QSize defaultSize = size();
  QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
  resize (newsize);
  Settings().setValue ("sizes/main",newsize);

  moveStep = Settings().value ("blockcontrol/movestep",moveStep).toReal ();
  Settings().setValue ("blockcontrol/movestep",moveStep);
  turnStep = Settings().value ("blockcontrol/turnstep",turnStep).toReal ();
  Settings().setValue ("blockcontrol/turnstep",turnStep);

  show ();
  Block * blk = new Block (blockConnections);
  blk->SetShape (":/shapes/square.dat");
  blk->SetPosition (QVector3D (10,10,20));
  blk->SetScale (20.0);
  Bond bond1 (Bond_Covalent, 2.0);
  bond1.SetMaxLength (15.0);
  blk->AddBond (bond1, QVector3D (1,1,0));
  blk->AddBond (bond1, QVector3D (0,1,0));
  mainUi.scene->AddBlock (blk);
  blocks.Insert (blk->Id(), blk);
  blockNames.append (QString::number(blk->Id()));
  connect (blk, SIGNAL (FreeBond (Block *, const QVector3D &, Bond *)),
           this, SLOT (HandleFreeBond (Block *, const QVector3D &, Bond *)));

  blk = new Block (blockConnections);
  blk->SetShape (":/shapes/square.dat");
  blk->SetPosition (QVector3D (10,30,20));
  blk->SetScale (20.0);
  Bond bond2 (Bond_Covalent, -2.0);
  bond2.SetMaxLength (15.0);
  blk->AddBond (bond2, QVector3D (1,1,0));
  blk->AddBond (bond2, QVector3D (0,1,0));
  blk->Rotate (Axis_Z, 170);
  mainUi.scene->AddBlock (blk);
  blocks.Insert (blk->Id(), blk);
  blockNames.append (QString::number(blk->Id()));

  blockModel->setStringList (blockNames);
  connect (blk, SIGNAL (FreeBond (Block *, const QVector3D &, Bond *)),
           this, SLOT (HandleFreeBond (Block *, const QVector3D &, Bond *)));
  return true;
}

void
Tiler::Connect ()
{
  connect (mainUi.actionQuit, SIGNAL (triggered()), 
           this, SLOT (Quit()));
  connect (mainUi.actionSettings, SIGNAL (triggered()),
           this, SLOT (EditSettings()));
  connect (mainUi.actionAbout, SIGNAL (triggered()),
           this, SLOT (About ()));
  connect (mainUi.actionLicense, SIGNAL (triggered()),
           this, SLOT (License ()));
  connect (mainUi.actionRestart, SIGNAL (triggered()),
           this, SLOT (Restart ()));
  connect (mainUi.redrawButton, SIGNAL (clicked()),
           this, SLOT (Recolor()));
  connect (mainUi.stepButton, SIGNAL (clicked ()),
           this, SLOT (StepShapes ()));
  connect (mainUi.blockList, SIGNAL (clicked (const QModelIndex &)),
           this, SLOT (BlockSelect (const QModelIndex &)));
  connect (mainUi.doneBlockButton, SIGNAL (clicked()),
           this, SLOT (BlockDone()));
  connect (mainUi.plusXButton, SIGNAL (clicked ()),
           this, SLOT (PlusX ()));
  connect (mainUi.minusXButton, SIGNAL (clicked ()),
           this, SLOT (MinusX ()));
  connect (mainUi.plusYButton, SIGNAL (clicked ()),
           this, SLOT (PlusY ()));
  connect (mainUi.minusYButton, SIGNAL (clicked ()),
           this, SLOT (MinusY ()));
  connect (mainUi.plusZButton, SIGNAL (clicked ()),
           this, SLOT (PlusZ ()));
  connect (mainUi.minusZButton, SIGNAL (clicked ()),
           this, SLOT (MinusZ ()));
}

void
Tiler::Restart ()
{
  qDebug () << " restart called ";
  runAgain = true;
  Quit ();
}


void
Tiler::Quit ()
{
  CloseCleanup ();
  if (app) {
    app->quit();
  }
}

void
Tiler::closeEvent (QCloseEvent *event)
{
  Q_UNUSED (event)
  CloseCleanup ();
}

void
Tiler::CloseCleanup ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
}

void
Tiler::EditSettings ()
{
  configEdit.Exec ();
  SetSettings ();
}

void
Tiler::SetSettings ()
{
  Settings().sync ();
}

void
Tiler::About ()
{
  QString version (deliberate::ProgramVersion::Version());
  QStringList messages;
  messages.append (version);
  messages.append (configMessages);

  QMessageBox  box;
  box.setText (version);
  box.setDetailedText (messages.join ("\n"));
  QTimer::singleShot (30000, &box, SLOT (accept()));
  box.exec ();
}

void
Tiler::Exiting ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
}

void
Tiler::License ()
{
  if (helpView) {
    helpView->Show ("qrc:/help/LICENSE.txt");
  }
}

void
Tiler::Recolor ()
{
  qDebug () << " Recolor " << mainUi.redBox->value()
                           << mainUi.greenBox->value()
                           << mainUi.blueBox->value() ;
  mainUi.scene->SetRGB (mainUi.redBox->value(),
                        mainUi.greenBox->value(),
                        mainUi.blueBox->value());
  eye.setX (mainUi.eyeX->value());
  eye.setY (mainUi.eyeY->value());
  eye.setZ (mainUi.eyeZ->value());
  focus.setX (mainUi.focusX->value());
  focus.setY (mainUi.focusY->value());
  focus.setZ (mainUi.focusZ->value());
  mainUi.scene->SetEye (eye);
  mainUi.scene->SetFocus (focus);
  mainUi.scene->Resize ();
  mainUi.scene->update ();
}

void
Tiler::StepShapes ()
{
  mainUi.scene->update ();
}

void
Tiler::BlockSelect (const QModelIndex & index)
{
  qDebug () << " selected index " << index;
  qDebug () << "   data " << blockModel->data (index, 0);
  int blockid = blockModel->data(index,0).toInt();
  if (blocks.Contains (blockid)) {
    Block * newSpecial = blocks.GetBlock(blockid);
    if (specialBlock) {
      specialBlock->SetColor (savedSpecialColor);
    }
    savedSpecialColor = newSpecial->Color();
    newSpecial->SetColor (Qt::white);
    specialBlock = newSpecial;
    mainUi.scene->update ();
    mainUi.viewControl->hide();
    mainUi.blockControl->show ();
  }
}

void
Tiler::BlockDone ()
{
  if (specialBlock) {
    specialBlock->SetColor (savedSpecialColor);
    specialBlock = 0;
  }
  mainUi.blockControl->hide ();
  mainUi.viewControl->show ();
}

void
Tiler::BlockMove (AxisType axis, qreal step)
{
  if (specialBlock) {
    float dx (0.0), dy (0.0), dz (0.0);
    switch (axis) {
    case Axis_X:
      dx = step;
      break;
    case Axis_Y:
      dy = step;
      break;
    case Axis_Z:
      dz = step;
      break;
    default:
      break;
    }
    specialBlock->Move (QVector3D (dx,dy,dz));
    blocks.BlockMoved (specialBlock);
    specialBlock->UpdateBonding ();
    mainUi.scene->update ();
  }
}

void
Tiler::BlockTurn (AxisType axis, qreal step)
{
  if (specialBlock) {
    specialBlock->Rotate (axis, step);
    specialBlock->UpdateBonding();
    mainUi.scene->update();
  }
}

void
Tiler::PlusX ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_X, moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_X, turnStep);
  }
}

void
Tiler::MinusX ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_X, - moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_X, - turnStep);
  }
}

void
Tiler::PlusY ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_Y, moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_Y, turnStep);
  }
}

void
Tiler::MinusY ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_Y, - moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_Y, - turnStep);
  }
}

void
Tiler::PlusZ ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_Z, moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_Z, turnStep);
  }
}

void
Tiler::MinusZ ()
{
  if (mainUi.moveCheck->isChecked ()) {
    BlockMove (Axis_Z, - moveStep);
  } else if (mainUi.turnCheck->isChecked ()) {
    BlockTurn (Axis_Z, - turnStep);
  }
}


void 
Tiler::HandleFreeBond (Block * block, const QVector3D & direction, Bond * bond)
{
  qDebug () << "Tiler::HandleFreeBond " << block << bond << direction;
  ActiveBondList activeBonds;
  FindNeighbors (block, 
                 block->Orientation().rotatedVector(direction), 
                 bond->MaxLength() + block->Radius(),
                 bond->ConeAngle(),
                 activeBonds);
  BondType  type = bond->Type();
  qreal remaining = bond->Remaining();
  int   sense = bond->Sense();
  if (AlmostZero (remaining)) {
    return;
  }
  qDebug () << " Number of Neighbor bonds " << activeBonds.count ();
  ActiveBondList::iterator ait;
  for (ait=activeBonds.begin(); ait!=activeBonds.end(); ait++) {
    // see if it is worth bonding
    Block * otherBlock  = ait->BlockPtr();
    Bond  * otherBond   = ait->BondPtr();
    QVector3D reverseDir = ait->Direction();
    qDebug () << "     try bonding with bond " << otherBond->Id() 
                                     << " on " << otherBlock->Id();
    qreal otherRemain = otherBond->Remaining();
    if (type != otherBond->Type()
        || AlmostZero (otherRemain)
        || SameSign (sense, otherBond->Sense())) {
      qDebug () << "      Bond used up ";
      continue;
    }
    qreal reduce = qMin (qAbs (remaining), qAbs (otherRemain));
    bond->AddRemaining (-reduce);
    otherBond->AddRemaining (-reduce);
    BlockConnect conn (qAbs (reduce));
    conn.AddBond (BlockBond (block,bond));
    conn.AddBond (BlockBond (otherBlock, otherBond));
    blockConnections->insert (conn.Id(),conn);
    block->AddConnect (conn.Id());
    otherBlock->AddConnect (conn.Id());
    qDebug () << "   connected block " << block->Id() << bond->Id()
              << " to other " << otherBlock->Id() << otherBond->Id()
              << " force " << reduce
              << " was " << remaining << " and " << otherRemain
              << " is " << bond->Remaining() << " and " 
              << otherBond->Remaining();
  }
}

void
Tiler::FindNeighbors (Block           *block,
                      QVector3D        direction,
                      qreal            distance,
                      qreal            coneAngle,
                      ActiveBondList  &list)
{
  list.clear ();
  BlockPtrSet  neighbors;
  blocks.FindNeighbors (block->Position(), distance, neighbors);
  qDebug () << " neigbor set size " << neighbors.count();
  neighbors.remove (block);
  BlockPtrSet::iterator it;
  QVector3D dir = direction.normalized();
  QVector3D pos = block->Position();
  for (it=neighbors.begin(); it!=neighbors.end(); it++) {
    Block * neighbor = *it;
    QVector3D nDir = neighbor->Position() - pos;
    qDebug () << "       nDir " << nDir;
    nDir.normalize();
    qDebug () << "     normal " << nDir;
    double angle = acos (QVector3D::dotProduct (dir, nDir)) * 180.0/M_PI;
    bool withinCone = angle < coneAngle;
    if (withinCone) {
      Bond & otherBond = neighbor->BondSite (-nDir);
      qDebug () << "   otherBond " << otherBond.Type() << " from nDir " << nDir;
      list.append (ActiveBond (neighbor, nDir, &otherBond));
    }
  }
}

void
Tiler::paintConnectionsGL ()
{
  BlockConnectMap::iterator it;
  glPushMatrix ();
  for (it=blockConnections->begin(); it!= blockConnections->end(); it++) {
    it->paintGL ();
  }
  glPopMatrix ();
}

} // namespace

