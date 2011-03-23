#include "qml-control-if.h"
#include <QDebug>


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

#include "newrss-magic.h"

#include <QApplication>
#include <QClipboard>

namespace deliberate
{

ControlInterface::ControlInterface (QObject *parent,
                                    FeedlistModel * feedList)
  :QObject(parent),
   isEditingFeed (false),
   feeds (feedList)
{
}

void
ControlInterface::SetFeeds (FeedlistModel * feedList)
{
  feeds = feedList;
}

void
ControlInterface::setEditingFeed (bool editing)
{
  isEditingFeed = editing;
  qDebug () << "ControlInterface::setEditingFeed " << editing;
}

void
ControlInterface::restartApp ()
{
  emit Restart ();
}

void
ControlInterface::help ()
{
  emit GetHelp ();
}

void
ControlInterface::exitApp ()
{
  emit Exit ();
}

bool
ControlInterface::editingFeed ()
{
  return isEditingFeed;
}

void
ControlInterface::feedClicked (int index, const QString & ident, 
                                            const QString & title)
{
  Q_UNUSED (index)
  Q_UNUSED (title)
  if (isEditingFeed) {
    emit EditFeed (ident);
  } else {
qDebug () << "COntrolInterface :: feedClicked " << ident;
    emit ShowFeed (ident);
  }
}

void
ControlInterface::editFeed (const QString & id)
{
  emit EditFeed (id);
}

void
ControlInterface::loadEditFeed (const QString & urlString)
{
  if (feeds) {
    if (feeds->haveFeedByAddress (urlString)) {
      emit EditFeed (feeds->FeedIdByAddress (urlString));
    }
  }
}

void
ControlInterface::selectFeed (const QString & feedId)
{
  emit ShowFeed (feedId);
}

void
ControlInterface::displayStory (const QString & feedId,
                                const QString & title, 
                                const QString & body)
{
  emit DisplayStory (feedId, title, body);
}

void
ControlInterface::saveFeed (const QString & feedId,
                             const QString & feedUrl,
                             const QString & title,
                             const QString & siteUrl,
                             const QString & nick,
                             const QString & description,
                             const QString & topics)
{
  if (!feeds) {
    return;
  }
  QStringList longTopics = topics.split (";");
  QStringList trimmedTopics;
  int nt=longTopics.count();
  for (int t=0;t<nt; t++) {
    QString topic = longTopics.at(t).trimmed();
    trimmedTopics << topic;
    feeds->topics()[topic].insert(feedId);
  }
  if (feeds->contains (feedId)) {
    Feed & feedRef = feeds->FeedRef(feedId);
    feedRef.values("xmlurl") = feedUrl;
    feedRef.values("title") = title;
    feedRef.values("weburl") = siteUrl;
    feedRef.values("nick") = nick;
    feedRef.values("description") = description;
    feedRef.topics() = trimmedTopics;
  } else {
    Feed newFeed;
    newFeed.values("xmlurl") = feedUrl;
    newFeed.values("title") = title;
    newFeed.values("weburl") = siteUrl;
    newFeed.values("nick") = nick;
    newFeed.values("description") = description;
    newFeed.topics() = trimmedTopics;
    feeds->addFeed (newFeed);
  }
  emit ListUpdated ();
}

void
ControlInterface::removeFeed (const QString & feedId)
{
  if (!feeds) {
    return;
  }
  if (feeds->contains (feedId)) {
    feeds->removeFeed (feedId);
    emit ListUpdated ();
  }
}

void
ControlInterface::probeFeed (const QString & url)
{
  emit ProbeFeed (url);
}

bool
ControlInterface::checkAlert (const QString & alert)
{
  if (alert.startsWith (Magic::PseudoAlertTag)) {
    handlePseudoAlert (alert);
    return true;
  } else {
    qDebug () << "Unknown alert " << alert;
    return false;
  }
}

bool
ControlInterface::addressKnown (const QString & urlString)
{
  if (feeds) {
    return feeds->haveFeedByAddress (urlString);
  } else {
    return false;
  }
}

void
ControlInterface::handlePseudoAlert (const QString & alert)
{
  QStringList parts = alert.split ("/");
  int numParts = parts.count();
  if (numParts > 1) {
    QString command = parts.at(1);
    if (command == "here") {
      if (numParts > 2) {
        emit BrowseLinkLocal (parts.at(2));
      }
    } else if (command == "browser") {
      if (numParts > 2) {
        emit BrowseLinkExternal (parts.at(2));
      }
    } else if (command == "license") {
      emit ShowLicense ();
    } else if (command == "manual") {
      emit ShowManual ();
    }
  }
}

void
ControlInterface::moveUp (const QString & id)
{
  if (feeds) {
    feeds->moveUp (id);
  }
}

void
ControlInterface::moveDown (const QString & id)
{
  if (feeds) {
    feeds->moveDown (id);
  }
}

void
ControlInterface::changeTopic (const QString & newTopic)
{
  if (feeds) {
    feeds->changeTopic (newTopic);
  }
}

void
ControlInterface::toCopy (const QString & copyText)
{
  QClipboard *clip = QApplication::clipboard();
  if (clip) {
    clip->setText (copyText);
  }
}


} // namespace
