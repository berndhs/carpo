#ifndef DELIBERATE_FEEDLIST_H
#define DELIBERATE_FEEDLIST_H


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


#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QTextStream>

namespace deliberate
{

class StoryMark
{
public:

  QString  readit;
  QString  hash;

  friend QTextStream & operator<< (QTextStream & stream, 
                                   const StoryMark & sm);
};

typedef QList<StoryMark>  StoryMarkList;

class Feed 
{
public:
 
  QString             & values (const QString & key);
  QString               values (const QString & key) const;
  QStringList         & topics ();
  const QStringList   & topics () const;
  StoryMarkList       & storyMarks ();
  const StoryMarkList & storyMarks () const;


  void StreamOut (QTextStream & stream) const;

  friend QTextStream & operator<< (QTextStream & stream,
                                   const Feed & feed);

private:

  QMap <QString, QString>  theValues;
  QStringList              theTopics;
  QList <StoryMark>        theStoryMarks;

};

class Folder
{
public:

  void clear ();

  QString        name;
  QString        hash;
  QList <Feed>   childFeeds;
  QList <Folder> childFolders;

  friend QTextStream & operator<< (QTextStream & stream,
                                   const Folder & folder);
};

} // namespace


#endif
