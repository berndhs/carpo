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

#include <Qt>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QGraphicsObject>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QSize>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>

#include "feedlist-writer.h"

#include "deliberate.h"
#include "version.h"

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
   feeds (this),
   configEdit (this)
{
  feedListFile = "drss_feeds.xml";
  qnam = new QNetworkAccessManager;
  feedlistParser = new FeedlistParser (this);
  ui.setupUi (this);
  htmlString = QString ("<html><head></head>"
                        "<body><h1>HTML String number %1</h1></body>"
                        "</html>");
  feedIF = new FeedInterface (this);
  controlIF = new ControlInterface (this, &feeds);
  restoreGeometry(Settings().value("geometry").toByteArray());
  qDebug () << "QML size " << Settings().value("sizes/qml").toSize();
  if (Settings().contains ("sizes/qml")) {
    QSize qmlsize = Settings().value("sizes/qml").toSize();
    qDebug () << " Found QML saved size " << qmlsize;
    ui.qmlView->resize (qmlsize);
  }
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
  qDebug () << " NewRss::Run";
  LoadList ();
  context = ui.qmlView->rootContext ();
  context->setContextProperty ("feedIndexModel", &headlines);
  context->setContextProperty ("feedListModel", &feeds);
  ui.qmlView->setSource (QUrl::fromLocalFile("qml/mainview.qml"));
  context->setContextProperty ("feedIF",feedIF);
  context->setContextProperty ("controlIF",controlIF);
  qDebug () << " NewRss  context base url " << context->baseUrl();
  qDebug () << " NewRss  engine import paths "
            << ui.qmlView->engine()->importPathList();         
  qDebug () << " NewRss  engine plugin paths "
            << ui.qmlView->engine()->pluginPathList();
  qmlRoot = ui.qmlView->rootObject();       
  qDebug () << " NewRss  engine base url "
            << ui.qmlView->engine()->baseUrl();
  qmlRoot = ui.qmlView->rootObject();
  qDebug () << " top size " << size();
  qDebug () << " qml size " << ui.qmlView->size();
  qDebug () << " try to set size " << ui.qmlView->size() ;
  qDebug () << "      on root object " << qmlRoot;
  if (qmlRoot) {
    QMetaObject::invokeMethod (qmlRoot, "setSize",
                 Q_ARG (QVariant, (ui.qmlView->size().width()-2)),
                 Q_ARG (QVariant, (ui.qmlView->size().height())-2));
  }
  ShowList ("FeedList");
  topFolder.clear ();
  show ();
  configEdit.Load ();
}

void
NewRss::Connect ()
{
  connect (ui.actionQuit, SIGNAL (triggered()),
           this, SLOT (Quit()));
  connect (ui.actionLoadList, SIGNAL (triggered()),
           this, SLOT (LoadList ()));
  connect (ui.actionAbout, SIGNAL (triggered()),
           this, SLOT (ShowAbout()));
  connect (ui.actionLicense, SIGNAL (triggered()),
           this, SLOT (ShowLicense()));
  connect (qnam, SIGNAL (finished (QNetworkReply *)),
           this, SLOT (FinishedNet (QNetworkReply *)));
  connect (feedIF, SIGNAL (ShowStory (const QString &)),
           this, SLOT (ShowStory (const QString &)));
  connect (feedIF, SIGNAL (ShowStorySite (const QString &)),
           this, SLOT (ShowStorySite (const QString &)));
  connect (feedIF, SIGNAL (ShowFeed (const QString &)),
           this, SLOT (ShowFeed (const QString &)));
  connect (feedIF, SIGNAL (ShowList (const QString &)),
           this, SLOT (ShowList (const QString &)));
  connect (feedIF, SIGNAL (HideList (const QString &)),
           this, SLOT (HideList (const QString &)));
  connect (controlIF, SIGNAL (ShowFeed (const QString &)),
           this, SLOT (ShowFeed (const QString &)));
  connect (controlIF, SIGNAL (EditFeed (const QString &)),
           this, SLOT (EditFeed (const QString &)));
  connect (controlIF, SIGNAL (ListUpdated ()),
           this, SLOT (SaveFeedListModel ()));
  connect (controlIF, SIGNAL (ProbeFeed (const QString &)),
           this, SLOT (ProbeFeed (const QString &)));
}


void
NewRss::ShowStory (const QString & id)
{
  qDebug () << " NewRss::ShowStory";
  if (stories.contains(id)) {
    htmlString = stories[id];
  } else {
    htmlString = "<b><i>No Such Story!</b></i>";
  }
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, htmlString));
}

void
NewRss::ShowStorySite (const QString & id)
{
  qDebug () << "NewRss::ShowStorySite";
  if (storyLinks.contains (id)) {
    QString first = storyLinks[id].first();
    QDesktopServices::openUrl (QUrl::fromUserInput(first));
  }
}

void
NewRss::ShowFeed (const QString & id)
{
  qDebug () << " NewRss::ShowFeed";
  QString urlString = feeds.FeedRef(id).values["xmlurl"];
  headlines.clear ();
  LoadFeed (urlString);
}

