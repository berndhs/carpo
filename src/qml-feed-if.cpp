#include "qml-feed-if.h"
#include <QDebug>

namespace deliberate
{

FeedInterface::FeedInterface (QObject *parent)
  :QObject(parent)
{
}

void
FeedInterface::clickedOn (const QString & id, const QString & title)
{
  qDebug () << "FeedInterface: clicked on " << id << title;
  emit ShowStory (id);
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

} // namespace
