
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
  id: topicListBox
  objectName: "TopicListBox"
  property real normalWidth:300
  property real rollDelay: 150
  property real itemHeight: 32
  property real itemWidth: width
  property real embedMargin: 2
  property real initialYScale: 0
  property string highlightColor: "oldlace"
  property string topObject: objectName
  property alias heightScale: rollupScale.yScale
  signal selected (int idx, string name, int count)
  signal quitit ()

  function hide () {
    rollupScale.yScale = 0
  }
  function show () {
    rollupScale.yScale = 1
    console.log ("Show Topic list at z = " + z)
  }
  function swiped () { hide () }
  function setTracking (doTrack) { topicList.setTracking (doTrack) }
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
          width: itemWidth; height: itemHeight
          color: "transparent"
          Text {  wrapMode:Text.Wrap; text:  name + " : " + number} 
          MouseArea {
            anchors.fill: parent
            onClicked: { selected (index, name, number); topicList.currentIndex = index }
            onPressed:   gestureIF.pressed (topObject, mouse.x, mouse.y)
            onExited:    gestureIF.exited (topObject, mouseX, mouseY)
            onReleased:  gestureIF.released (topObject, mouseX, mouseY)
            onPositionChanged: {
              if (topicList.isTracking) gestureIF.moved (topObject, mouse.x, mouse.y)
            }
            onPressAndHold: { quitit () }
          }
        }
      }
    }
  }

  ListView {
    id: topicList
    property bool isTracking: false
    function setTracking (doTrack) { isTracking = doTrack }
    z: 2
    width: parent.width
    height: parent.height
    clip: true
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    delegate: verticalDelegate
    snapMode: ListView.NoSnap
    model: topicModel
    highlight: Rectangle { width: topicList.width; color: highlightColor; radius: 5 }
  }
}