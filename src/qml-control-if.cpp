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
    emit ShowFeed (ident);
  }
}

void
ControlInterface::saveFeed (const QString & feedId,
                             const QString & feedUrl,
                             const QString & title,
                             const QString & siteUrl,
                             const QString & nick,
                             const QString & description)
{
  if (!feeds) {
    return;
  }
  if (feeds->contains (feedId)) {
    Feed & feedRef = feeds->FeedRef(feedId);
    feedRef.values["xmlurl"] = feedUrl;
    feedRef.values["title"] = title;
    feedRef.values["weburl"] = siteUrl;
    feedRef.values["nick"] = nick;
    feedRef.values["description"] = description;
  } else {
    Feed newFeed;
    newFeed.values["xmlurl"] = feedUrl;
    newFeed.values["title"] = title;
    newFeed.values["weburl"] = siteUrl;
    newFeed.values["nick"] = nick;
    newFeed.values["description"] = description;
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

} // namespace
