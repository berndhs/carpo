#include "carpo.h"

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
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QDir>
#include <QCryptographicHash>
#include <QWebView>

#include "feedlist-writer.h"
#include "carpo-magic.h"
#include "deliberate.h"
#include "version.h"

namespace deliberate
{

Carpo::Carpo (QWidget *parent)
  :QMainWindow (parent),
   runAgain (false),
   app (0),
   context (0),
   qmlRoot (0),
   headlines (this),
   feedIF (0),
   controlIF (0),
   gestureIF (0),
   qnam (0),
   feedlistParser (0),
   feeds (this),
   configEdit (this),
   propStore (0),
   topicModel (this),
   autoUpdate (feeds, this),
   saveTimer (this)
{
  setObjectName ("CarpoMainObject");
  feedListFile =  QDesktopServices::storageLocation 
              (QDesktopServices::DataLocation)
              + QDir::separator()
              + QString ("carpo_feeds.xml");
  propStore = new PropertyStore (this, &Settings());
  propStore->Init (":/default-properties.txt");
  feedListFile = Settings().value ("files/feedlist",feedListFile).toString();
  Settings().setValue ("files/feedlist",feedListFile);
  qnam = new QNetworkAccessManager;
  feedlistParser = new FeedlistParser (this);
  ui.setupUi (this);
  ui.qmlView->setResizeMode (QDeclarativeView::SizeRootObjectToView);
  htmlString = QString ("<html><head></head>"
                        "<body><h1>HTML String number %1</h1></body>"
                        "</html>");
  feedIF = new FeedInterface (this);
  controlIF = new ControlInterface (this, &feeds);
  gestureIF = new GestureInterface (this);
  reporter = new ReportEvent (this);
  installEventFilter (reporter);
  restoreGeometry(Settings().value("geometry").toByteArray());
  if (Settings().contains ("sizes/qml")) {
    QSize qmlSize (800,600);
    qmlSize = Settings().value("sizes/qml", qmlSize).toSize();
    qDebug () << " Found QML saved size " << qmlSize;
    ui.qmlView->resize (qmlSize);
  }
  topicModel.SetFeedModel (&feeds);
  connect (&saveTimer, SIGNAL (timeout()), this, SLOT (MaybeSave()));
  saveTimer.start (5*60*1000);  // 5 minutes
  Connect ();
}

void
Carpo::Init (QApplication & ap)
{
  app = &ap;
}

void
Carpo::AddConfigMessages (const QStringList & messages)
{
  configMessages = messages;
}

void
Carpo::DumpProperties ()
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
Carpo::Run ()
{
  QmlRun ();
}

void
Carpo::QmlRun ()
{
  qDebug () << " Carpo::QmlRun";
  LoadList ();
  context = ui.qmlView->rootContext ();
  context->setContextProperty ("feedIndexModel", &headlines);
  context->setContextProperty ("feedListModel", &feeds);
  context->setContextProperty ("configModel", &configEdit);
  context->setContextProperty ("topicModel", &topicModel);
  context->setContextProperty ("streamListModel", &autoUpdate);
  context->setContextProperty ("feedIF",feedIF);
  context->setContextProperty ("controlIF",controlIF);
  context->setContextProperty ("configIF",&configEdit);
  context->setContextProperty ("gestureIF",gestureIF);

  ui.qmlView->setSource (QUrl("qrc:///qml/DefaultMain.qml"));
  //ui.qmlView->setSource (QUrl::fromLocalFile("qml/DefaultMain.qml"));
  qmlRoot = ui.qmlView->rootObject();
  if (qmlRoot == 0) {
    QMessageBox::critical (this, "Fatal", "QML Load Failure");
    QTimer::singleShot (150, this, SLOT(Quit ()));
    return;
  }
  qmlRoot->installEventFilter (reporter);
  controlIF->SetQmlRoot (qobject_cast<QDeclarativeItem*> (qmlRoot));
  QDeclarativeItem * qmlWebView = qmlRoot->
                     findChild<QDeclarativeItem*>("StoryView");
  controlIF->SetQmlWeb (qmlWebView);

  if (gestureIF) {
    gestureIF->SetQmlRoot (qmlRoot);
  }
  propStore->ReadFromObjects (qmlRoot);
  propStore->FillSettings (qmlRoot);
  propStore->SyncToObjects (qmlRoot);
  QSize topSize = size();
  if (qmlRoot) {
    QMetaObject::invokeMethod (qmlRoot, "setSize",
                 Q_ARG (QVariant, (topSize.width()-2)),
                 Q_ARG (QVariant, (topSize.height())-2));
  }
  ShowList ("FeedList");
  topFolder.clear ();
  configEdit.Load ();
  show ();
  autoUpdate.Init ();
  int streamDelay (15);
  streamDelay = Settings().value ("timers/newspoll",streamDelay).toInt();
  Settings().setValue ("timers/newspoll",streamDelay);
  autoUpdate.Start (streamDelay*1000);
}

void
Carpo::Reset ()
{
  propStore->FillSettings (qmlRoot);
  propStore->SyncToObjects (qmlRoot);
}

void
Carpo::Connect ()
{
  connect (qnam, SIGNAL (finished (QNetworkReply *)),
           this, SLOT (FinishedNet (QNetworkReply *)));
  connect (feedIF, SIGNAL (ShowStory (const QString &, const QString &)),
           this, SLOT (ShowStory (const QString &, const QString &)));
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
           this, SLOT (MaybeSave ()));
  connect (&feeds, SIGNAL (ListChanged ()),
           this, SLOT (SaveFeedListModel ()));
  connect (&feeds, SIGNAL (TopicChanged ()),
           this, SLOT (TopicChanged ()));
  connect (controlIF, SIGNAL (ProbeFeed (const QString &)),
           this, SLOT (ProbeFeed (const QString &)));
  connect (controlIF, SIGNAL (BrowseLinkExternal (const QString &)),
           this, SLOT (ShowStorySite (const QString &)));
  connect (controlIF, SIGNAL (BrowseLinkLocal (const QString &)),
           this, SLOT (ShowStorySiteLocal (const QString &)));
  connect (controlIF, SIGNAL (DisplayStory (const QString &,
                                            const QString &,
                                            const QString &)),
           this, SLOT (DisplayStory (const QString &,
                                            const QString &,
                                            const QString &)));
  connect (controlIF, SIGNAL (Restart ()),
           this, SLOT (Restart ()));
  connect (controlIF, SIGNAL (Exit()),
           this, SLOT (Quit()));
  connect (controlIF, SIGNAL (GetHelp ()),
           this, SLOT (ShowAbout()));
  connect (controlIF, SIGNAL (ShowLicense ()),
           this, SLOT (ShowLicense()));
  connect (controlIF, SIGNAL (ShowManual ()),
           this, SLOT (ShowManual ()));
  connect (controlIF, SIGNAL (ImportFeeds (const QString &)),
           this, SLOT (ImportList (const QString &)));
  connect (&autoUpdate, SIGNAL (NewestRow (int)),
           this, SLOT (NewestNewsRow (int)));
  connect (reporter, SIGNAL (WheelEvent 
                 (QObject *, QPointF, Qt::Orientation, int)),
           this, SLOT (HandleWheelEvent 
                 (QObject *, QPointF, Qt::Orientation, int)));
}


