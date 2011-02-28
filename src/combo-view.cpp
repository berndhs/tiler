#include "combo-view.h"
#include "gl-scene.h"

#include <QPaintEvent>
#include <QPalette>
#include <QDebug>

namespace tiler
{

ComboView::ComboView (QWidget *parent)
  :QDeclarativeView (parent),
   background (0)
{
  setBackgroundBrush (QBrush (QColor (255,0,0,0),Qt::NoBrush ));
  setBackgroundRole (QPalette::Window);
  setAttribute (Qt::WA_TranslucentBackground,true);
  //QPalette palette;
  //palette.setColor(QPalette::Base, Qt::blue);
  //setPalette (palette);
}


void 
ComboView::drawBackground (QPainter * painter, const QRectF & rect)
{
  qDebug () << " ComboView::drawBackground " << painter << rect;
  if (background) {
    background->updateGL ();
  }
  //QDeclarativeView::drawBackground (painter, rect);
}

void 
ComboView::drawForeground (QPainter * painter, const QRectF & rect)
{
  qDebug () << " ComboView::drawForeground " << painter << rect;
  QDeclarativeView::drawForeground (painter, rect);
}

void
ComboView::update ()
{
  qDebug () << "ComboView::update" << this;
  QDeclarativeView::update ();
}

} // namespace