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

using namespace deliberate;

namespace tiler
{

Tiler::Tiler (QWidget *parent)
  :QMainWindow (parent),
   initDone (false),
   app (0),
   configEdit (this),
   helpView (0),
   runAgain (false)
{
  mainUi.setupUi (this);
  mainUi.actionRestart->setEnabled (false);
  helpView = new HelpView (this);
  Connect ();
}

void
Tiler::Init (QApplication &ap)
{
  app = &ap;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  Settings().sync();
  shapes.append (mainUi.scene->LoadShape (":/shapes/square.dat",
                             QVector3D (10,10,12),
                             4.0));
  shapes.append (mainUi.scene->LoadShape (":/shapes/turn.dat",
                             QVector3D (20,20,12),
                             4.0));
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
  show ();
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
  mainUi.scene->Resize ();
  mainUi.scene->update ();
}

void
Tiler::StepShapes ()
{
  for (int s=0; s<shapes.count(); s++) {
    mainUi.scene->MoveShape (shapes.at(s), QVector3D (-1,-1,0.2));
    mainUi.scene->RotateShape (shapes.at(s), 15, Axis_Y);
  }
  mainUi.scene->update ();
}


} // namespace

