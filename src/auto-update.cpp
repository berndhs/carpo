#include "auto-update.h"



/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


#include <QCryptographicHash>

#include <QDebug>
#include <stdlib.h>

namespace deliberate
{

AutoUpdate::AutoUpdate (FeedlistModel & feedlistModel, QObject *parent)
  :QObject (parent),
   updateTimer (this),
   feeds (feedlistModel),
   idList (feedlistModel.feedIdList())
{
  qnam = new QNetworkAccessManager;
}

AutoUpdate::~AutoUpdate ()
{
  //qDebug () << "AutoUpdate::~AutoUpdate  destructor";
  //qDebug () << newStoryList;
}

void
AutoUpdate::SetInterval (int msecs)
{
  updateTimer.setInterval (msecs);
}

void
AutoUpdate::Start (int msecs)
{
  updateTimer.start (msecs);
  qDebug () << " AutoUpdate::Start " << updateTimer.interval();
}

void
AutoUpdate::Stop ()
{
  updateTimer.stop ();
}

void
AutoUpdate::Init ()
{
  unsigned   int seed (0);
  timespec       fineTime;
  clock_gettime (CLOCK_REALTIME, &fineTime);
  seed = abs (fineTime.tv_sec + fineTime.tv_nsec);
  srandom (seed);

  chaser = idList.begin();
  int skip = (random() % idList.count());
  for (int i=0; i<skip; i++) {
    chaser++;
  }
  qDebug () << "AutoUpdate skipped " << skip << " items, now at " << *chaser;
  connect (&updateTimer, SIGNAL (timeout()),
           this, SLOT (Poll()));
  connect (qnam, SIGNAL (finished (QNetworkReply *)),
           this, SLOT (FinishedNet (QNetworkReply *)));
}

void
AutoUpdate::Poll (int numFeeds)
{
  qDebug () << "Autoupdate::Poll " << numFeeds;
  if (chaser != idList.end()) {
    QString feedId = *chaser;
    qDebug () << "AutoUpdate::Poll feed " << feedId;
    QString urlString = feeds.FeedRef(feedId).values("xmlurl");
    if (qnam) {
      QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
      DrssNetReply * dreply = new DrssNetReply (netreply, 
                                      DrssNetReply::Kind_PollFeed);
      dreply->setFeedId (feedId);
      expectReplies[netreply] = dreply;
    }
  }
  chaser ++;
  if (chaser == idList.end()) {
    chaser = idList.begin();
  }
}

void
AutoUpdate::FinishedNet (QNetworkReply * reply)
{
  if (reply) {
    if (expectReplies.contains (reply)) {
      DrssNetReply * dreply = expectReplies[reply];
      if (dreply) {
        DrssNetReply::Kind  kind = dreply->kind();
        switch (kind) {
        case DrssNetReply::Kind_PollFeed:
          GetPollReply (dreply);
          break;
        default:
          qDebug () << " bad network reply kind " << reply << kind;
          break;
        }
        expectReplies.remove (reply);
        delete dreply;
      }
    }
  }
}

void
AutoUpdate::GetPollReply (DrssNetReply * dreply)
{
  if (!dreply) {
    return;
  }
  QString feedId = dreply->feedId();
  QNetworkReply * reply = dreply->netReply();
  if (!reply) {
    return;
  }
  QDomDocument feedDoc;
  feedDoc.setContent (reply);
  QDomNodeList items = feedDoc.elementsByTagName ("item");
  QDomNodeList entries = feedDoc.elementsByTagName ("entry");
  if (items.count() > 0) {
    ParseStories (feedId, items, "description");
  }
  if (entries.count() > 0) {
    ParseStories (feedId, entries, "summary");
  }
}

void
AutoUpdate::ParseStories (const QString & feedId,
                      QDomNodeList & items, const QString & contentTag)
{
  int ni = items.count();
  for (int i=0; i<ni; i++) {
    QDomNode item = items.at(i);
    if (item.isElement()) {
      QDomElement elt = item.toElement();
      QDomNodeList titles = elt.elementsByTagName ("title");
      QDomNodeList descrs = elt.elementsByTagName (contentTag);
      if (titles.count() > 0 && descrs.count() > 0) {  // should be 1
        QString title = titles.at(0).toElement().text();
        QString story = descrs.at(0).toElement().text();
        QString hash (QCryptographicHash::hash (story.toUtf8().data(), 
                           QCryptographicHash::Md5).toHex());
        bool haveIt = feeds.HaveStory (feedId, hash);
        if (!haveIt) {
          feeds.MarkHashRead (feedId, hash, false);
          newStoryList.append (NewStory (feedId, title, story));
          qDebug () << "   AutoUpdate for feed " << feedId;
          qDebug () << "              new story " << title;
        }
      }
    }
  }
}

QDebug
operator<< (QDebug debug, const NewStory & story)
{
  debug.nospace() << " ( NewStory "
                  << " feedId " << story.feedId()
                  << " title " << story.title()
                  << " story " << story.story().left(80)
                  << (story.story().length() > 80 ? "..." : "")
                  ;
  return debug.space (); 
}


} // namespace
