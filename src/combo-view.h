#ifndef TILER_QML_OVERLAY_H
#define TILER_QML_OVERLAY_H

#include <QDeclarativeView>
#include <QPainter>
#include <QRectF>

class QPaintEvent;

namespace tiler
{
class GLScene;

class ComboView : public QDeclarativeView
{
Q_OBJECT
public:

  ComboView (QWidget *parent=0);

  void SetGLBackground (GLScene * back) { background = back; }

  void drawBackground (QPainter * painter, const QRectF & rect);

public slots:

  void update ();

private:

  GLScene  * background;

};

} // namespace


#endif
