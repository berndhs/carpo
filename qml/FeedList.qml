
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


import QtQuick 1.1

Rectangle {
  property real normalWidth: parent.width
  property real shrinkDelay: 250
  property real itemHeight: 32
  property alias currentIndex: feedList.currentIndex
  signal selected (int idx, string i, string t)
  signal selectMoveUp (string ident)
  signal selectMoveDown (string ident)
  signal selectedLong (int idx, string i, string t)
  signal reportOrientation (int orient)

  function nextStory () {
    feedList.incrementCurrentIndex()
  }
  function flipOrientation () {
    feedList.flipOrientation ()
  }
  function currentOrientation () { return feedList.orientation }
  function reportCurrent () {
    return feedList.currentIndex
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
      width: normalWidth; height: itemHeight

      Column {
        id: moveDownColumn
        anchors {left: parent.left }
        Rectangle {
          id: moveDownRect
          width: childrenRect.width; height: itemHeight
          color: "transparent"
          Text { width: 48; text: "Dwn " }
          MouseArea {
            anchors.fill: parent
            onClicked: { selectMoveDown (ident) }
          }
        }
      }
      Column {
        id: moveUpColumn
        anchors {left: moveDownColumn.right }
        Rectangle {
          id: moveUpRect
          width: childrenRect.width; height: itemHeight
          color: "transparent"
          Text { width: 32; text: "Up " }
          MouseArea {
            anchors.fill: parent
            onClicked: {  selectMoveUp (ident) }
          }
        }
      }
      Column { 
        id: titleColumn
        anchors { left: moveUpColumn.right; leftMargin: 6 }
        Rectangle {
          id: titleItemRect
          width: normalWidth; height: itemHeight
          color: "transparent"
          Text {  width:normalWidth; wrapMode:Text.Wrap; text:  title } 
          MouseArea {
            anchors.fill: parent
            onClicked: { selected (index, ident, title); feedList.currentIndex = index }
            onPressAndHold: { 
              selectedLong (index, ident, title)
              feedList.currentIndex = index
            }
          }
        }
      }
    }
  }

  ListView {
    id: feedList
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
    model: feedListModel
    highlight: Rectangle { color: "oldlace"}
  }
}