#ifndef DELIBERATE_PROPERTY_STORE_H
#define DELIBERATE_PROPERTY_STORE_H


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



#include <QString>
#include <QVariant>
#include <QMap>
#include <QList>
#include <QSettings>

namespace deliberate
{

class Property
{
public:
  Property () {}
  Property (const QString & object, 
            const QString & name, 
            const QVariant & value)
    :theObject (object),
     theName (name),
     theValue (value)
    {}
  Property (const Property & other)
    :theObject (other.theObject),
     theName (other.theName),
     theValue (other.theValue)
    {}

  QString objectName () const { return theObject; }
  QString name () const       { return theName; }
  QVariant value () const     { return theValue; }

  void setObjectName (const QString & objName) { theObject = objName; }
  void setName (const QString & name)          { theName = name; }
  void setValue (const QVariant & value)       { theValue = value; }

private:

  QString     theObject;
  QString     theName;
  QVariant    theValue;
};

typedef QList <Property>  PropertyList;


class PropertyStore : public QObject
{
Q_OBJECT

private:

  typedef QMap <QString, Property>  PropertyMap;

public:

  PropertyStore (QObject *parent=0, QSettings * settings=0);

  void setSettings (QSettings * settings);
  QSettings * settings ();
  void Init (const QString & filename);

  void ReadFromObjects (QObject * rootObject);
  void ReadFromSettings ();
  void SyncToObjects (QObject * rootObject);
  void SyncToSettings ();
  void FillSettings (QObject * rootObject);

  bool     contains (const QString & object, const QString & property);
  QVariant value (const QString & object, const QString & property);
  void     setValue (const QString & object,
                     const QString & property,
                     const QVariant & value);
  PropertyList  objectProperties (const QString & object);

private:

  QSettings           * theSettings;
  PropertyMap           properties;

};

} // namespace

#endif
