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
#include <time.h>

namespace deliberate
{

AutoUpdate::AutoUpdate (FeedlistModel & feedlistModel, QObject *parent)
  :QAbstractListModel (parent),
   updateTimer (this),
   feeds (feedlistModel),
   idList (feedlistModel.feedIdList())
{
  qnam = new QNetworkAccessManager;
  QHash<int, QByteArray> roles;
  roles[Type_FeedId] = "feedId";
  roles[Type_FeedTitle] = "feedTitle";
  roles[Type_StoryTitle] = "storyTitle";
  roles[Type_StoryHash] = "storyHash";
  setRoleNames(roles);
}

AutoUpdate::~AutoUpdate ()
{
  //qDebug () << "AutoUpdate::~AutoUpdate  destructor";
  //qDebug () << newStoryList;
}

int  
AutoUpdate::rowCount (const QModelIndex & index) const
{
  Q_UNUSED (index)
  return newStoryList.count();
}

QVariant  
AutoUpdate::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  switch (role) {
  case Qt::DisplayRole:
  case int (Type_StoryTitle):
    return newStoryList.at(row).title();
    break;
  case int (Type_FeedId):
    return newStoryList.at(row).feedId();
    break;
  case int (Type_FeedTitle):
    return feeds.FeedRef(newStoryList.at(row).feedId()).values("title");
    break;
  case int (Type_StoryHash):
    return newStoryList.at(row).hash();
    break;
  default:
    return QVariant ();
    break;
  }
  return QVariant ();
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

  #if USE_LINUX_RANDOM
  timespec       fineTime;
  clock_gettime (CLOCK_REALTIME, &fineTime);
  seed = abs (fineTime.tv_sec + fineTime.tv_nsec);
  srandom (seed);
  int skip = (random() % (idList.count()+1));
  qDebug () << " AutoUpdate Linux seed " << seed;
  #else
  seed = time(0);
  srand (seed);
  int skip = (rand() % (idList.count()+1));
  qDebug () << " AutoUpdate non-Linux seed " << seed;
  #endif


  chaser = 0;
  if (idList.count() > 0) {
    chaser = skip;
  }
  connect (&updateTimer, SIGNAL (timeout()),
           this, SLOT (Poll()));
  connect (qnam, SIGNAL (finished (QNetworkReply *)),
           this, SLOT (FinishedNet (QNetworkReply *)));
}

void
AutoUpdate::Poll (int numFeeds)
{
  Q_UNUSED (numFeeds)
  if (idList.count() < 1) {
    return;
  }
  if (chaser < idList.count()) {
    QString feedId = idList.at(chaser);
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
  if (chaser >= idList.count()) {
    chaser = 0;
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
  for (int i=ni-1; i>=0; i--) {
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
          AddStory (feedId, title, hash);
        }
      }
    }
  }
}

void
AutoUpdate::AddStory (const QString & feedId, 
                      const QString & title,
                      const QString & hash)
{
  int newRow = newStoryList.count();
  beginInsertRows (QModelIndex(), newRow, newRow);
  newStoryList.append (NewStory (feedId, title, hash));
  endInsertRows ();
  emit NewestRow (newRow);
}

QDebug
operator<< (QDebug debug, const NewStory & story)
{
  debug.nospace() << " ( NewStory "
                  << " feedId " << story.feedId()
                  << " title " << story.title()
                  << " hash " << story.hash()
                  << " ) "
                  ;
  return debug.space (); 
}


} // namespace
