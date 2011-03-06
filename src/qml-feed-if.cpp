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

} // namespace
