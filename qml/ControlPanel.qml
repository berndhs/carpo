
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

 import QtQuick 1.0


Rectangle {

  signal controlSelect (real whereX, real whereY)
  signal toggleViewSelect ()
  signal moreSelect ()
  signal maintainSelect ()
  property string choiceButtonColor: "sandybrown"
  property string detailButtonColor: "sandybrown"
  property string maintainButtonColor: "sandybrown"

  id: controlPanel
  height: 20
  z: 1
  property real extraMargin: 20
  width: parent.width - 2*embedMargin - 2*extraMargin
  color: "transparent"
  anchors.left: parent.left
  anchors.leftMargin: embedMargin + extraMargin
  anchors.rightMargin: embedMargin + extraMargin
  MouseArea {
    anchors.fill: parent
    onClicked: { 
      console.log("Click Control Box " + mouseX + " " + mouseY)
      controlSelect (mouseX, mouseY) 
    }
  }
  Rectangle {
    id: buttonRow
    width: childrenRect.width
    height: parent.height
    property real buttonWidth: parent.width/4
    anchors { horizontalCenter: parent.horizontalCenter ; top: parent.top }
    Rectangle {
      id: turner
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: choiceButtonColor
      anchors { left: parent.left; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          toggleViewSelect () 
        }
      }
      Text { text: "List / Index"; anchors.centerIn: parent }
    }
    Rectangle {
      id: detailer
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: detailButtonColor
      anchors { left: turner.right; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          moreSelect () 
        }
      }
      Text { text: "Detail On/Off"; anchors.centerIn: parent }
    }
    Rectangle {
      id: maintainer
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: maintainButtonColor
      anchors { left: detailer.right; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          maintainSelect () 
        }
      }
      Text { text: "Maintenance"; anchors.centerIn: parent }
    }
  }
}