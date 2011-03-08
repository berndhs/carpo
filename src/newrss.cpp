#include "newrss.h"


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


#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QDebug>

#include "deliberate.h"

namespace deliberate
{

NewRss::NewRss (QWidget *parent)
  :QMainWindow (parent),
   app (0),
   context (0),
   qmlRoot (0),
   headlines (this),
   feedIF (0),
   qnam (0),
   feedlistParser (0),
   feedUrlString ("http://www.atomenabled.org/atom.xml")
{
  qnam = new QNetworkAccessManager;
  feedlistParser = new FeedlistParser (this);
  ui.setupUi (this);
  htmlString = QString ("<html><head></head>"
                        "<body><h1>HTML String number %1</h1></body>"
                        "</html>");
  feedIF = new FeedInterface (this);
  QSize defaultSize = size();
  QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
  resize (newsize);
  QMetaObject::invokeMethod (qmlRoot, "setSize",
               Q_ARG (QVariant, (ui.qmlView->size().width()-2)),
               Q_ARG (QVariant, (ui.qmlView->size().height())-2));
  Connect ();
}

void
NewRss::Init (QApplication & ap)
{
  app = &ap;
}

void
NewRss::AddConfigMessages (const QStringList & messages)
{
  configMessages = messages;
}

void
NewRss::Run ()
{
  context = ui.qmlView->rootContext ();
  context->setContextProperty ("displayModel", &headlines);
  ui.qmlView->setSource (QUrl::fromLocalFile("qml/mainview.qml"));
  context->setContextProperty("feedIF",feedIF);
  qmlRoot = ui.qmlView->rootObject();
  QSize defaultSize = size();
  QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
  resize (newsize);
  QMetaObject::invokeMethod (qmlRoot, "setSize",
               Q_ARG (QVariant, (ui.qmlView->size().width()-2)),
               Q_ARG (QVariant, (ui.qmlView->size().height())-2));
  show ();
}

void
NewRss::Connect ()
{
  connect (ui.actionQuit, SIGNAL (triggered()),
           this, SLOT (Quit()));
  connect (ui.actionLoad, SIGNAL (triggered()),
           this, SLOT (Load ()));
  connect (&headlines, 
           SIGNAL (rowsInserted ( const QModelIndex & , int , int)),
           this,
           SLOT (RowsInserted ( const QModelIndex &, int, int)));
  connect (ui.actionLoadRss, SIGNAL (triggered()),
           this, SLOT (LoadFeed1()));
  connect (ui.actionLoadAtom, SIGNAL (triggered()),
           this, SLOT (LoadFeed2()));
  connect (ui.actionLoadList, SIGNAL (triggered()),
           this, SLOT (LoadList ()));
  connect (ui.actionShrinkIndex, SIGNAL (triggered()),
           this, SLOT (ShrinkIndex ()));
  connect (ui.actionExpandIndex, SIGNAL (triggered()),
           this, SLOT (ExpandIndex ()));
  connect (qnam, SIGNAL (finished (QNetworkReply *)),
           this, SLOT (FinishedNet (QNetworkReply *)));
  connect (feedIF, SIGNAL (ShowStory (const QString &)),
           this, SLOT (ShowStory (const QString &)));
}

void
NewRss::RowsInserted (const QModelIndex & index, int start, int end)
{
  qDebug () << "NewRss::RowsInserted " << index << start << end;
}

void
NewRss::Load ()
{
  static int count (1);
qDebug () << "NewRss::Load " << context << qmlRoot;
  if (context) {
    if (qmlRoot) {
      QMetaObject::invokeMethod (qmlRoot, "turnIndex");
    }
  }
}

void
NewRss::ShowStory (const QString & id)
{
  if (stories.contains(id)) {
    htmlString = stories[id];
  } else {
    htmlString = "<b><i>No Such Story!</b></i>";
  }
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, htmlString));
}

void
NewRss::ShrinkIndex ()
{
  QMetaObject::invokeMethod (qmlRoot, "shrinkIndex");
}

void
NewRss::ExpandIndex ()
{
  QMetaObject::invokeMethod (qmlRoot, "expandIndex");
}

void
NewRss::Quit ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
  if (app) {
    app->quit();
  }
}

void
NewRss::LoadFeed1 ()
{
  if (qnam) {
    qnam->get (QNetworkRequest (QUrl ("http://xkcd.com/rss.xml")));
  }
}

void
NewRss::LoadFeed2 ()
{
  if (qnam) {
    qnam->get (QNetworkRequest (QUrl ("http://www.atomenabled.org/atom.xml")));
  }
}

void
NewRss::FinishedNet (QNetworkReply * reply)
{
  qDebug () << " NewRss::FinishedNet " << reply;
  bool ok = feedDoc.setContent (reply);
  qDebug () << "   parse " << ok;
  qDebug () << "   document " << feedDoc.toString ();
  QDomNodeList items = feedDoc.elementsByTagName ("item");
  QDomNodeList entries = feedDoc.elementsByTagName ("entry");
  if (items.count() > 0) {
    ParseStories (items, "description");
  }
  if (entries.count() > 0) {
    ParseStories (entries, "content");
  }
}

void
NewRss::ParseStories (QDomNodeList & items, const QString & contentTag)
{
  int ni = items.count();
  for (int i=0; i<ni; i++) {
    QDomNode item = items.at(i);
    if (item.isElement()) {
      qDebug () << " Item Element";
      QDomElement elt = item.toElement();
      QDomNodeList titles = elt.elementsByTagName ("title");
      QDomNodeList descrs = elt.elementsByTagName (contentTag);
      int minCount = qMin( titles.count(), descrs.count());
      for (int t=0; t<minCount; t++) {
        QString title = titles.at(t).toElement().text();
        QString descr = descrs.at(t).toElement().text();
        QString id = headlines.addNewLine (title);
        stories[id] = descr;
      }
    }
  }
}


void
NewRss::resizeEvent (QResizeEvent *event)
{
  if (!event) {
    return;
  }
  QMainWindow::resizeEvent (event);
  if (context) {
    QMetaObject::invokeMethod (qmlRoot, "setSize",
                 Q_ARG (QVariant, (ui.qmlView->size().width()-2)),
                 Q_ARG (QVariant, (ui.qmlView->size().height())-2));
  }
}

void
NewRss::LoadList ()
{
  if (feedlistParser) {
    feedlistParser->Read ();
  }
}

} // namespace
