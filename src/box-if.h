#ifndef COMPEX_BOX_IF_H
#define COMPEX_BOX_IF_H

#include <QObject>
#include <QVariant>
#include <QSize>

namespace tiler 
{

class BoxInterface : public QObject
{
Q_OBJECT

public:

  BoxInterface (QObject *parent = 0);

  Q_INVOKABLE  int width () const;
  Q_INVOKABLE  int height () const;

  void SetSize (const QSize & size) 
    { 
      theHeight = size.height(); 
      theWidth = size.width();
    }

  void SetSize (int h, int w) 
    { 
      theHeight = h; 
      theWidth = w; 
    }

  void SendSize (int h, int w);

public slots:

  void Start ();
  void Stop ();
  void Clicked ();
  void BigClicked (float x, float y);

signals:

  void ChangeShape (QVariant hi, QVariant wide);

private:

  void CheckBox (const QString & msg);

  int  theWidth;
  int  theHeight;
};

} // namespace

#endif
