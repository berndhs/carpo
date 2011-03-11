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

int 
FeedlistModel::rowCount (const QModelIndex & parent) const
{
  Q_UNUSED (parent)
  return idents.count();
}

QVariant 
FeedlistModel::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retval;
  if (role == Qt::DisplayRole) {
    retval = QString ("%1: %2").arg(idents.value(row))
                               .arg(titles.value(row));
  } else if (role == int (Type_Ident)) {
    retval = idents.value (row);
  } else if (role == int (Type_Title)) {
    retval = titles.value (row);
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
  titles << newFeed.values["title"];
  endInsertRows ();
}

Feed &
FeedlistModel::FeedRef (const QString & id)
{
  return feedMap[id];
}

} // namespace