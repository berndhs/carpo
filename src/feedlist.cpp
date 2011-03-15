#include "feedlist.h"



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

QString &
Feed::values (const QString & key)
{
  return theValues[key];
}

QString
Feed::values (const QString & key) const
{
  return theValues[key];
}

StoryMarkList &
Feed::storyMarks ()
{
  return theStoryMarks;
}

const StoryMarkList &
Feed::storyMarks () const
{
  return theStoryMarks;
}

QTextStream & 
operator<< (QTextStream & stream, 
            const StoryMark & sm)
{
  stream << " (storymark readit " << sm.readit << " hash " << sm.hash << ") ";
  return stream;
}

void
Feed::StreamOut (QTextStream & stream) const
{
  stream << " (Feed theValues (" ;
  QMap<QString, QString>::const_iterator cit;
  for (cit = theValues.constBegin(); cit!= theValues.constEnd(); cit++) {
    stream << " ( \"" << cit.key() << "\" , \"" << cit.value() << "\" ) ";
  }
  stream << ") StoryMarks  (" ;
  int nsm = theStoryMarks.count();
  for (int s=0; s<nsm; s++) {
     stream << " ( " << theStoryMarks.at(s) << " ) ";
  }
  stream << ") ) ";
}

void
Folder::clear ()
{
  childFeeds.clear ();
  childFolders.clear (); 
}

QTextStream &
operator<< (QTextStream & stream, const Feed & feed)
{
  feed.StreamOut (stream);
  return stream;
}

QTextStream &
operator<< (QTextStream & stream, const Folder & folder)
{
  stream << " (Folder name \"" << folder.name
         << "\" hash \"" << folder.hash 
         << "\" feeds  (";
  int nfe = folder.childFeeds.count ();
  for (int f=0; f<nfe; f++) {
    stream << " ( " << folder.childFeeds.at(f) << " ) ";
  }
  int nfo = folder.childFolders.count ();
  for (int f=0; f<nfo; f++) {
    stream << " ( " << folder.childFolders.at(f) << " ) ";
  }
  stream << " ) folders ( " ;
  stream << " ) ) ";
  return stream;
}

} // namespace
