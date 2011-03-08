#ifndef DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H
#define DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QString>

namespace deliberate 
{

class FeedInterface : public QObject
{
Q_OBJECT

public:

  FeedInterface (QObject *parent = 0);

  Q_INVOKABLE void clickedOn (int index, const QString & day, const QString & meal);
  Q_INVOKABLE void listOrientation (int orient);
  Q_INVOKABLE void report (const QString & msg);
  Q_INVOKABLE void toggleLists ();

signals:

  void ShowStory (const QString & id);
  void ShowList  (const QString & list);
  void HideList (const QString & list);


private:

  enum ListChoice {
    Choice_List = 0,
    Choice_Index
  };

  ListChoice    activeList;
  ListChoice    hiddenList;

  QMap <ListChoice, QString> listNames;

};

} // namespace

#endif
