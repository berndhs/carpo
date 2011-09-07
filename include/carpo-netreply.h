#ifndef DELIBERATE_DRSS_NETREPLY_H
#define DELIBERATE_DRSS_NETREPLY_H


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
#include <QString>
#include <QNetworkReply>

namespace deliberate
{

class CarpoNetReply 
{
public:

  enum Kind {
    Kind_None = 0,
    Kind_GetFeed = 1,
    Kind_Probe = 2,
    Kind_WebPage = 3,
    Kind_PollFeed = 4,
    Kind_Bad
  };

  CarpoNetReply ();
  CarpoNetReply (QNetworkReply * nr, Kind k);
  CarpoNetReply (const CarpoNetReply & other);

  QNetworkReply * netReply () const;
  void            setNetReply (QNetworkReply * reply);
  Kind            kind () const;
  QString         feedId ();
  QString         storyHash ();
  int             forwardCount ();
  void            setKind (Kind k);
  void            setFeedId (const QString & feedId);
  void            setStoryHash (const QString & sh);
  void            incrementForwardCount (int increment=1);

private:

  QNetworkReply   *theNetreply;
  Kind             theKind;
  QString          theFeedId;
  QString          theStoryHash;
  int              theForwardCount;
};

} // namespace

#endif
