#include "event-special.h"



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
#include <QPoint>

namespace deliberate
{

ReportEvent::ReportEvent (QObject *parent)
  :QObject (parent)
{
}

bool
ReportEvent::eventFilter (QObject * obj, QEvent * evt)
{
  QEvent::Type  type = evt->type();
  if (type == QEvent::GraphicsSceneWheel) {
    return handleWheelEvent (obj, dynamic_cast<QGraphicsSceneWheelEvent*>(evt));
  } 
  return QObject::eventFilter (obj, evt);
}

bool
ReportEvent::handleWheelEvent (QObject *obj, QGraphicsSceneWheelEvent * wevt)
{
  if (wevt == 0) {
    return false;
  }
  QPointF pos = wevt->scenePos ();
  Qt::Orientation orient = wevt->orientation ();
  int degrees = wevt->delta ();
  wevt->setAccepted (true);
  qDebug () << __PRETTY_FUNCTION__ << this << " emit WheelEvent " ;
  emit WheelEvent (obj, pos, orient, degrees);
  return true;
}

} // namespace
