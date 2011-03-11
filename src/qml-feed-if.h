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

  enum ListChoice {
    Choice_List = 0,
    Choice_Index
  };

  FeedInterface (QObject *parent = 0);

  Q_INVOKABLE void storyClicked (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE void feedClicked (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE void listOrientation (int orient);
  Q_INVOKABLE void report (const QString & msg);
  Q_INVOKABLE void toggleLists ();

  void SetActive (ListChoice active);

signals:

  void ShowStory (const QString & id);
  void ShowFeed (const QString & id);
  void ShowList  (const QString & list);
  void HideList (const QString & list);


private:

  ListChoice    activeList;
  ListChoice    hiddenList;

  QMap <ListChoice, QString> listNames;

};

} // namespace

#endif