#ifndef DELIBERATE_FEEDLIST_MODEL_H
#define DELIBERATE_FEEDLIST_MODEL_H


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

#include <QAbstractListModel>
#include <QStringList>
#include <QString>
#include <QMap>
#include "feedlist.h"

namespace deliberate
{
class FeedlistModel : public QAbstractListModel
{
Q_OBJECT

public:

  FeedlistModel (QObject *parent=0);
  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  void addFeed (const Feed & newFeed);

  Feed & FeedRef (const QString & id);
  bool   contains (const QString & id) const;

private:

  enum DataType {
     Type_Ident = Qt::UserRole+1,
     Type_Title = Qt::UserRole+2
  };

  QStringList   idents;
  QStringList   titles;

  QMap <QString, Feed>  feedMap;

  static int    nextId;

};

} // namespace

#endif
