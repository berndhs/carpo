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
#include <QDeclarativeItem>
#include <QGraphicsObject>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QObjectList>
#include <QDebug>
#include <QMessageBox>
#include <QSize>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QDir>

#include "feedlist-writer.h"
#include "newrss-magic.h"
#include "deliberate.h"
#include "version.h"

namespace deliberate
{

NewRss::NewRss (QWidget *parent)
  :QMainWindow (parent),
   runAgain (false),
   app (0),
   context (0),
   qmlRoot (0),
   headlines (this),
   feedIF (0),
   qnam (0),
   feedlistParser (0),
   feeds (this),
   configEdit (this),
   propStore (0),
   topicModel (this)
{
  feedListFile =  QDesktopServices::storageLocation 
              (QDesktopServices::DataLocation)
              + QDir::separator()
              + QString ("drss_feeds.xml");
  propStore = new PropertyStore (this, &Settings());
  propStore->Init (":/default-properties.txt");
  feedListFile = Settings().value ("files/feedlist",feedListFile).toString();
  Settings().setValue ("files/feedlist",feedListFile);
  qnam = new QNetworkAccessManager;
  feedlistParser = new FeedlistParser (this);
  ui.setupUi (this);
  htmlString = QString ("<html><head></head>"
                        "<body><h1>HTML String number %1</h1></body>"
                        "</html>");
  feedIF = new FeedInterface (this);
  controlIF = new ControlInterface (this, &feeds);
  restoreGeometry(Settings().value("geometry").toByteArray());
  if (Settings().contains ("sizes/qml")) {
    QSize qmlsize = Settings().value("sizes/qml").toSize();
    qDebug () << " Found QML saved size " << qmlsize;
    ui.qmlView->resize (qmlsize);
  }
  topicModel.SetFeedModel (&feeds);
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
NewRss::DumpProperties ()
{
  QObject *item;
  QStringList names;
  names << "displayArea" << "controlPanel" << "indexBox"
             << "feedListArea" << "feedIndexArea"
             << "storyView" << "feedEditArea" << "feedEdit";
  if (!qmlRoot) {
    qDebug () << " no QML Root object ";
    return;
  }
  qDebug () << "QML Root " << qmlRoot->objectName();
  for (int i=0; i<names.count(); i++) {
    item = qmlRoot->findChild<QObject*>(names.at(i));
    qDebug () << "QML child " << names.at(i) << item;
  }
  QObjectList children = qmlRoot->children ();
  for (int i=0; i<children.count(); i++) {
    QObject * obj = children.at(i);
    qDebug () << " child " << i << obj;
  }
}

void
NewRss::Run ()
{
  qDebug () << " NewRss::Run";
  LoadList ();
  context = ui.qmlView->rootContext ();
  context->setContextProperty ("feedIndexModel", &headlines);
  context->setContextProperty ("feedListModel", &feeds);
  context->setContextProperty ("configModel", &configEdit);
  context->setContextProperty ("topicModel", &topicModel);
  ui.qmlView->setSource (QUrl::fromLocalFile("qml/mainview.qml"));
  context->setContextProperty ("feedIF",feedIF);
  context->setContextProperty ("controlIF",controlIF);
  context->setContextProperty ("configIF",&configEdit);
  qDebug () << " NewRss  context base url " << context->baseUrl();
  qDebug () << " NewRss  engine import paths "
            << ui.qmlView->engine()->importPathList();         
  qDebug () << " NewRss  engine plugin paths "
            << ui.qmlView->engine()->pluginPathList();
  qmlRoot = ui.qmlView->rootObject();       
  qDebug () << " NewRss  engine base url "
            << ui.qmlView->engine()->baseUrl();
  qmlRoot = ui.qmlView->rootObject();
  if (qmlRoot == 0) {
    QMessageBox::critical (this, "Fatal", "QML Load Failure");
    QTimer::singleShot (150, this, SLOT(Quit ()));
    return;
  }
  propStore->ReadFromObjects (qmlRoot);
  propStore->FillSettings (qmlRoot);
  propStore->SyncToObjects (qmlRoot);
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
  configEdit.Load ();
  show ();
}

void
NewRss::Reset ()
{
  propStore->FillSettings (qmlRoot);
  propStore->SyncToObjects (qmlRoot);
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
  connect (ui.actionReset, SIGNAL (triggered()),
           this, SLOT (Reset ()));
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
  connect (&feeds, SIGNAL (ListChanged ()),
           this, SLOT (SaveFeedListModel ()));
  connect (controlIF, SIGNAL (ProbeFeed (const QString &)),
           this, SLOT (ProbeFeed (const QString &)));
  connect (controlIF, SIGNAL (BrowseLinkExternal (const QString &)),
           this, SLOT (ShowStorySite (const QString &)));
  connect (controlIF, SIGNAL (BrowseLinkLocal (const QString &)),
           this, SLOT (ShowStorySiteLocal (const QString &)));
}


void
NewRss::ShowStory (const QString & id)
{
  qDebug () << " NewRss::ShowStory";
  if (stories.contains(id)) {
    QString tagBrowseHere 
               (Magic::PseudoAlertTag + "/here/");
    tagBrowseHere.append (id);
    QString tagBrowseBrowser 
               (Magic::PseudoAlertTag + "/browser/");
    tagBrowseBrowser.append (id);
    QString browseButton ("<button type=\"button\" "
                       "onClick=\"alert('%1')\">%2</button>");
    QString buttons = QString ("<span style=\"font-size:small\"> ")
                     + browseButton.arg(tagBrowseHere).arg("Here")
                     + browseButton.arg(tagBrowseBrowser).arg("Browser")
                     + QString( "</span>");
    qDebug () << " button string " << buttons;
    QString date (tr(" date unknown "));
    if (storyDates.contains(id)) {
      date = storyDates[id];
    } 
    htmlString = QString("<div>")
                        + buttons + date + "</div>" + stories[id];
  } else {
    htmlString = "<b><i>No Such Story!</b></i>";
  }
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, htmlString));
}

