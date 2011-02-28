#include "box-if.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

namespace tiler
{

BoxInterface::BoxInterface (QObject *parent)
  :QObject(parent),
   theWidth (200),
   theHeight (100)
{
}

int
BoxInterface::width () const
{
  qDebug () << " BoxInterface width return " << theWidth;
  return theWidth;
}

int
BoxInterface::height () const
{
  qDebug () << " BoxIinterface height return " << theHeight;
  return theHeight;
}

void
BoxInterface::Start ()
{
}

void
BoxInterface::Stop ()
{
}

void
BoxInterface::SendSize (int h, int w)
{
  SetSize (h,w);
  qDebug () << " SendSize called with " << h << w ;
  emit ChangeShape (QVariant (h), QVariant (w));
}

void
BoxInterface::Clicked ()
{
  qDebug () << " Little Clicked Box";
  CheckBox ("BoxInterface::Clicked Box");
}

void
BoxInterface::BigClicked (float x, float y)
{
  qDebug () << "BigClicked " << x << y;
  CheckBox (QString ("BoxInterface::BigClicked %1 %2").arg(x).arg(y));
}

void
BoxInterface::CheckBox (const QString & msg)
{
  static int count (1);
  qDebug () << "GLScene::CheckBox " << objectName() << msg;
  QMessageBox box;
  box.setText (QString ("%1 wait %3 after %2")
                        .arg(objectName())
                        .arg(msg)
                        .arg(count++));
  QTimer::singleShot (1000, &box, SLOT (accept()));
  box.exec ();
}

} // namespace
