#ifndef DELIBERATE_FEEDLIST_PARSER_H
#define DELIBERATE_FEEDLIST_PARSER_H


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
#include <QXmlStreamReader>
#include <QMap>
#include <QString>

#include "feedlist.h"

namespace deliberate
{


class FeedlistParser : public QObject
{
Q_OBJECT

public:

  FeedlistParser (QObject *parent=0);

  void Read (Folder & topFolder, const QString & filename);

private:

  void InitFuncPtrs ();

  static QXmlStreamReader::TokenType   
         ReadNext (QXmlStreamReader & xread);

  void   ParseFeedlist (QXmlStreamReader & xread,
                                               Folder & topFolder);
  void   ParseFeed (QXmlStreamReader & xread,
                                           Feed & feed);
  void   ParseFolder (QXmlStreamReader & xread,
                                           Folder & folder);

  static void ParseTextElement (const QString & tag,
                                QXmlStreamReader & xread, 
                                Feed & feed);
  static void ParseStorySig (const QString & tag,
                             QXmlStreamReader & xread, 
                             Feed & feed);
  static void ParseTopic (const QString & tag,
                             QXmlStreamReader & xread, 
                             Feed & feed);

  typedef void (*FieldParseFuncPtr) (const QString &, 
                                     QXmlStreamReader &, 
                                     Feed &);
 

  QXmlStreamReader::TokenType   token;

  QMap <QString, FieldParseFuncPtr>  fileParse;

};

} // namespace

#endif
