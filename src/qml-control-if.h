#ifndef DELIBERATE_QML_CONTROL_INTERFACE_BOX_IF_H
#define DELIBERATE_QML_CONTROL_INTERFACE_BOX_IF_H


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
#include <QString>
#include "feedlist-model.h"

namespace deliberate 
{

class ControlInterface : public QObject
{
Q_OBJECT

public:

  ControlInterface (QObject *parent = 0, FeedlistModel *feedList=0);

  Q_INVOKABLE void setEditingFeed (bool editing);
  Q_INVOKABLE void feedClicked (int index, const QString & ident, 
                                            const QString & title);
  Q_INVOKABLE bool editingFeed ();
  Q_INVOKABLE void saveFeed (const QString & feedId,
                             const QString & feedUrl,
                             const QString & title,
                             const QString & siteUrl,
                             const QString & nick,
                             const QString & description,
                             const QString & topics);
  Q_INVOKABLE void removeFeed (const QString & feedId);
  Q_INVOKABLE void probeFeed (const QString & url);
  Q_INVOKABLE void checkAlert (const QString & alert);
  Q_INVOKABLE void moveUp (const QString & id);
  Q_INVOKABLE void moveDown (const QString & id);
  Q_INVOKABLE void changeTopic (const QString & newTopic);

  void SetFeeds (FeedlistModel * feedList);

signals:

  void ShowFeed (const QString & feedId);
  void EditFeed (const QString & feedId);
  void ProbeFeed (const QString & url);
  void BrowseLinkExternal (const QString & storyId);
  void BrowseLinkLocal (const QString & storyId);
  void ListUpdated ();

private:

  void handlePseudoAlert (const QString & alert);

  bool     isEditingFeed;
  FeedlistModel   *feeds;

};

} // namespace

#endif
