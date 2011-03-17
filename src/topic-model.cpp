#include "topic-model.h"


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

TopicModel::TopicModel (QObject *parent)
  :QAbstractListModel (parent),
   feeds (0)
{
  QHash<int, QByteArray> roles;
  roles[Type_Name] = "name";
  roles[Type_Count] = "number";
  setRoleNames(roles);
}

int
TopicModel::rowCount (const QModelIndex & index) const
{
  Q_UNUSED (index)
  return topicKeys.count();
}

QVariant
TopicModel::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  if (!feeds) {
    return QVariant();
  }
  int row = index.row ();
  if (row < 0 || row > topicKeys.count()) {
    return QVariant();
  }
  QVariant retval;
  switch (role) {
  case Qt::DisplayRole:
  case int (Type_Name):
    retval = topicKeys.at(row);
    break;
  case int (Type_Count):
    retval = feeds->topics()[topicKeys.at(row)].count();
    break;
  default:
    break;
  }
  return retval;
}

void
TopicModel::SetFeedModel (FeedlistModel * feedListModel)
{
  beginResetModel ();
  topicKeys.clear ();
  feeds = feedListModel;
  if (!feeds) {
    endResetModel ();
    return;
  }
  TopicIndexMap::iterator it;
  for (it=feeds->topics().begin(); it != feeds->topics().end(); it++) {
    topicKeys.append (it.key());
  }
  endResetModel ();
  emit dataChanged (createIndex (0,0), createIndex (topicKeys.count()-1,0));
}

void
TopicModel::ReIndex ()
{
  beginResetModel ();
  topicKeys.clear ();
  if (!feeds) {
    endResetModel ();
    return;
  }
  TopicIndexMap::iterator it;
  for (it=feeds->topics().begin(); it != feeds->topics().end(); it++) {
    topicKeys.append (it.key());
  }
  endResetModel ();
  emit dataChanged (createIndex (0,0), createIndex (topicKeys.count()-1,0));
}

} // namespace
