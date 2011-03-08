
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
  property real shrinkDelay: 500
  z: -1
  signal selected (int idx, string i, string t)
  signal reportOrientation (int orient)

  function nextStory () {
    storyList.incrementCurrentIndex()
  }
  function flipOrientation () {
    storyList.flipOrientation ()
  }
  function reportCurrent () {
    return storyList.currentIndex
  }
  function shrink () {
    shrinkWidth.running = true; 
    shrinkScale.running = true; 
  }
  function expand () {
    expandWidth.running = true;
    expandScale.running = true;
  }
  color: "transparent"
  anchors.top: parent.top
  anchors.left: parent.left
  anchors.topMargin: 0
  anchors.leftMargin: 0
  height: parent.height
  width: parent.width

  PropertyAnimation on width { 
    id: shrinkWidth
    running: false
    to: 0
    duration: shrinkDelay
  }
  PropertyAnimation on scale { 
    id: shrinkScale
    running: false
    to: 0
    duration: shrinkDelay
  }
  PropertyAnimation on width { 
    id: expandWidth
    running: false
    to: normalWidth
    duration: shrinkDelay
  }
  PropertyAnimation on scale { 
    id: expandScale
    running: false
    to: 1
    duration: shrinkDelay
  }
  Component {
    id: verticalDelegate
    Item {
      width: 300; height: 60
      Column { 
        anchors.leftMargin: 4
        Text { text: '<b>' + title + '</b>' } 
      }
      MouseArea {
        anchors.fill: parent
        onClicked: { selected (index, ident, title); storyList.currentIndex = index }
      }
    }
  }

  Component {
    id: horizontalDelegate
    Item {
      width: 300; height: 60
      Row { 
        anchors.topMargin: 4
        Text { text: '<b>' + title + '</b>' } 
      }
      MouseArea {
        anchors.fill: parent
        onClicked: { selected (index, ident, title); storyList.currentIndex = index }
      }
    }
  }

  ListView {
    id: storyList
    function changeOrientation (orient) {
      feedIF.report ("changeOrientation start")
      orientation = orient
      if (orient == ListView.Horizontal) {
        delegate = horizontalDelegate
      } else {
        delegate = verticalDelegate
      }
      reportOrientation (orientation)
      feedIF.report ("storyList orientation " + orientation)
      feedIF.report ("changeOrientation end")
    }
    function flipOrientation () {
      feedIF.report ("flipOrientation start " + orientation)
      if (orientation == ListView.Horizontal) {
        changeOrientation (ListView.Vertical) 
      } else {
        changeOrientation (ListView.Horizontal)
      }
      feedIF.report ("flipOrientation end")
    }
    model: feedIndexModel
    z: 2
    width: parent.width - 2*embedMargin
    height: parent.width - 2*embedMargin
    clip: false
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    delegate: verticalDelegate
    snapMode: ListView.NoSnap
    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
  }
}