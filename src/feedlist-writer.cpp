#include "feedlist-writer.h"

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
#include <QIODevice>

namespace deliberate 
{

FeedlistWriter::FeedlistWriter ()
{
}

bool
FeedlistWriter::write (FeedlistModel *model, QIODevice *device)
{
  if (!model || !device || !device->isWritable()) {
    return false;
  }
  setDevice (device);
  setAutoFormatting (true);
  setAutoFormattingIndent (1);
  writeStartDocument ("1.0");
  writeStartElement ("drssfeedlist");
  writeAttribute ("version", "2.0");
  FeedlistModel::iterator fit;
  for (fit=model->begin(); fit!= model->end(); ++fit) {
    write (fit.value());
  }
  writeEndElement (); // drssfeedlist
  writeEndDocument ();
}

void
FeedlistWriter::write (Feed & feed)
{
  writeStartElement ("feed");
  writeAttribute ("hash",feed.values("hash"));
  writeTextElement ("title", feed.values("title"));
  writeTextElement ("webUrl",feed.values("weburl"));
  writeTextElement ("xmlUrl",feed.values("xmlurl"));
  writeTextElement ("description",feed.values("description"));
  writeTextElement ("imageUrl",feed.values("imageurl"));
  writeTextElement ("nick",feed.values("nick"));
  writeTextElement ("language",feed.values("language"));
  writeEndElement (); // feed
}

} // namespace
