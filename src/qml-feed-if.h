#ifndef DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H
#define DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H

#include <QObject>
#include <QVariant>

namespace deliberate 
{

class FeedInterface : public QObject
{
Q_OBJECT

public:

  FeedInterface (QObject *parent = 0);

  Q_INVOKABLE  int width () const;
  Q_INVOKABLE  int height () const;

  void SetSize (int h, int w) { theHeight = h; theWidth = w; }

  void SendSize (int h, int w);

public slots:

  void Start ();
  void Stop ();
  void Clicked ();

signals:

  void ChangeShape (QVariant hi, QVariant wide);

private:

  int  theWidth;
  int  theHeight;
};

} // namespace

#endif
