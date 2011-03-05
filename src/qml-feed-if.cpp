#include "qml-feed-if.h"
#include <QDebug>

namespace deliberate
{

FeedInterface::FeedInterface (QObject *parent)
  :QObject(parent)
{
}

void
FeedInterface::clickedOn (const QString & day, const QString & meal)
{
  qDebug () << "FeedInterface: clicked on " << day << meal;
}

} // namespace
