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
  :key(""),value(""),kind(Kind_None)
{
}

QmlConfigEdit::ConfigItem::ConfigItem (const QString & ke, 
                                       const QVariant & val, 
                                       QmlConfigEdit::ConfigItem::Kind ki)
  :key (ke),
   value (val),
   kind (ki)
{
}

QmlConfigEdit::ConfigItem::ConfigItem (const QmlConfigEdit::ConfigItem & other)
  :key (other.key),
   value (other.value),
   kind (other.kind)
{
}

QmlConfigEdit::QmlConfigEdit (QObject * parent)
  :QAbstractListModel (parent)
{
  exemptGroups << "sizes" ;
  QHash<int, QByteArray> roles;
  roles[Type_Key] = "confKey";
  roles[Type_Value] = "confValue";
  roles[Type_ReadOnly] = "confReadOnly";
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
    qDebug () << " Bad Index";
    return QVariant();
  }

  int row = index.row();
  if (row < 0 || row >= configRows.count()) {
    qDebug () << " Bad Row " << row;
    return QVariant();
  }
  QVariant retval ;
  switch (role) {
  case Qt::DisplayRole:
  case int(Type_Key):
    retval = QVariant(configRows[row].key);
    break;
  case int(Type_Value):
    retval = configRows[row].value;
    break;
  case int(Type_ReadOnly):
    retval = QVariant (configRows[row].kind != ConfigItem::Kind_Value);
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
  qDebug () << " QmlConfigEdit::rowCount " << parent << rc;
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

  name = QString ("program");
  data = Zett.value (name).toString(); 
  addRow (ConfigItem (name, data, ConfigItem::Kind_Header));
  QStringList groups = Zett.childGroups();
  QStringList::iterator  grit, subit;
  for (grit = groups.begin(); grit != groups.end(); grit++) {
    if (exemptGroups.contains (*grit)) {
      qDebug () << "     Exempt";
      continue;
    }
    addRow (ConfigItem (*grit, QVariant("group"), ConfigItem::Kind_Header));
    Zett.beginGroup (*grit);
    QStringList subs = Zett.childKeys ();
    for (subit = subs.begin(); subit != subs.end(); subit++) {
      name = *subit;
      data = Zett.value(name); 
      addRow (ConfigItem (name, data, ConfigItem::Kind_Value));
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
  qDebug () << " addRow " << item.key << item.value << item.kind;
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
              << data (index, Type_ReadOnly);
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


} // namespace
