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

#include "config-edit.h"
#include "deliberate.h"
#include <QHash>
#include <QByteArray>
#include <QDebug>

namespace deliberate {


ConfigItem::ConfigItem ()
  :kind(Kind_None)
{
}

ConfigItem::ConfigItem (const QString & ke, const QVariant & val, Kind ki)
  :key (ke),
   value (val),
   kind (ki)
{
}

QmlConfigEdit::QmlConfigEdit (QObject * parent)
  :QAbstractListModel (parent)
{
  exemptGroups << "sizes" ;
  QHash<int, QByteArray> roles;
  roles[Type_Key] = "key";
  roles[Type_Value] = "value";
  setRoleNames(roles);
}

QmlConfigEdit::~QmlConfigEdit ()
{
}

QVariant
QmlConfigEdit::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }

  int row = index.row();
  if (row < 0 || row >= configRows.count()) {
    return QVariant();
  }
  QVariant retval;
  switch (role) {
  case Qt::DisplayRole:
  case int(Type_Key):
    retval = QVariant(configRows[row].key);
    break;
  case int(Type_Value):
    retval = configRows[row].value;
    break;
  default:  
    break;
  }
  return retval;
}

void
QmlConfigEdit::Run ()
{
  Load ();
}

int 
QmlConfigEdit::rowCount (const QModelIndex & parent) const
{
  Q_UNUSED (parent)
  return configRows.count();
}


void
QmlConfigEdit::Load ()
{
  int nr = rowCount();
  if (nr > 0) {
    removeRows (0,nr-1);
  }
  QSettings & Zett = Settings();
  QString   name;
  QVariant  data;
  QStringList top = Zett.childKeys ();
  QStringList::iterator topit;

  name = QString ("program");
  data = Zett.value (name).toString(); 
  QStringList groups = Zett.childGroups();
  QStringList::iterator  grit, subit;
  for (grit = groups.begin(); grit != groups.end(); grit++) {
    qDebug () << "Settings Group " << *grit;
    addRow (ConfigItem (*grit, QVariant(), ConfigItem::Kind_Header));
    if (exemptGroups.contains (*grit)) {
      qDebug () << "     Exempt";
      continue;
    }
    Zett.beginGroup (*grit);
    QStringList subs = Zett.childKeys ();
    for (subit = subs.begin(); subit != subs.end(); subit++) {
      name = *subit;
      data = Zett.value(name); 
      qDebug () << "     item key  " << name ;
      qDebug () << "          data " << data;
      addRow (ConfigItem (name, data, ConfigItem::Kind_Value));
    }
    Zett.endGroup ();
  }
}

void
QmlConfigEdit::addRow (const ConfigItem & item)
{
  int nr = configRows.count();
  beginInsertRows (QModelIndex(), nr, nr);
  configRows.append (item);
  endInsertRows ();
}


void
QmlConfigEdit::Save ()
{
  //Zett.sync();
}


} // namespace
