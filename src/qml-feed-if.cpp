#include "qml-feed-if.h"
#include <QDebug>


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


namespace deliberate
{

FeedInterface::FeedInterface (QObject *parent)
  :QObject(parent),
   activeList (Choice_List),
   hiddenList (Choice_Index)
{
  listNames[Choice_List] = "FeedList";
  listNames[Choice_Index] = "FeedIndex";
}

void
FeedInterface::storyClicked (int index, const QString & id, const QString & title)
{
  emit ShowStory (id, title);
}

void
FeedInterface::storyHold (int index, const QString & id, const QString & title)
{
  emit ShowStorySite (id);
}

void
FeedInterface::feedClicked (int index, const QString & id, const QString & title)
{
  emit ShowFeed (id);
}

void
FeedInterface::listOrientation (int orient)
{
  qDebug () << "FeedInterface::listOrientation " << orient;
}

void
FeedInterface::report (const QString & msg)
{
  qDebug () << "FeedInterface::report " << msg;
}

void
FeedInterface::toggleLists ()
{
  hiddenList = activeList;
  switch (activeList) {
  case Choice_Index:
    activeList = Choice_List;
  break;
  case Choice_List:
    activeList = Choice_Index;
    break;
  default:
    activeList = Choice_List;
    break;
  }
  emit HideList (listNames[hiddenList]);
  emit ShowList (listNames[activeList]);
}

void
FeedInterface::SetActive (ListChoice active)
{
  activeList = active;
  switch (activeList) {
  case Choice_Index:
    hiddenList = Choice_List;
  break;
  case Choice_List:
    hiddenList = Choice_Index;
    break;
  default:
    hiddenList = Choice_List;
    break;
  }
}


} // namespace
