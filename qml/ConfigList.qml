

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
  property real widthRatio: 0.25
  property real embedMargin: 6
  property real keyFieldWidth: normalWidth*widthRatio - embedMargin
  property real valueFieldWidth: normalWidth*(1 - widthRatio) - embedMargin
  property real shrinkDelay: 250
  property real itemHeight: 32
  property bool isShown: true
  property string valueBackgroundColor: "#c7f0f0"
  property string keyBackgroundColor: "#f0f0f0"
  property real topMargin: 0
  property real leftMargin: 10
  property real rightMargin: 10
  signal updateConfigItem (string theGroup, string theKey, string newValue)
  signal restartConfig ()
  signal doneConfig ()
  function hide () {
    console.log ("ConfList hide")
    shrinkScale.running = true
    isShown = false
  }
  function show () {
    console.log ("ConfList show")
    expandScale.running = true
    isShown = true
  }
  id: configListRect
  color: "transparent"
  anchors.top: parent.top
  anchors.left: parent.left
  anchors.topMargin: 0
  anchors.leftMargin: leftMargin
  width: parent.width - leftMargin
  height: parent.height
  PropertyAnimation on scale { 
    id: shrinkScale
    running: false
    to: 0
    duration: shrinkDelay
  }
  PropertyAnimation on scale { 
    id: expandScale
    running: false
    to: 1
    duration: shrinkDelay
  }
  Rectangle {
    id: buttonRow
    height: childrenRect.height
    width: childrenRect.width
    anchors { horizontalCenter: parent.horizontalCenter }
    color: "transparent"
    ChoiceButton {
      id: saveConfigButton
      height: itemHeight * 1.2
      labelText: "Restart"
      onClicked: { console.log ("Restart config clicked "); restartConfig() }
    }
    ChoiceButton {
      id: doneConfigButton
      anchors { left: saveConfigButton.right }
      height: itemHeight * 1.2
      labelText: "Done"
      onClicked: { console.log ("Done config clicked "); doneConfig () }
    }
  }
  Component {
    id: verticalConfigDelegate
    Rectangle {
      width: parent.width
      height: itemHeight; 
      color: "transparent"
      anchors.topMargin: 2
      Column { 
        id: keyColumn
        anchors.topMargin: 4
        height: itemHeight
        width: keyFieldWidth   
        Rectangle {
          id: keyColumnRect
          anchors { topMargin: 2 }
          width: parent.width
          height: parent.height - 2
          color: keyBackgroundColor  
          z: parent.z + 1
          Text {  
            anchors.left: parent.left
            x: parent.x + 2
            width:parent.width; wrapMode:Text.Wrap
            text:  (confHasValue ? "....." + confKey : "<b>" + confKey + "</b" )
          }
        } 
      }
      Column {
        id: valueColumn
        width: valueFieldWidth
        height: itemHeight
        anchors.left: keyColumn.right
        Rectangle {
          id: valueColumnRect
          anchors.topMargin: 2
          anchors.leftMargin: confLevel * 10
          width: parent.width
          height: parent.height -2
          color: valueBackgroundColor
          z: parent.z + 1
          TextInput {
            id: valueField
            width: parent.width - 2
            enabled: confHasValue
            autoScroll: true
            text:  confValue
            selectByMouse: true
          }
        }
        Keys.onEnterPressed: { 
          updateConfigItem (confGroup, confKey, valueField.text)
        }
        Keys.onReturnPressed: { 
          updateConfigItem (confGroup, confKey, valueField.text)
        }
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log ("clicked item " + index + " key " + confKey)
        }
      }
    }
  }
  ListView {
    id: configListView
    visible: true
    delegate: verticalConfigDelegate
    clip: true
    width: parent.width - 4
    height: parent.height -itemHeight - buttonRow.height
    currentIndex: -1
    contentWidth: childrenRect.width; contentHeight: childrenRect.height
    anchors.top: buttonRow.bottom
    anchors.topMargin: 6
    orientation: ListView.Vertical
    model: configModel
    snapMode: ListView.SnapToItem
    highlight: Rectangle { color: "#ffbbbb"}
  }
}