void
Carpo::ShowStory (const QString & id, const QString & title)
{
  qDebug () << " Carpo::ShowStory " << id << title;
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
                      + browseButton
                        .arg(tagBrowseHere)
                        .arg(tr("Here"))
                      + browseButton
                        .arg(tagBrowseBrowser)
                        .arg(tr("Browser"))
                      + QString( "</span>");
    QString date (tr(" date unknown "));
    if (storyDates.contains(id)) {
      date = storyDates[id];
    } 
    QString realTitle (title);
    if (realTitle.length() < 1) {
      if (titles.contains(id)) {
        realTitle = titles[id];
      }
    }
    htmlString = QString("<div>")
                 + buttons 
                 + QString ("<span style=\"font-size:smaller\">%1</span>")
                   .arg(date )
                 + QString ("&nbsp;<i>%1</i>")
                  .arg(realTitle)
                 + "</div>" 
                 + stories[id];
  } else {
    htmlString = "<b><i>No Such Story!</b></i>";
  }
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, htmlString));
  currentStory = id;
  feeds.MarkRead (currentFeed, stories[id], true);
  headlines.markRead (id, true);
  DebugProperty ("FeedIndexArea","currentIndex");
}

void
Carpo::DisplayStory (const QString & feedId,
                      const QString & storyTitle,
                      const QString & storyHash)
{
  Q_UNUSED (storyTitle)
  qDebug () << " Carpo::DisplayStory";
  ShowFeed (feedId, storyHash);
}