void
NewRss::EditFeed (const QString & id)
{
  Feed & feed = feeds.FeedRef (id);
  DisplayEditFeed (id, feed);
}

void
NewRss::DisplayEditFeed (const QString & id, const Feed & feed)
{
  QString urlString = feed.values["xmlurl"];
  QString nick = feed.values["nick"];
  QString siteUrl = feed.values["weburl"];
  QString title = feed.values["title"];
  QString descr = feed.values["description"];
  QMetaObject::invokeMethod (qmlRoot, "displayEditFeed",
                 Q_ARG (QVariant, id),
                 Q_ARG (QVariant, urlString),
                 Q_ARG (QVariant, title),
                 Q_ARG (QVariant, siteUrl),
                 Q_ARG (QVariant, nick),
                 Q_ARG (QVariant, descr));
}

void
NewRss::ShowList (const QString & list)
{
  QMetaObject::invokeMethod (qmlRoot, 
                             QString("expand%1").arg(list).toLatin1().data());
}

void
NewRss::ShowAbout ()
{
  QString about (QString("<pre>\n%1\n</pre>").arg(ProgramVersion::Version()));
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, about));
}

void
NewRss::ShowLicense ()
{
  QFile licfile (":/help/LICENSE.txt");
  licfile.open (QFile::ReadOnly);
  QByteArray lictext = licfile.readAll();
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                Q_ARG (QVariant, "<pre>\n" + lictext + "\n</pre>\n"));
}

void
NewRss::HideList (const QString & list)
{
  QMetaObject::invokeMethod (qmlRoot, 
                            QString("shrink%1").arg(list).toLatin1().data());
}

void
NewRss::ShrinkIndex ()
{
  HideList ("FeedIndex");
}

void
NewRss::ExpandIndex ()
{
  ShowList ("FeedIndex");
}

void
NewRss::Quit ()
{
  Settings().setValue("geometry", saveGeometry());
  Settings().setValue("sizes/qml", ui.qmlView->size());
  Settings().sync();
  if (app) {
    app->quit();
  }
}

void
NewRss::LoadFeed (const QString & urlString)
{
  if (qnam) {
    QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
    DrssNetReply * dreply = new DrssNetReply (netreply, 
                                      DrssNetReply::Kind_GetFeed);
    expectReplies[netreply] = dreply;
  }
}

void
NewRss::ProbeFeed (const QString & urlString)
{
  if (qnam) {
    QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
    DrssNetReply * dreply = new DrssNetReply (netreply, 
                                      DrssNetReply::Kind_Probe);
    expectReplies[netreply] = dreply;
  }
}

void
NewRss::FinishedNet (QNetworkReply * reply)
{
  if (reply) {
    if (expectReplies.contains (reply)) {
      DrssNetReply * dreply = expectReplies[reply];
      if (dreply) {
        DrssNetReply::Kind  kind = dreply->kind();
        switch (kind) {
        case DrssNetReply::Kind_GetFeed:
          GetFeedReply (dreply->netReply());
          break;
        case DrssNetReply::Kind_Probe:
          ProbeReply (dreply->netReply());
          break;
        default:
          qDebug () << " bad network reply kind " << reply << kind;
          break;
        }
        expectReplies.remove (reply);
        delete dreply;
      }
    }
  }
}

void
NewRss::GetFeedReply (QNetworkReply * reply)
{
  if (!reply) {
    return;
  }
  feedDoc.setContent (reply);
  QDomNodeList items = feedDoc.elementsByTagName ("item");
  QDomNodeList entries = feedDoc.elementsByTagName ("entry");
  if (items.count() > 0) {
    ParseStories (items, "description");
  }
  if (entries.count() > 0) {
    ParseStories (entries, "content");
  }
  if (headlines.count() > 0) {
    ShowList ("FeedIndex");
    HideList ("FeedList");
    feedIF->SetActive (FeedInterface::Choice_Index);
  }
}

void
NewRss::ProbeReply (QNetworkReply * reply)
{
  QDomDocument probeDoc;
  probeDoc.setContent (reply);
  QDomElement root = probeDoc.documentElement ();
  QString tag = root.tagName();
  Feed newFeed;
  newFeed.values["xmlurl"] = reply->url().toString();
  bool foundData (false);
  if (tag == "rss" || tag.startsWith ("rdf")) {  // probaly RSS
    for (QDomElement rssel = root.firstChildElement();
         !rssel.isNull();
         rssel = rssel.nextSiblingElement()) {
      tag = rssel.tagName();
      if (tag == "channel") {
        foundData = PopulateFromRssDoc (rssel, newFeed);
      } 
      if (foundData) {
        break;
      }
    }
  } else if (tag == "feed") {
    foundData = PopulateFromAtomDoc (root, newFeed);
  }
  if (foundData) {
    DisplayEditFeed ("", newFeed);
  }
}

