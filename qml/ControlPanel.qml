
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
  signal flipSelect (real whereX, real whereY)
  signal toggleViewSelect (real whereX, real whereY)

  id: controlPanel
  height: 20
  z: 1
  property real extraMargin: 20
  width: parent.width - 2*embedMargin - 2*extraMargin
  color: "transparent"
  border.color: "black"
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
    id: flipper
    width: parent.width/2
    height: parent.height
    z: 2
    anchors.left: parent.left
    color: "cyan"
    MouseArea {
      anchors.fill: flipper
      onClicked: { 
        console.log("Click Flipper Box " + mouseX + " " + mouseY)
        flipSelect (mouseX, mouseY) 
      }
    }
    Text { text: "Flip Horizontal/Vertical" }
  }
  Rectangle {
    id: turner
    width: parent.width/2
    height: parent.height
    z: 2
    anchors.left: flipper.right
    color: "red"
    MouseArea {
      anchors.fill: turner
      onClicked: { 
        console.log("Click Turner Box " + mouseX + " " + mouseY)
        toggleViewSelect (mouseX, mouseY) 
      }
    }
    Text { text: "<b>Toggle</b> Feed-List / Feed-Index view " }
  }
}