void
Carpo::ShowStorySite (const QString & id)
{
  if (storyLinks.contains (id)) {
    QString first = storyLinks[id].first();
    QDesktopServices::openUrl (QUrl::fromUserInput(first));
  }
}

void
Carpo::ShowStorySiteLocal (const QString & id)
{
  if (storyLinks.contains (id)) {
    QString urlString = storyLinks[id].first();
    QMetaObject::invokeMethod (qmlRoot, "setTheUrl",
                   Q_ARG (QVariant, urlString));
  }
}

void
Carpo::ShowFeed (const QString & id, const QString & storyHash)
{
  qDebug () << "Carpo  :: ShowFeed " << id << storyHash;
  QString urlString = feeds.FeedRef(id).values("xmlurl");
  headlines.StartNew (feeds.FeedRef(id).values("title"));
  LoadFeed (urlString, storyHash);
  currentFeed = id;
}

void
Carpo::EditFeed (const QString & id)
{
  Feed & feed = feeds.FeedRef (id);
  DisplayEditFeed (id, feed);
}

void
Carpo::DisplayEditFeed (const QString & id, const Feed & feed)
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
Carpo::ShowList (const QString & list)
{
  QMetaObject::invokeMethod (qmlRoot, 
                             QString("expand%1").arg(list).toLatin1().data());
}

void
Carpo::ShowAbout ()
{
  QString about (QString("<pre>\n%1\n</pre>")
                .arg(ProgramVersion::Version()
                      + "\r\n\r\n"
                      + configMessages.join("\r\n")));
  QString button ("<button type=\"button\" "
                       "onClick=\"alert('%1')\">%2</button>");
  QString licenseButton (button
             .arg (Magic::PseudoAlertTag + "/license/")
             .arg (tr("License")));
  QString manualButton (button
             .arg (Magic::PseudoAlertTag + "/manual/")
             .arg (tr("Manual")));
  about.append (QString ("<br><div>%1&nbsp;%2</div>")
             .arg (licenseButton).arg (manualButton));
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                 Q_ARG (QVariant, about));
}

void
Carpo::ShowLicense ()
{
  QFile licfile (":/help/LICENSE.txt");
  licfile.open (QFile::ReadOnly);
  QByteArray lictext = licfile.readAll();
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                Q_ARG (QVariant, "<pre>\n" + lictext + "\n</pre>\n"));
}

void
Carpo::ShowManual ()
{
  QFile manuFile (":/help/manual.html");
  manuFile.open (QFile::ReadOnly);
  QByteArray manuHtml = manuFile.readAll();
  QMetaObject::invokeMethod (qmlRoot, "setTheHtml",
                Q_ARG (QVariant,QString (manuHtml)));
}

void
Carpo::HideList (const QString & list)
{
  QMetaObject::invokeMethod (qmlRoot, 
                            QString("shrink%1").arg(list).toLatin1().data());
}

void
Carpo::ShrinkIndex ()
{
  HideList ("FeedIndex");
}

void
Carpo::ExpandIndex ()
{
  ShowList ("FeedIndex");
}

void
Carpo::TopicChanged ()
{
  HideList ("FeedIndex");
  ShowList ("FeedList");
}

