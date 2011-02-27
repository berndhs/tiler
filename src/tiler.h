#ifndef TILER_H
#define TILER_H

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
#include <QMainWindow>
#include "ui_tiler.h"
#include "config-edit.h"
#include "helpview.h"
#include "active-bond.h"
#include "block-map.h"
#include <QList>
#include <QVector3D>
#include <QStringList>
#include <QStringListModel>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>

class QApplication;

using namespace deliberate;

namespace tiler 
{

class BoxInterface;
class BlockConnectMap;
class GLScene;

class Tiler : public QMainWindow
{
Q_OBJECT

public:

  Tiler (QWidget *parent=0);

  void  Init (QApplication &ap);
  bool  Run ();
  bool  Again ();

  void  AddConfigMessages (const QStringList & cm) 
           { configMessages.append (cm); }

  void closeEvent ( QCloseEvent *event);

private slots:

  void DoRun ();
  void Quit ();
  void Restart ();
  void EditSettings ();
  void SetSettings ();
  void About ();
  void License ();
  void Exiting ();
  void Recolor ();
  void StepShapes ();
  void BlockSelect (const QModelIndex & index);
  void BlockDone ();
  void PlusX ();
  void PlusY ();
  void PlusZ ();
  void MinusX ();
  void MinusY ();
  void MinusZ ();

  void HandleFreeBond (Block * block, const QVector3D & direction, Bond * bond);
  void paintConnectionsGL ();

private:

  void Connect ();
  void CloseCleanup ();
  void BlockMove (AxisType axis, qreal step);
  void BlockTurn (AxisType axis, qreal step);
  void CheckBox (const QString & msg);

  void FindNeighbors (Block           *block,
                      QVector3D        direction,
                      qreal            distance,
                      qreal            coneAngle,
                      ActiveBondList  &list);
  

  bool             initDone;
  QApplication    *app;
  Ui_TilerMain     mainUi;
  GLScene         *scene;
 
  ConfigEdit       configEdit;
  QStringList      configMessages;

  deliberate::HelpView        *helpView;
  bool             runAgain;

  QVector3D   eye;
  QVector3D   focus;
  
  QStringList          blockNames;
  QStringListModel    *blockModel;
  BlockMap             blocks;
  Block               *specialBlock;
  QColor               savedSpecialColor;

  qreal                moveStep;
  qreal                turnStep;

  BlockConnectMap     *blockConnections;
  BoxInterface    *boxIF;
  QDeclarativeContext *context;
  QDeclarativeEngine * engine;

};

} // namespace

#endif
