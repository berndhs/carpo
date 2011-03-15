#include "property-store.h"


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

#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QObject>
#include <QMessageBox>
#include <QDebug>

namespace deliberate
{

PropertyStore::PropertyStore (QObject *parent, QSettings *settings)
  :QObject(parent),
   theSettings (settings)
{
}

void
PropertyStore::setSettings (QSettings * settings)
{
  theSettings = settings;
}

QSettings *
PropertyStore::settings ()
{
  return theSettings;
}

bool
PropertyStore::contains (const QString & object, const QString & property)
{
  QString realKey (object + "/" + property);
  return properties.contains (realKey);
}

QVariant
PropertyStore::value (const QString & object, const QString & property)
{
  QString realKey (object + "/" + property);
  if (properties.contains (realKey)) {
    return properties[realKey].value();
  } else {
    return QVariant ();
  }
}

void
PropertyStore::setValue (const QString & object,
                         const QString & property,
                         const QVariant & value)
{
  QString realKey (object + "/" + property);
  properties[realKey] = Property (object, property, value);
}

PropertyList
PropertyStore::objectProperties (const QString & object)
{
  QString partialKey (object + "/");
  PropertyMap::iterator pit;
  PropertyList list;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    if (pit.key().startsWith (partialKey)) {
      list << *pit;
    }
  }
  return list;
}

void
PropertyStore::Init (const QString & filename)
{
  qDebug () << "PropertyStore::Init " << filename;
  QFile file (filename);
  bool ok = file.open (QFile::ReadOnly);
  if (!ok) {
    qDebug () << " Cannot open property file " << filename;
    return;
  }
  properties.clear ();
  while (!file.atEnd()) {
    QByteArray lineData = file.readLine();
    QStringList words (QString (lineData).split(QRegExp("\\s+")));
    if (words.count() > 1) {
      QString realKey (words.at(0) + "/" + words.at(1));
      properties[realKey] = Property (words.at(0), words.at(1), QVariant());
    }
  }
  file.close();
}

void
PropertyStore::ReadFromObjects (QObject * rootObject)
{
  if (rootObject == 0) {
    return;
  }
  PropertyMap::iterator pit;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    Property & prop (*pit);
    QObject * obj = rootObject->findChild<QObject*> (prop.objectName());
    if (obj) {
      prop.setValue (obj->property (prop.name().toLatin1().data()));
    }
  }
}

void
PropertyStore::ReadFromSettings ()
{
  if (theSettings == 0) {
    return;
  }
  PropertyMap::iterator pit;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    Property & prop (*pit);
    if (theSettings->contains (pit.key())) {
      prop.setValue (theSettings->value (pit.key()));
    }
  }
}

void
PropertyStore::SyncToObjects (QObject * rootObject)
{
  PropertyMap::iterator pit;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    Property & prop (*pit);
    QObject * obj = rootObject->findChild<QObject*> (prop.objectName());
    if (obj) {
      obj->setProperty (prop.name().toLatin1().data(),
                        prop.value());
    }
  }
}

void
PropertyStore::SyncToSettings ()
{
  if (theSettings == 0) {
    return;
  }
  PropertyMap::iterator pit;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    Property & prop (*pit);
    theSettings->setValue (pit.key(), prop.value());
  }
}

void
PropertyStore::FillSettings (QObject * rootObject)
{
  if (theSettings == 0) {
    return;
  }
  PropertyMap::iterator pit;
  for (pit=properties.begin(); pit != properties.end(); pit++) {
    Property & prop (*pit);
    QObject * obj = rootObject->findChild<QObject*> (prop.objectName());
    if (obj) {
      QVariant defaultVal = obj->property (prop.name().toLatin1().data());
      QVariant savedVal = theSettings->value (pit.key(), defaultVal);
      theSettings->setValue (pit.key(), savedVal);
      prop.setValue (savedVal);
    }
  }
}

} // namespace