void
Carpo::Quit ()
{
  if (feeds.dirty ()) {
    SaveFeedListModel (false);
  }
  Settings().setValue("geometry", saveGeometry());
  Settings().setValue("sizes/qml", ui.qmlView->size());
  Settings().sync();
  if (app) {
    app->quit();
  }
}

void
Carpo::LoadFeed (const QString & urlString, const QString & storyHash)
{
  if (qnam) {
    QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
    CarpoNetReply * dreply = new CarpoNetReply (netreply, 
                                      CarpoNetReply::Kind_GetFeed);
    dreply->setStoryHash (storyHash);
    expectReplies[netreply] = dreply;
    if (controlIF) {
      controlIF->SetLoading (true);
    }
  }
}

void
Carpo::ProbeFeed (const QString & urlString)
{
  if (qnam) {
    QNetworkReply * netreply = qnam->get (QNetworkRequest (QUrl (urlString)));
    CarpoNetReply * dreply = new CarpoNetReply (netreply, 
                                      CarpoNetReply::Kind_Probe);
    expectReplies[netreply] = dreply;
    if (controlIF) {
      controlIF->SetLoading (true);
    }
  }
}

void
Carpo::FinishedNet (QNetworkReply * reply)
{
  if (reply) {
    if (expectReplies.contains (reply)) {
      if (controlIF) {
        controlIF->SetLoading (false);
      }
      CarpoNetReply * dreply = expectReplies[reply];
      if (dreply) {
        CarpoNetReply::Kind  kind = dreply->kind();
        switch (kind) {
        case CarpoNetReply::Kind_GetFeed:
          GetFeedReply (dreply->netReply(), dreply->storyHash());
          break;
        case CarpoNetReply::Kind_Probe:
          ProbeReply (dreply->netReply());
          break;
        case CarpoNetReply::Kind_WebPage:
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
Carpo::GetFeedReply (QNetworkReply * reply, const QString & storyHash)
{
  if (!reply) {
    return;
  }
  feedDoc.setContent (reply);
  QDomNodeList items = feedDoc.elementsByTagName ("item");
  QDomNodeList entries = feedDoc.elementsByTagName ("entry");
  stories.clear ();
  titles.clear ();
  storyLinks.clear ();
  storyDates.clear ();
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
  if (storyHash.length() > 0) {
    QMap<QString, QString>::iterator sit;
    for (sit = stories.begin(); sit != stories.end(); sit++) {
      if (storyHash == QCryptographicHash::hash (sit.value().toUtf8().data(), 
                           QCryptographicHash::Md5).toHex()) {
        ShowStory (sit.key());
        break;
      }
    }
  }
}

void
Carpo::WebPageReply (QNetworkReply * reply)
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
Carpo::NewestNewsRow (int row)
{
  QMetaObject::invokeMethod (qmlRoot, "newestNewsRow",
                 Q_ARG (QVariant, row));
}

void
Carpo::Restart ()
{
  qDebug () << "Carpo :: Restart";
  autoUpdate.Stop ();
  controlIF->SetQmlRoot (0);
  controlIF->SetQmlWeb (0);
  controlIF->ClearHtmlStack ();
  QTimer::singleShot (100, this, SLOT (QmlRun()));
}

bool
Carpo::Again ()
{
  bool again = runAgain;
  runAgain = false;
  return again;
}

void
Carpo::ProbeReply (QNetworkReply * reply)
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
Carpo::PopulateFromRssDoc (QDomElement & el, Feed & feed)
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
Carpo::PopulateFromAtomDoc (QDomElement & el, Feed & feed)
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
Carpo::ParseAtomLinkElem (QDomElement & el, QString & xml, QString & web)
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
Carpo::ParseAtomAuthorElem (QDomElement &el, QString & name)
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
Carpo::ParseStories (QDomNodeList & items, const QString & contentTag,
                      const QString & dateTag1, const QString & dateTag2)
{
  int ni = items.count();
  for (int i=0; i<ni; i++) {
    QDomNode item = items.at(i);
    if (item.isElement()) {
      QDomElement elt = item.toElement();
      QDomNodeList titleNodes = elt.elementsByTagName ("title");
      QDomNodeList descNodes = elt.elementsByTagName (contentTag);
      if (titleNodes.count() > 0 && descNodes.count() > 0) {  // should be 1
        QString title = titleNodes.at(0).toElement().text();
        QString descr = descNodes.at(0).toElement().text();
        QString hash (QCryptographicHash::hash (descr.toUtf8().data(), 
                           QCryptographicHash::Md5).toHex());
        bool seenit = feeds.Seenit (currentFeed, hash);
        QString id = headlines.addNewLine (title, seenit);
        stories[id] = descr;
        if (title.length() > 0) {
          titles[id] = title;
        }
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
Carpo::resizeEvent (QResizeEvent *event)
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
Carpo::closeEvent (QCloseEvent *event)
{
  if (feeds.dirty ()) {
    SaveFeedListModel (false);
  }
  Settings().setValue("geometry", saveGeometry());
  Settings().setValue("sizes/qml", ui.qmlView->size());
  Settings().sync();
  QMainWindow::closeEvent (event);
}

void
Carpo::LoadList ()
{
  if (feedlistParser) {
    topFolder.clear ();
    feeds.clear ();
    CheckExists (feedListFile);
    feedlistParser->Read (topFolder, feedListFile);
    FillFeedModel (topFolder, feeds);
    topicModel.ReIndex ();
  }
}

void
Carpo::ImportList (const QString & format)
{
  Folder tempFolder;
  QString fileName = QFileDialog::getOpenFileName (this,
             QString (tr("Open %1 file")).arg(format));
  if (fileName.length() > 0) {
    if (format == "DRSS") {
      CheckExists (feedListFile);
      feedlistParser->Read (tempFolder, fileName);
      FillFeedModel (tempFolder, feeds);
      topicModel.ReIndex ();
      feeds.changeTopic (Magic::AllTopicsTag);
    }
  }
}

void
Carpo::CheckExists (const QString & filename)
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
Carpo::FillFeedModel (const Folder & folder, 
                       FeedlistModel & model,
                       const QStringList & parentFolders)
{
  int nfee = folder.childFeeds.count();
  QStringList importFolders (parentFolders);
  for (int i=0; i<nfee; i++) {
    Feed feed = folder.childFeeds.at(i);
    feed.topics().append (parentFolders);
    model.addFeed (feed);
  }
  int nfol = folder.childFolders.count();
  for (int i=0; i<nfol; i++) {
    QStringList folders (parentFolders);
    folders.append (folder.name);
    FillFeedModel (folder.childFolders.at(i), model, folders);
  }
}

void
Carpo::MaybeSave ()
{
  if (feeds.dirty()) {
    SaveFeedListModel (false);
  }
}

void
Carpo::SaveFeedListModel (bool reindex)
{
  qDebug () << " Carpo  ::  SaveFeedListModel " << reindex;
  if (reindex) {
    topicModel.ReIndex ();
  }
   
  QFile saveFile (feedListFile);
  saveFile.open (QFile::WriteOnly);
  FeedlistWriter writer;
  writer.write (&feeds, &saveFile);
  saveFile.close ();
  feeds.setDirty (false);
}

void
Carpo::DebugProperty (const QString & objName, const QString & propName)
{
  if (qmlRoot == 0) {
    return;
  }
  QDeclarativeItem * item = qmlRoot->findChild<QDeclarativeItem*> (objName);
  if (item) {
    QVariant prop = item->property (propName.toAscii().data());
    qDebug () << "Carpo:  obj " << objName << propName << " is " << prop;
  }
}
void
Carpo::HandleWheelEvent (QObject *detectObject,
                         QPointF pos,
                         Qt::Orientation orientation,
                         int  delta)
{
  Q_UNUSED (detectObject)
  if (qmlRoot) {
    QMetaObject::invokeMethod (qmlRoot, "wheelTurned",
                 Q_ARG (QVariant, pos.x()),
                 Q_ARG (QVariant, pos.y()),
                 Q_ARG (QVariant, orientation),
                 Q_ARG (QVariant, delta));
  }
}

} // namespace
