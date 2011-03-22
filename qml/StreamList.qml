
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
  property real normalWidth:500
  property real normalHeight:500
  property real leftMargin: 0
  property real shrinkDelay: 250
  property real itemHeight: 32
  property real singleHeight: 0.75
  property real doubleHeight: 1.0
  property string feedButtonColor: "#eeeeff"
  property string storyButtonColor: "#eeffee"
  property string feedLineColor: "#ffffdd"
  property string storyLineColor: "#ffddff"
  signal selectFeed (string feedId)
  signal selectStory (string feedId, string title, string hash)
  signal quitit ()

  function hide () {
    shrinkWidth.running = true; 
    shrinkScale.running = true; 
  }
  function show () {
    expandWidth.running = true;
    expandScale.running = true;
  }
  function setNewestRow (theRow) {
    streamList.currentIndex = theRow
  }
  color: "transparent"
  anchors.leftMargin: leftMargin
  width: normalWidth
  height: normalHeight

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
      width: normalWidth; height: 2*singleHeight*itemHeight
      Column { 
        id: itemColumn
        anchors { left: parent.left }
        Rectangle {
          id: itemRect
          width: normalWidth; height: 2*singleHeight*itemHeight
          color: feedLineColor
          radius: 5
          Rectangle {
            id: itemFeedRect
            width: normalWidth; height: singleHeight*itemHeight 
            color: feedLineColor
            Text { text: feedTitle }
            MouseArea {
              anchors.fill: parent
              onClicked: {  
                selectStory (feedId, storyTitle, storyHash); 
                streamList.currentIndex = index 
              }
              onPressAndHold : { quitit () }
            }  
          }
          Rectangle {
            id: itemTitleRect
            width: normalWidth; height: singleHeight*itemHeight 
            x: itemFeedRect.x
            y: itemFeedRect.y + itemFeedRect.height 
            color: storyLineColor
            Text { text: storyTitle } 
            MouseArea {
              anchors.fill: parent
              onClicked: {  
                selectStory (feedId, storyTitle, storyHash); 
                streamList.currentIndex = index 
              }
              onPressAndHold : { quitit () }
            }
          }
        }
      }
    }
  }

  ListView {
    id: streamList
    z: 2
    width: parent.width - 2*embedMargin
    height: parent.height - 2*embedMargin
    clip: false
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    delegate: verticalDelegate
    snapMode: ListView.NoSnap
    model: streamListModel
    highlight: Rectangle { color: "oldlace"; radius: 5 }
  }
}