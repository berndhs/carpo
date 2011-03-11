#include "qml-feed-if.h"
#include <QDebug>

namespace deliberate
{

FeedInterface::FeedInterface (QObject *parent)
  :QObject(parent),
   activeList (Choice_List),
   hiddenList (Choice_Index)
{
  listNames[Choice_List] = "FeedList";
  listNames[Choice_Index] = "FeedIndex";
}

void
FeedInterface::storyClicked (int index, const QString & id, const QString & title)
{
  qDebug () << "FeedInterface: clicked on " << index << id << title;
  emit ShowStory (id);
}

void
FeedInterface::feedClicked (int index, const QString & id, const QString & title)
{
  qDebug () << "FeedInterface: clicked on " << index << id << title;
  emit ShowFeed (id);
}

void
FeedInterface::listOrientation (int orient)
{
  qDebug () << "FeedInterface::listOrientation " << orient;
}

void
FeedInterface::report (const QString & msg)
{
  qDebug () << "FeedInterface::report " << msg;
}

void
FeedInterface::toggleLists ()
{
  hiddenList = activeList;
  switch (activeList) {
  case Choice_Index:
    activeList = Choice_List;
  break;
  case Choice_List:
    activeList = Choice_Index;
    break;
  default:
    activeList = Choice_List;
    break;
  }
  emit HideList (listNames[hiddenList]);
  emit ShowList (listNames[activeList]);
}

void
FeedInterface::SetActive (ListChoice active)
{
  activeList = active;
  switch (activeList) {
  case Choice_Index:
    hiddenList = Choice_List;
  break;
  case Choice_List:
    hiddenList = Choice_Index;
    break;
  default:
    hiddenList = Choice_List;
    break;
  }
}


} // namespace