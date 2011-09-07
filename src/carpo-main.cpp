
#include "carpo-main.h"

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2010, Bernd Stramm
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


#include <QApplication>
#include <QIcon>
#include "deliberate.h"
#include "delib-debug.h"
#include "version.h"
#include "cmdoptions.h"
#include "carpo.h"
#include "orientation.h"

#include <QDebug>


int
main (int argc, char *argv[])
{
  QCoreApplication::setOrganizationName ("BerndStramm");
  QCoreApplication::setOrganizationDomain ("carpo.sourceforge.net");
  QCoreApplication::setApplicationName ("carpo");
  deliberate::ProgramVersion pv ("Carpo");
  QCoreApplication::setApplicationVersion (pv.ShortVersion());

  QApplication  app (argc, argv);
  
  const char utilUri[] = "moui.geuzen.utils.static";

  qmlRegisterType<geuzen::OrientationWatcher>(utilUri, 1, 0,
                 "GeuzenOrientation");

  QSettings  settings;
  deliberate::InitSettings ();
  deliberate::SetSettings (settings);
  settings.setValue ("program",pv.MyName());


  QStringList  configMessages;

  deliberate::CmdOptions  opts ("carpo");
  opts.AddSoloOption ("debug","D",QObject::tr("show Debug log window"));
  opts.AddStringOption ("logdebug","L",QObject::tr("write Debug log to file"));

  //deliberate::UseMyOwnMessageHandler ();

  bool optsOk = opts.Parse (argc, argv);
  if (!optsOk) {
    opts.Usage ();
    exit(1);
  }
  if (opts.WantHelp ()) {
    opts.Usage ();
    exit (0);
  }
  pv.CLIVersion ();
  configMessages.append (QString("Built on %1 %2")
                         .arg (__DATE__).arg(__TIME__));
  configMessages.append (QObject::tr("Build with Qt %1").arg(QT_VERSION_STR));
  configMessages.append (QObject::tr("Running with Qt %1").arg(qVersion()));

  if (opts.WantVersion ()) {
    exit (0);
  }
  bool showDebug = opts.SeenOpt ("debug");
  int result;

#if DELIBERATE_DEBUG
  deliberate::StartDebugLog (showDebug);
  bool logDebug = opts.SeenOpt ("logdebug");
  if (logDebug) {
    QString logfile ("/dev/null");
    opts.SetStringOpt ("logdebug",logfile);
    deliberate::StartFileLog (logfile);
  }
#endif
  deliberate::Carpo   carpo;

  QIcon appIcon ;
  appIcon.addFile (QString (":/images/icon256.png"), QSize (256,256));
  appIcon.addFile (QString (":/images/icon128.png"), QSize (128,128));
  appIcon.addFile (QString (":/images/icon64.png"), QSize (64,64));
  appIcon.addFile (QString (":/images/icon48.png"), QSize (48,48));
  appIcon.addFile (QString (":/images/icon32.png"), QSize (32,32));
  appIcon.addFile (QString (":/images/icon16.png"), QSize (16,16));
  qDebug () << __PRETTY_FUNCTION__ << " loaded icon null " << appIcon.isNull();
  qDebug () << __PRETTY_FUNCTION__ << "   sizes " << appIcon.availableSizes();
  app.setWindowIcon (appIcon);
  carpo.setWindowIcon (appIcon);
  carpo.Init (app);
  carpo.AddConfigMessages (configMessages);
  carpo.Run ();
  result = app.exec ();
  return result;
}
