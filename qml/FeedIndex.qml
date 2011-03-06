
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
  z: -1
  signal selected (string d, string m)
  function changeOrientation (theOrient) {
    storyList.orientation = theOrient
  }
  function nextStory () {
    storyList.incrementCurrentIndex()
  }
  color: "transparent"
  border.color: "red"
  anchors.top: parent.top
  anchors.left: parent.left
  anchors.topMargin: embedMargin
  anchors.leftMargin: embedMargin
  height: parent.height - 2*embedMargin
  width: parent.width - 2*embedMargin 

  Component {
    id: contactDelegate
    Item {
      width: 300; height: 60
      Column {
        Text { text: '<i>' + title + '</i>' }
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          selected (ident, title)
        }
      }
    }

  }

  ListView {
    id: storyList
    z: 2
    width: parent.width - 2*embedMargin
    height: parent.width - 2*embedMargin
    clip: true
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top:  parent.top
    anchors.topMargin: embedMargin
    orientation: ListView.Vertical
    model: displayModel
    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    delegate: contactDelegate
/*
    onDidselect: {
      parent.selected (d,m)
    }
*/
  }
}