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
#include <QMessageBox>
#include <QDebug>

namespace deliberate {


QmlConfigEdit::ConfigItem::ConfigItem ()
  :group(""),key(""),value(""),kind(Kind_None), level (-1)
{
}

QmlConfigEdit::ConfigItem::ConfigItem (const QString & gr,
                                       const QString & ke, 
                                       const QVariant & val, 
                                       QmlConfigEdit::ConfigItem::Kind ki,
                                       int lev)
  :group(gr),
   key (ke),
   value (val),
   kind (ki),
   level (lev)
{
}

QmlConfigEdit::ConfigItem::ConfigItem (const QmlConfigEdit::ConfigItem & other)
  :group (other.group),
   key (other.key),
   value (other.value),
   kind (other.kind),
   level (other.level)
{
}

QmlConfigEdit::QmlConfigEdit (QObject * parent)
  :QAbstractListModel (parent)
{
  exemptGroups << "sizes" ;
  QHash<int, QByteArray> roles;
  roles[Type_Group] = "confGroup";
  roles[Type_Key] = "confKey";
  roles[Type_Value] = "confValue";
  roles[Type_HasValue] = "confHasValue";
  roles[Type_Level]  = "confLevel";
  setRoleNames(roles);
  connect (this, SIGNAL (rowsInserted(const QModelIndex &, int, int)),
           this, SLOT (didInsertRows  (const QModelIndex &, int, int)));
  connect (this, SIGNAL (modelReset()),
           this, SLOT (didResetModel()));
  connect (this, SIGNAL (dataChanged (const QModelIndex &, const QModelIndex &)),
           this, SLOT (didChangeData (const QModelIndex &, const QModelIndex &)));
}

QmlConfigEdit::~QmlConfigEdit ()
{
}

QVariant
QmlConfigEdit::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    qDebug () << " Bad Index " << index;
    return QVariant();
  }

  int row = index.row();
  if (row < 0 || row >= configRows.count()) {
    qDebug () << " Bad Row " << row;
    return QVariant();
  }
  QVariant retval ;
  bool hasValue = configRows[row].kind == ConfigItem::Kind_Value;
  switch (role) {
  case Qt::DisplayRole:
  case int(Type_Key):
    retval = QVariant(configRows[row].key);
    break;
  case int (Type_Group):
    retval = QVariant(configRows[row].group);
    break;
  case int(Type_Value):
    retval = (hasValue? configRows[row].value : QString(""));
    break;
  case int(Type_HasValue):
    retval = QVariant (hasValue);
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
  int rc = configRows.count();
  return rc;
}


void
QmlConfigEdit::Load ()
{
  qDebug () << "QmlConfigEdit::Load" ;
  int nr = rowCount();
  beginResetModel ();
  if (nr > 0) {
    removeRows (0,nr-1);
    configRows.clear ();
  }
  QSettings & Zett = Settings();
  QString   name;
  QVariant  data;
  QStringList top = Zett.childKeys ();
  QStringList::iterator topit;

  QStringList groups = Zett.childGroups();
  QStringList::iterator  grit, subit;
  int level (0);
  for (grit = groups.begin(); grit != groups.end(); grit++) {
    QString group (*grit);
    if (exemptGroups.contains (group)) {
      qDebug () << "     Exempt";
      continue;
    }
    addRow (ConfigItem ("", group, QVariant(""), ConfigItem::Kind_Header, level));
    Zett.beginGroup (group);
    QStringList subs = Zett.childKeys ();
    for (subit = subs.begin(); subit != subs.end(); subit++) {
      name = *subit;
      data = Zett.value(name); 
      addRow (ConfigItem (group, name, data, ConfigItem::Kind_Value, level+1));
    }
    Zett.endGroup ();
  }
  beginInsertRows (QModelIndex(), nr, nr);
  endInsertRows ();
  endResetModel ();
}

void
QmlConfigEdit::addRow (const ConfigItem & item)
{
  configRows << item;
}

void
QmlConfigEdit::didInsertRows  ( const QModelIndex & parent, int start, int end )
{
  qDebug () << "QmlConfigEdit didInsertRows " << parent << start << end;
}

void
QmlConfigEdit::didResetModel ()
{
  qDebug () << "QmlConfigEdit::didResetModel";
}

void
QmlConfigEdit::didChangeData (const QModelIndex & ul, const QModelIndex & lr)
{
  qDebug () << "QmlConfigEdit::didChangeData " << ul << lr;
}

void
QmlConfigEdit::loadView ()
{
  Load ();
}

void
QmlConfigEdit::testContent ()
{
  qDebug () << "QmlConfigEdit::testContent";
  int nr = rowCount ();
  for (int r=0; r<nr; r++) {
    QModelIndex index = createIndex (r,0);
    qDebug () << " row " << r 
              << data (index, Type_Key)
              << data (index, Type_Value)
              << data (index, Type_HasValue);
  }
}

void
QmlConfigEdit::saveView ()
{
  Save ();
}

void
QmlConfigEdit::Save ()
{
  QMessageBox::warning (0,"Warning", "QmlConfigEdit::Save incomplete");
  //Zett.sync();
}

void
QmlConfigEdit::updateValue (const QString & group,
                                const QString & key,
                                const QVariant & value)
{
  QString realKey (QString ("%1/%2").arg(group).arg(key));
  if (Settings().contains (realKey)) {
    Settings().setValue (realKey, value);
    qDebug () << " updated Settings " << realKey << " to " << value;
    qDebug () << " is now " << Settings().value (realKey);
  } else {
    qDebug () << " Cannot update Settings " << realKey;
  }
  Load ();
}


} // namespace
