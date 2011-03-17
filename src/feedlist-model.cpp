#include "feedlist-model.h"


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

#include <QDebug>


namespace deliberate
{


int FeedlistModel::nextId (555);

FeedlistModel::FeedlistModel (QObject *parent)
  :QAbstractListModel(parent)
{
  QHash<int, QByteArray> roles;
  roles[Type_Ident] = "ident";
  roles[Type_Title] = "title";
  setRoleNames(roles);
}

TopicIndexMap &
FeedlistModel::topics ()
{
  return topicIndex;
}

int 
FeedlistModel::rowCount (const QModelIndex & parent) const
{
  Q_UNUSED (parent)
  return idents.count();
}

bool
FeedlistModel::contains (const QString & id) const
{
  return feedMap.contains (id);
}

QVariant 
FeedlistModel::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retval;
  QString ident = idents.value(row);
  if (role == Qt::DisplayRole) {
    retval = QString ("%1: %2").arg(ident)
                               .arg(feedMap[ident].values("title"));
  } else if (role == int (Type_Ident)) {
    retval = ident;
  } else if (role == int (Type_Title)) {
    retval = feedMap[ident].values("title");
  }
  return retval;
}

void
FeedlistModel::addFeed (const Feed & newFeed)
{
  QString ident (QString ("Feed_%1").arg(nextId++));
  feedMap[ident] = newFeed;
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  idents << ident;
  QStringList  topics (newFeed.topics());
  int nt = topics.count();
  for (int t=0; t<nt; t++) {
    topicIndex[topics.at(t)].insert (ident);
  }
  endInsertRows ();
}

void
FeedlistModel::removeFeed (const QString & id)
{
  int index = idents.indexOf (id);
  if (index < 0) {
    return;
  }
  beginRemoveRows (QModelIndex(), index, index);
  QStringList  topics (feedMap[id].topics());
  int nt = topics.count();
  for (int t=0; t<nt; t++) {
    topicIndex[topics.at(t)].remove (id);
  }
  idents.removeAll (id);
  feedMap.remove (id);
  endRemoveRows();
}

Feed &
FeedlistModel::FeedRef (const QString & id)
{
  return feedMap[id];
}


void
FeedlistModel::moveUp (const QString & id)
{
  int ndx = idents.indexOf (id);
  if (ndx > 0)  {
    idents.move (ndx, ndx-1);
    emit dataChanged (createIndex (ndx-1,0), createIndex (ndx,0));
    emit ListChanged ();
  }
}

void
FeedlistModel::moveDown (const QString & id)
{
  int ndx = idents.indexOf (id);
  if (ndx >= 0 && ndx < idents.count() - 1)  {
    idents.move (ndx, ndx+1);
    emit dataChanged (createIndex (ndx,0), createIndex (ndx+1,0));
    emit ListChanged ();
  }
}

} // namespace
