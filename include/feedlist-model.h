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
#include <QSet>
#include "feedlist.h"

namespace deliberate
{

typedef QSet<QString>              FeedIdSet;
typedef QMap <QString, FeedIdSet>  TopicIndexMap;

class FeedlistModel : public QAbstractListModel
{
Q_OBJECT

private:

  typedef QMap <QString, Feed>  FeedMapType;

public:

  /** \brief stuff invokable from QML */

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  /** \brief other functions */

  FeedlistModel (QObject *parent=0);

  void clear ();

  bool dirty () { return isDirty; }
  void setDirty (bool d) { isDirty = d; }

  bool addFeed (const Feed & newFeed);
  void removeFeed (const QString & id);
  void moveUp (const QString & id);     // move id to lower index
  void moveDown (const QString & id);   // move id to higher index

  bool    contains (const QString & id) const;
  bool    haveFeedByAddress (const QString & xmlUrl) const;
 
  Feed  & FeedRef (const QString & id);
  Feed  & FeedRefByAddress (const QString & xmlUrl);
  QString FeedIdByAddress (const QString & xmlUrl);

  TopicIndexMap & topics ();

  void changeTopic (const QString & newTopic);

  void MarkRead (const QString & feedId, 
                 const QString & storyText, 
                 bool isRead);
  void MarkHashRead (const QString & feedId, 
                 const QString & hash, 
                 bool isRead);
  bool Seenit (const QString & feedId, const QString & hash);
  bool HaveStory (const QString & feedId, const QString & hash);


  QStringList & feedIdList ();
 
signals:
 
  void ListChanged ();
  void TopicChanged ();

private:

  enum DataType {
     Type_Ident = Qt::UserRole+1,
     Type_Title = Qt::UserRole+2
  };

  bool           isDirty;
  QStringList    *idents;

  QStringList     mainIdents;
  QStringList     topicIdents;
  QString         currentTopic;

  FeedMapType    feedMap;

  static int     nextId;

  friend class   FeedlistWriter;

  TopicIndexMap  topicIndex;

  QMap <QString, QString>  addrToId;

};

} // namespace

#endif
