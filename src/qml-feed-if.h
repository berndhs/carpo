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

  Q_INVOKABLE void clickedOn (const QString & day, const QString & meal);
  Q_INVOKABLE void listOrientation (int orient);
  Q_INVOKABLE void report (const QString & msg);

signals:

  void ShowStory (const QString & id);


private:

};

} // namespace

#endif
