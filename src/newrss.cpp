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
#include <QObject>
#include <QDebug>

namespace deliberate
{

NewRss::NewRss (QWidget *parent)
  :QMainWindow (parent),
   app (0),
   context (0),
   uiObject (0),
   headlines (this)
{
  ui.setupUi (this);
  htmlString = QString ("<html><head></head>"
                        "<body><h1>HTML String number %1</h1></body>"
                        "</html>");
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
  headlines.addLine ("Monday","Beef");
  headlines.addLine ("Wednesday","Pork");
  headlines.addLine ("Friday","Fish");
  context->setContextProperty ("displayModel", &headlines);
  ui.qmlView->setSource (QUrl::fromLocalFile("qml/mainview.qml"));
  uiObject = ui.qmlView->rootObject();
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
qDebug () << "NewRss::Load " << context << uiObject;
  if (context) {
    if (uiObject) {
qDebug () << " sending html string " << htmlString;
      QMetaObject::invokeMethod (uiObject, "setTheHtml",
                    Q_ARG (QVariant, htmlString.arg (count++)));
      headlines.addLine ("Saturday","Cheese");
    }
  }
}

void
NewRss::Quit ()
{
  if (app) {
    app->quit();
  }
}

} // namespace
