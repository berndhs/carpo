
#include "carpo-netreply.h"


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
CarpoNetReply::CarpoNetReply ()
  :theNetreply (0),
   theKind (Kind_None)
{
}

CarpoNetReply::CarpoNetReply (QNetworkReply * nr, Kind k)
  :theNetreply (nr),
   theKind (k)
{
}

CarpoNetReply::CarpoNetReply (const CarpoNetReply & other)
  :theNetreply (other.theNetreply),
   theKind (other.theKind)
{
}

QNetworkReply *
CarpoNetReply::netReply () const
{
  return theNetreply;
}

void
CarpoNetReply::setNetReply (QNetworkReply * reply)
{
  theNetreply = reply;
}

CarpoNetReply::Kind
CarpoNetReply::kind () const
{
  return theKind;
}

void
CarpoNetReply::setKind (Kind k)
{
  theKind = k;
}

QString
CarpoNetReply::feedId ()
{
  return theFeedId;
}

void
CarpoNetReply::setFeedId (const QString & feedId)
{
  theFeedId = feedId;
}

QString
CarpoNetReply::storyHash ()
{
  return theStoryHash;
}

void
CarpoNetReply::setStoryHash (const QString & sh)
{
  theStoryHash = sh;
}

} // namespace