void
NewRss::ShowStorySite (const QString & id)
{
  if (storyLinks.contains (id)) {
    QString first = storyLinks[id].first();
    QDesktopServices::openUrl (QUrl::fromUserInput(first));
  }
}

void
NewRss::ShowStorySiteLocal (const QString & id)
{
  qDebug () << "NewRss::ShowStorySiteLocal";
  if (storyLinks.contains (id)) {
    QString urlString = storyLinks[id].first();
    if (qnam) {
      QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
      DrssNetReply * dreply = new DrssNetReply (netreply, 
                                      DrssNetReply::Kind_WebPage);
      expectReplies[netreply] = dreply;
    }
  }
}

void
NewRss::ShowFeed (const QString & id)
{
  qDebug () << " NewRss::ShowFeed";
  QString urlString = feeds.FeedRef(id).values("xmlurl");
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
  QString urlString = feed.values("xmlurl");
  QString nick = feed.values("nick");
  QString siteUrl = feed.values("weburl");
  QString title = feed.values("title");
  QString descr = feed.values("description");
  QString topics = feed.topics().join("; ");
  QMetaObject::invokeMethod (qmlRoot, "displayEditFeed",
                 Q_ARG (QVariant, id),
                 Q_ARG (QVariant, urlString),
                 Q_ARG (QVariant, title),
                 Q_ARG (QVariant, siteUrl),
                 Q_ARG (QVariant, nick),
                 Q_ARG (QVariant, descr),
                 Q_ARG (QVariant, topics));
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
        case DrssNetReply::Kind_WebPage:
          WebPageReply (dreply->netReply());
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
    ParseStories (items, "description", "pubDate");
  }
  if (entries.count() > 0) {
    ParseStories (entries, "summary","published","updated");
  }
  if (headlines.count() > 0) {
    ShowList ("FeedIndex");
    HideList ("FeedList");
    feedIF->SetActive (FeedInterface::Choice_Index);
  }
}

void
NewRss::WebPageReply (QNetworkReply * reply)
{
  if (!reply) {
    return;
  }
  QByteArray pageBytes = reply->readAll();
  htmlString = QString (pageBytes);
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, htmlString));
}

void
NewRss::Restart ()
{
  runAgain = true;
  QTimer::singleShot (250, this, SLOT (Quit()));
}

bool
NewRss::Again ()
{
  bool again = runAgain;
  runAgain = false;
  return again;
}

void
NewRss::ProbeReply (QNetworkReply * reply)
{
  QDomDocument probeDoc;
  probeDoc.setContent (reply);
  QDomElement root = probeDoc.documentElement ();
  QString tag = root.tagName();
  Feed newFeed;
  newFeed.values("xmlurl") = reply->url().toString();
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
  feed.values("title") = title;
  feed.values("weburl") = weblink;
  feed.values("description") = description;
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
  feed.values("title") = title;
  feed.values("weburl") = weblink;
  feed.values("description") = description;
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
NewRss::ParseStories (QDomNodeList & items, const QString & contentTag,
                      const QString & dateTag1, const QString & dateTag2)
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
          QString link = links.at(i).toElement().text().trimmed();
          if (link.length() < 1) { // maybe atom with href in attribute
            link = links.at(i).toElement().attribute("href");
          }
          if (link.length() > 0) {
            linkList << link;
          }
        }
        if (linkList.count() > 0) {
          storyLinks[id] = linkList;
        }
        QString date; // latest pub date
        QDomNodeList dates = elt.elementsByTagName (dateTag1);
        int nd = dates.count();
        if (nd > 0) {
          date = dates.at(0).toElement().text();
        }
        if (dateTag2.length() > 0) {
          dates = elt.elementsByTagName (dateTag2);
          nd = dates.count();
          if (nd > 0) {
            date = dates.at(nd-1).toElement().text();
          }
        }
        if (date.length() > 0) {
          storyDates[id] = date;
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
    CheckExists (feedListFile);
    feedlistParser->Read (topFolder, feedListFile);
    FillFeedModel (topFolder, feeds);
    topicModel.ReIndex ();
  }
}

void
NewRss::CheckExists (const QString & filename)
{
  QFileInfo fileInfo (filename);
  if (!fileInfo.exists()) {
    QDir dir (fileInfo.absolutePath());
    dir.mkpath (fileInfo.absolutePath());
    QFile file (filename);
    file.open (QFile::ReadWrite);
    file.write (QByteArray (""));
    file.close();
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
  topicModel.ReIndex ();
  QFile saveFile (feedListFile);
  saveFile.open (QFile::WriteOnly);
  FeedlistWriter writer;
  writer.write (&feeds, &saveFile);
  saveFile.close ();
}

} // namespace
