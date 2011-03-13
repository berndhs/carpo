#ifndef DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H
#define DELIBERATE_QML_FEED_INTERFACE_BOX_IF_H


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
#include <QVariant>
#include <QMap>
#include <QString>

namespace deliberate 
{

class FeedInterface : public QObject
{
Q_OBJECT

public:

  enum ListChoice {
    Choice_List = 0,
    Choice_Index
  };

  FeedInterface (QObject *parent = 0);

  Q_INVOKABLE void storyClicked (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE void feedClicked (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE void storyHold (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE void listOrientation (int orient);
  Q_INVOKABLE void report (const QString & msg);
  Q_INVOKABLE void toggleLists ();

  void SetActive (ListChoice active);

signals:

  void ShowStory (const QString & id);
  void ShowStorySite (const QString & id);
  void ShowFeed (const QString & id);
  void ShowList  (const QString & list);
  void HideList (const QString & list);


private:

  ListChoice    activeList;
  ListChoice    hiddenList;

  QMap <ListChoice, QString> listNames;

};

} // namespace

#endif
