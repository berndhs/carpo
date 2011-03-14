

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
  property real normalWidth: parent.width
  property real shrinkDelay: 250
  property real itemHeight: 32
  function hide () {
    visible = false
  }
  function show () {
    visible = true
  }
  id: configListRect
  visible: true
  color: "transparent"
  anchors.top: parent.top
  anchors.left: parent.left
  anchors.topMargin: 0
  anchors.leftMargin: 0
  width: parent.width
  height: parent.height
  Component {
    id: verticalConfigDelegate
    Rectangle {
      width: parent.width; height: itemHeight
      anchors.topMargin: 2
      color: "cyan"
      Column { 
        anchors.topMargin: 4
        
        Text {  width:300; wrapMode:Text.Wrap
                text:  confKey + " -> " + confValue +" | " + confReadOnly} 
        
      }
      MouseArea {
        anchors.fill: parent
        onClicked: { console.log ("config item clicked " + index + " + "+ key) }
      }
    }
  }
  ListView {
    id: configListView
    visible: true
    delegate: verticalConfigDelegate
    clip: false
    width: parent.width - 4
    height: parent.height -4
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top: parent.top
    anchors.topMargin: 6
    orientation: ListView.Vertical
    snapMode: ListView.NoSnap
    model: configModel
    highlight: Rectangle { color: "oldlace"; radius: 5 }
  }
}