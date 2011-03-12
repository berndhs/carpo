#include "qml-control-if.h"
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

ControlInterface::ControlInterface (QObject *parent)
  :QObject(parent),
   isEditingFeed (false)
{
}

void
ControlInterface::setEditingFeed (bool editing)
{
  isEditingFeed = editing;
  qDebug () << "ControlInterface::setEditingFeed " << editing;
}

bool
ControlInterface::editingFeed ()
{
  return isEditingFeed;
}

void
ControlInterface::feedClicked (int index, const QString & ident, 
                                            const QString & title)
{
  Q_UNUSED (index)
  Q_UNUSED (title)
  if (isEditingFeed) {
    emit EditFeed (ident);
  } else {
    emit ShowFeed (ident);
  }
}

} // namespace
