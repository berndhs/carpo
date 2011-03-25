#ifndef DELIBERATE_DRSS_AUTO_UPDATE_H
#define DELIBERATE_DRSS_AUTO_UPDATE_H


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


#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QAbstractListModel>
#include <QDebug>

#include "feedlist-model.h"
#include "carpo-netreply.h"

namespace deliberate
{

class NewStory
{
public:

   NewStory ()
     :theFeedId(""), theTitle (""), theHash ("")
   {}
   NewStory (const QString & feedId, const QString & title, const QString & hash)
     :theFeedId (feedId), theTitle (title), theHash (hash)
   {}

   QString feedId () const { return theFeedId; }
   QString title  () const { return theTitle; }
   QString hash  () const { return theHash; }

   friend QDebug operator << (QDebug debug, const NewStory & story);

private:

  QString  theFeedId;
  QString  theTitle;
  QString  theHash;

};

typedef QList <NewStory>  NewStoryList;

class AutoUpdate : public QAbstractListModel
{
Q_OBJECT

public:

  AutoUpdate (FeedlistModel & feedlistModel, QObject *parent=0);
  ~AutoUpdate ();

  int  rowCount (const QModelIndex & index) const;
  QVariant  data (const QModelIndex & index, int role) const;

  void SetInterval (int msecs);
  void Start (int msecs);
  void Stop ();
  void Init ();

  NewStoryList & newStories ();

private slots:

  void Poll (int numFeeds = 1);
  void FinishedNet (QNetworkReply * reply);

signals:
  
  void NewestRow (int row);

private:

  enum DataType {
    Type_FeedId = Qt::UserRole+1,
    Type_FeedTitle  = Qt::UserRole+2,
    Type_StoryTitle = Qt::UserRole+3,
    Type_StoryHash  = Qt::UserRole+4
  };

  void GetPollReply (CarpoNetReply * reply);
  void ParseStories (const QString & feedId,
                     QDomNodeList & items, 
                     const QString & contentTag);
  void AddStory (const QString & feedId, 
                 const QString & title, 
                 const QString & hash);

  QTimer                   updateTimer;
  FeedlistModel          & feeds;
  QStringList            & idList;
  int                      chaser;
  QNetworkAccessManager   *qnam;
  QMap <QNetworkReply *, CarpoNetReply *>  expectReplies;
  NewStoryList             newStoryList;
}; 

} // namespace

#endif
