#include "feedlist-parser.h"


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


#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

namespace deliberate
{

FeedlistParser::FeedlistParser (QObject *parent)
  :QObject (parent)
{
  InitFuncPtrs ();
}

void
FeedlistParser::InitFuncPtrs ()
{
  fileParse["language"] = ParseTextElement;
  fileParse["xmlurl"] = ParseTextElement;
  fileParse["title"] = ParseTextElement;
  fileParse["description"] = ParseTextElement;
  fileParse["imageurl"] = ParseTextElement;
  fileParse["nick"] = ParseTextElement;
  fileParse["weburl"] = ParseTextElement;
  fileParse["storysig"] = ParseStorySig;
}

void
FeedlistParser::Read ()
{
  qDebug () << "FeedlistParser::Read " ;
  QFile file ("drss_feeds.xml");
  bool ok = file.open (QFile::ReadOnly);
  if (!ok) {
    qDebug () << " cannot open file " << file.fileName();
  }
  QXmlStreamReader  xread (&file);
  
  qDebug () << " parser says " << ok;
  bool done (false);
  QString tag;
  while (!done) {
    token = ReadNext (xread);
    done = xread.atEnd() || xread.hasError();
    if (done) {
      break;
    }
    switch (token) {
    case QXmlStreamReader::StartElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Top  start of " << tag;
      if (tag == "drssfeedlist") {
        ParseFeedlist (xread);
      }
      break;
    case QXmlStreamReader::EndElement:
      tag = xread.name().toString();
      qDebug () << " Top   end of " << tag;
      break;
    default:
      qDebug () << " Top other token";
      break;
    }
  }
}

void
FeedlistParser::ParseFeedlist (QXmlStreamReader & xread)
{
  bool done (false);
  QString tag;
  Folder  topFolder;
  while (!done) {
    Folder subFolder;
    Feed   feed;
    token = ReadNext (xread);
    done = xread.atEnd() || xread.hasError();
    if (done) {
      break;
    }
    switch (token) {
    case QXmlStreamReader::StartElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Feedlist  start of " << tag;
      if (tag == "folder") {
        ParseFolder (xread, subFolder);
        topFolder.childFolders.append (subFolder);
      } else if (tag == "feed") {
        ParseFeed (xread, feed);
        topFolder.childFeeds.append (feed);
      }
      break;
    case QXmlStreamReader::EndElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Feedlist   end of " << tag;
      if (tag == "drssfeedlist") {
        done = true;
      }
      break;
    default:
      qDebug () << " Feedlist other token";
      break;
    }
  }
  qDebug () << " ======== done Feedlist";
}

void
FeedlistParser::ParseFeed (QXmlStreamReader & xread, Feed & feed)
{
  QXmlStreamAttributes atts = xread.attributes();
  QString feedHash = atts.value ("hash").toString();
  qDebug () << " Feed  hash " << feedHash;
  bool done (false);
  QString tag;
  while (!done) {
    token = ReadNext (xread);
    done = xread.atEnd() || xread.hasError();
    if (done) {
      break;
    }
    switch (token) {
    case QXmlStreamReader::StartElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Feed  start of " << tag;
      if (fileParse.contains (tag)) {
        fileParse[tag](tag, xread,feed);
      } else {
        qDebug () << " Feed UNKOOWN tag " << tag;
      }
      break;
    case QXmlStreamReader::EndElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Feed   end of " << tag;
      if (tag == "feed") {
        done = true;
      }
      break;
    default:
      qDebug () << " Feed other token";
      break;
    }
  }
  qDebug () << " ........ done Feed";
  qDebug () << feed.values;
  int ns = feed.storyMarks.count();
  for (int s=0; s<ns; s++) {
    qDebug () << "   storysig " << feed.storyMarks.at(s).readit 
                                << feed.storyMarks.at(s).hash;
  }
}

void
FeedlistParser::ParseFolder (QXmlStreamReader & xread, Folder & folder)
{
  QXmlStreamAttributes atts = xread.attributes();
  folder.name = atts.value ("name").toString();
  folder.hash = atts.value ("hash").toString();
  qDebug () << " Folder name " << folder.name << " hash " << folder.hash;
  bool done (false);
  QString tag;
  while (!done) {
    Folder  subFolder;
    Feed    feed;
    token = ReadNext (xread);
    done = xread.atEnd() || xread.hasError();
    if (done) {
      break;
    }
    switch (token) {
    case QXmlStreamReader::StartElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Folder  start of " << tag;
      if (tag == "feed") {
        ParseFeed (xread, feed);
        folder.childFeeds.append (feed);
      } else if (tag == "folder") {
        ParseFolder (xread, subFolder);
        folder.childFolders.append (subFolder);
      }
      break;
    case QXmlStreamReader::EndElement:
      tag = xread.name().toString().toLower();
      qDebug () << " Folder   end of " << tag;
      if (tag == "folder") {
        done = true;
      }
      break;
    default:
      qDebug () << " Folder other token";
      break;
    }
  }
  qDebug () << " -------- done Folder";
}

QXmlStreamReader::TokenType
FeedlistParser::ReadNext (QXmlStreamReader & xread)
{
  QXmlStreamReader::TokenType  tokt;
  tokt = xread.readNext ();
  qDebug () << "ReadNext token  " << tokt << xread.tokenString();
  return tokt;
}

void
FeedlistParser::ParseTextElement (const QString & tag,
                                  QXmlStreamReader & xread, 
                                  Feed & feed)
{
  QXmlStreamReader::TokenType  token;
  token = ReadNext (xread);
  if (token != QXmlStreamReader::Characters) {
    return;
  }
  QString text = xread.text().toString();
  qDebug () << " text element text " << text;
  feed.values[tag] = text;
}

void
FeedlistParser::ParseStorySig (const QString & tag,
                                  QXmlStreamReader & xread, 
                                  Feed & feed)
{ 
  Q_UNUSED (tag)
  QXmlStreamAttributes atts = xread.attributes();
  StoryMark  mark;
  mark.readit = atts.value("readit").toString();
  mark.hash = atts.value ("hash").toString();
  feed.storyMarks.append (mark);
}

} // namespace
