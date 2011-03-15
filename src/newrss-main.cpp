
#include "newrss-main.h"

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
#include "deliberate.h"
#include "version.h"
#include "cmdoptions.h"
#include "newrss.h"

#include <QDebug>


int
main (int argc, char *argv[])
{
  QCoreApplication::setOrganizationName ("BerndStramm");
  QCoreApplication::setOrganizationDomain ("bernd-stramm.com");
  QCoreApplication::setApplicationName ("newrss");
  deliberate::ProgramVersion pv ("NewRss");
  QCoreApplication::setApplicationVersion (pv.ShortVersion());

  QApplication  app (argc, argv);

  QSettings  settings;
  deliberate::InitSettings ();
  deliberate::SetSettings (settings);
  settings.setValue ("program",pv.MyName());


  QStringList  configMessages;

  deliberate::CmdOptions  opts ("newrss");

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
  #if 0
  for (int cm=0; cm<configMessages.size(); cm++) {
    deliberate::StdOut () << configMessages[cm] << endl;
  }
  #endif
  if (opts.WantVersion ()) {
    exit (0);
  }
  int result;

  deliberate::NewRss   newrss;

  app.setWindowIcon (newrss.windowIcon());
  newrss.Init (app);
  newrss.AddConfigMessages (configMessages);
  newrss.Run ();
  result = app.exec ();
  return result;
}
