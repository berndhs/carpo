#ifndef DELIBERATE_CARPO_H
#define DELIBERATE_CARPO_H


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
#include <QMainWindow>
#include <QApplication>
#include <QStringList>
#include <QObject>
#include <QModelIndex>
#include <QGraphicsObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomElement>
#include <QResizeEvent>
#include <QCloseEvent>
#include "headline-list.h"
#include "qml-feed-if.h"
#include "qml-control-if.h"
#include "qml-gesture-if.h"
#include "ui_carpo.h"
#include "feedlist-parser.h"
#include "feedlist.h"
#include "feedlist-model.h"
#include "carpo-netreply.h"
#include "config-edit.h"
#include "property-store.h"
#include "topic-model.h"
#include "auto-update.h"

class QDeclarativeContext;

namespace deliberate
{
class Carpo : public QMainWindow
{
Q_OBJECT
public:

  Carpo (QWidget *parent=0);

  void Init (QApplication & ap);
  void AddConfigMessages (const QStringList & messages);
  void Run ();

  bool Again ();

private slots:

  void Quit ();
  void QmlRun ();
  void LoadList ();
  void FinishedNet (QNetworkReply * reply);
  void ShowStory (const QString & id);
  void DisplayStory (const QString & feedId,
                     const QString & storyTitle, 
                     const QString & storyHash);
  void ShowStorySite (const QString & id);
  void ShowStorySiteLocal (const QString & id);
  void ShowFeed (const QString & id, const QString & storyHash = QString ());
  void EditFeed (const QString & id);
  void ShowList (const QString & list);
  void HideList (const QString & list);
  void ShowAbout ();
  void ShowLicense ();
  void ShowManual ();
  void ShrinkIndex ();
  void ExpandIndex ();
  void SaveFeedListModel (bool reindex=false);
  void MaybeSave ();
  void ProbeFeed (const QString & urlString);
  void Restart ();
  void Reset ();
  void NewestNewsRow (int row);
  void TopicChanged ();
  void ImportList (const QString & format);

protected:

  void resizeEvent (QResizeEvent *event);
  void closeEvent  (QCloseEvent *event);

private:

  void  Connect ();
  void  ParseStories (QDomNodeList & items, 
                      const QString & contentTag,
                      const QString & dateTag1,
                      const QString & dateTag2 = QString());
  void  FillFeedModel (const Folder & folder, 
                       FeedlistModel & model,
                       const QStringList & parentFolders = QStringList ());
  void  LoadFeed (const QString & urlString, 
                  const QString & storyHash = QString());
  void  GetFeedReply (QNetworkReply * reply,  
                      const QString & storyHash = QString());
  void  ProbeReply (QNetworkReply * reply);
  void  WebPageReply (QNetworkReply * reply);
  bool  PopulateFromRssDoc (QDomElement & el, Feed & feed);
  bool  PopulateFromAtomDoc (QDomElement & el, Feed & feed);
  bool  ParseAtomAuthorElem (QDomElement &el, QString & name);
  bool  ParseAtomLinkElem (QDomElement & el, QString & xml, QString & web);
  void  DisplayEditFeed (const QString & id, const Feed & feed);
  void  DumpProperties ();
  void  CheckExists (const QString & filename);


  bool           runAgain;
  QApplication  *app;
  Ui_CarpoMain  ui;
  QStringList    configMessages;

  QDeclarativeContext *context;
  QGraphicsObject     *qmlRoot;
  QString              htmlString;

  HeadlineList         headlines;
  FeedInterface       *feedIF;
  ControlInterface    *controlIF;
  GestureInterface    *gestureIF;

  QNetworkAccessManager *qnam;
  QDomDocument           feedDoc;

  FeedlistParser       *feedlistParser;

  QString              feedListFile;

  Folder                   topFolder;
  FeedlistModel             feeds;
  QMap <QString, QString>  stories;
  QMap <QString, QStringList> storyLinks;
  QMap <QString, QString>  storyDates;

  QMap <QNetworkReply *, CarpoNetReply *>  expectReplies;

  QmlConfigEdit        configEdit;
  PropertyStore       *propStore;
  TopicModel           topicModel;
  AutoUpdate           autoUpdate;
  QString              currentFeed;
  QString              currentStory;
  QTimer               saveTimer;

}; 

} // namespace



#endif