bool
NewRss::PopulateFromRssDoc (QDomElement & el, Feed & feed)
{
  static QString tag_title("title");
  static QString tag_link("link");
  static QString tag_description("description");
  QString t;
  QString weblink;
  QString title;
  QString description (tr("cannot find RSS feed data"));
  bool foundsomething(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    t = child.tagName();
    if (t == tag_title) {
      title = child.text();
      foundsomething = true;
    } else if (t == tag_link) {
      weblink = child.text();
      foundsomething = true;
    } else if (t == tag_description) {
      description = child.text();
    }
  }
  feed.values["title"] = title;
  feed.values["weburl"] = weblink;
  feed.values["description"] = description;
  return foundsomething;
}


bool
NewRss::PopulateFromAtomDoc (QDomElement & el, Feed & feed)
{
  QString t;
  static QString tag_title("title");
  static QString tag_link("link");
  static QString tag_author("author");
  static QString tag_subtitle("subtitle");
  QString xmllink;
  QString weblink;
  QString title;
  QString author;
  QString description (tr(""));
  bool foundsomething(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    t = child.tagName();
    if (t == tag_title) {
      title = child.text();
      foundsomething = true;
    } else if (t == tag_link) {
      foundsomething |= ParseAtomLinkElem (child, xmllink, weblink);
    } else if (t == tag_author) {
      foundsomething |= ParseAtomAuthorElem (child, author);
    } else if (t == tag_subtitle) {
      description = child.text();
    }
  }
  feed.values["title"] = title;
  feed.values["weburl"] = weblink;
  feed.values["description"] = description;
  return foundsomething;
}

bool 
NewRss::ParseAtomLinkElem (QDomElement & el, QString & xml, QString & web)
{
   QString relAttr = el.attribute("rel");
   bool ok(false);
   if (relAttr == "self") {
     xml = el.attribute("href");
     ok = true;
   } else {
     web = el.attribute("href");
     ok = true;
   }
   return ok;
}

bool
NewRss::ParseAtomAuthorElem (QDomElement &el, QString & name)
{
  bool ok(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    if (child.tagName() == "name") {
      name = child.text();
      ok = name.length() > 0;
    }
  }
  return ok;
}

void
NewRss::ParseStories (QDomNodeList & items, const QString & contentTag)
{
  int ni = items.count();
  for (int i=0; i<ni; i++) {
    QDomNode item = items.at(i);
    if (item.isElement()) {
      QDomElement elt = item.toElement();
      QDomNodeList titles = elt.elementsByTagName ("title");
      QDomNodeList descrs = elt.elementsByTagName (contentTag);
      if (titles.count() > 0 && descrs.count() > 0) {  // should be 1
        QString title = titles.at(0).toElement().text();
        QString descr = descrs.at(0).toElement().text();
        QString id = headlines.addNewLine (title);
        stories[id] = descr;
        QDomNodeList links = elt.elementsByTagName ("link");
        int nl = links.count();
        QStringList linkList;
        for (int i=0; i<nl; i++) {
          linkList << links.at(i).toElement().text();
        }
        if (linkList.count() > 0) {
          storyLinks[id] = linkList;
        }
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
  qDebug () << " NewRss::resizeEvent";
  qDebug () << "     top size " << size();
  qDebug () << "     qml size " << ui.qmlView->size();
  QMainWindow::resizeEvent (event);
  if (qmlRoot) {
    QMetaObject::invokeMethod (qmlRoot, "setSize",
                 Q_ARG (QVariant, (ui.qmlView->size().width()-2)),
                 Q_ARG (QVariant, (ui.qmlView->size().height())-2));
  }
}

void
NewRss::closeEvent (QCloseEvent *event)
{
  QMessageBox::StandardButton select = 
    QMessageBox::question (this, "Exit", "Really",
                  QMessageBox::No | QMessageBox::Close);
  qDebug () << " Exit select " << select;
  if (select != QMessageBox::Close) {
    event->ignore();
    return;
  }
  Settings().setValue("geometry", saveGeometry());
  Settings().setValue("sizes/qml", ui.qmlView->size());
  Settings().sync();
  QMainWindow::closeEvent (event);
}

void
NewRss::LoadList ()
{
  if (feedlistParser) {
    topFolder.clear ();
    feedlistParser->Read (topFolder, feedListFile);
    FillFeedModel (topFolder, feeds);
  }
}

void
NewRss::FillFeedModel (const Folder & folder, FeedlistModel & model)
{
  int nfee = folder.childFeeds.count();
  for (int i=0; i<nfee; i++) {
    model.addFeed (folder.childFeeds.at(i));
  }
  int nfol = folder.childFolders.count();
  for (int i=0; i<nfol; i++) {
    FillFeedModel (folder.childFolders.at(i), model);
  }
}

void
NewRss::SaveFeedListModel ()
{
  qDebug () << "NewRss::SaveFeedListModel";
  QFile saveFile (feedListFile);
  saveFile.open (QFile::WriteOnly);
  FeedlistWriter writer;
  writer.write (&feeds, &saveFile);
  saveFile.close ();
}

} // namespace
