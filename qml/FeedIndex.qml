
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
  property real itemWidth: normalWidth
  property real shrinkDelay: 250
  property real itemHeight: 32
  property real headHeight: itemHeight * 0.75
  property real headTextMargin: 6
  property string headColor: "yellow"
  property string itemColor: parent.color
  property string headerIntro: qsTr(" Stories from: ")
  property string headerText: qsTr("Feed Items")
  property alias currentIndex: storyIndex.currentIndex
  signal selected (int idx, string i, string t)
  signal holdit (int idx, string i, string t)
  signal quitit ()
  signal reportOrientation (int orient)

  function updateHeaderText () { headerText = storyIndex.model.feedTitle () }
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
    Rectangle {
      width: normalWidth; height: itemHeight
      color: itemColor 
      Column { 
        anchors.topMargin: 4
        Text {  
          width:normalWidth; wrapMode:Text.Wrap
          text: (seenit ? "_x_ " : "<b>new</b> " ) + title 
        } 
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {  storyIndex.currentIndex = index; selected (index, ident, title) }
        onPressAndHold: {storyIndex.currentIndex = index;  holdit (index, ident, title) }
      }
      Component.onCompleted: { updateHeaderText () }
    }
  }
  Component {
    id: titleHeader  
    Rectangle {
      width:  titleHeaderText.width + 2 * headTextMargin; height:  headHeight
      gradient: Gradient {
        GradientStop {
          position: 0.00
          color: "#7777ff"
        }
        GradientStop {
          position: 1.00
          color: "#bbbbff"
        }
      }
      Text { 
        id: titleHeaderText
        anchors.leftMargin: 10; 
        text: headerIntro + headerText 
      }
      MouseArea {
        anchors.fill: parent
        onPressAndHold: { quitit() }
      }
    }
  }

  ListView {
    id: storyIndex
    model: feedIndexModel
    z: 2
    width: parent.width - 2*embedMargin
    height: parent.height - 2*embedMargin
    clip: false
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    header: titleHeader
    delegate: verticalDelegate
    snapMode: ListView.NoSnap
    highlightFollowsCurrentItem: true
    highlight: Rectangle { color: "red"}
  }
}