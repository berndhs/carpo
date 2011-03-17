
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
  property real normalWidth:300
  property real rollDelay: 250
  property real itemHeight: 32
  property real embedMargin: 2
  property real initialYScale: 0
  signal selected (int idx, string name, int count)

  function hide () {
    rollupScale.yScale = 0
    console.log ("shink running ")
  }
  function show () {
    rollupScale.yScale = 1
  }
  color: "transparent"
  anchors.topMargin: 0
  anchors.leftMargin: 0
  height: parent.height
  width: parent.width  

  transform: Scale {
    id: rollupScale
    xScale: 1
    yScale: initialYScale
    Behavior  on yScale {
      NumberAnimation { duration: rollDelay }
    }
  }
  Component {
    id: verticalDelegate
    Item {
      width: childrenRect.width; height: itemHeight
      Column { 
        id: nameColumn
        anchors { left: parent.left; leftMargin: 6 }
        Rectangle {
          id: nameItemRect
          width: childrenRect.width; height: itemHeight
          color: "transparent"
          Text {  wrapMode:Text.Wrap; text:  name } 
          MouseArea {
            anchors.fill: parent
            onClicked: { selected (index, name, number); topicList.currentIndex = index }
          }
        }
      }
      Column { 
        id: countColumn
        anchors { left: nameColumn.right; leftMargin: 2 }
        Rectangle {
          id: countItemRect
          width: childrenRect.width; height: itemHeight
          color: "transparent"
          Text {   wrapMode:Text.Wrap; text: " : " + number } 
          MouseArea {
            anchors.fill: parent
            onClicked: { selected (index, name, number); topicList.currentIndex = index }
          }
        }
      }
    }
  }

  ListView {
    id: topicList
    z: 2
    width: parent.width
    height: parent.height
    clip: false
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    delegate: verticalDelegate
    snapMode: ListView.NoSnap
    model: topicModel
    highlight: Rectangle { width: topicList.width; color: "oldlace"; radius: 5 }
  }
}