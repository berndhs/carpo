#include "qml-feed-if.h"
#include <QDebug>

namespace deliberate
{

FeedInterface::FeedInterface (QObject *parent)
  :QObject(parent),
   theWidth (200),
   theHeight (100)
{
}

int
FeedInterface::width () const
{
  qDebug () << " FeedInterface width return " << theWidth;
  return theWidth;
}

int
FeedInterface::height () const
{
  qDebug () << " BoxIinterface height return " << theHeight;
  return theHeight;
}

void
FeedInterface::Start ()
{
}

void
FeedInterface::Stop ()
{
}

void
FeedInterface::SendSize (int h, int w)
{
  SetSize (h,w);
  qDebug () << " SendSize called with " << h << w ;
  emit ChangeShape (QVariant (h), QVariant (w));
}

void
FeedInterface::Clicked ()
{
  qDebug () << " Clicked Box";
}

} // namespace
