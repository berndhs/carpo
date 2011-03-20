
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
  id: controlPanel
  signal controlSelect (real whereX, real whereY)
  signal toggleViewSelect ()
  signal moreSelect ()
  signal maintainSelect ()
  signal showTopics ()
  signal hideTopics ()
  signal showRecent ()
  signal hideRecent ()
  property string choiceButtonColor: "sandybrown"
  property string detailButtonColor: "sandybrown"
  property string maintainButtonColor: "sandybrown"
  property real menuItemHeight: 32
  property real embedMargin : 0
  property string indexMenuButtonColor: "cyan"

  height: 20
  z: 1
  property real extraMargin: 20
  width: parent.width - 2*embedMargin - 2*extraMargin
  color: "transparent"
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
    id: buttonRow
    width: childrenRect.width
    height: parent.height
    property real buttonWidth: parent.width/4
    anchors { horizontalCenter: parent.horizontalCenter ; top: parent.top }
    Rectangle {
      id: indexButton
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: choiceButtonColor
      anchors { left: parent.left; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          toggleViewSelect () 
        }
        onPressAndHold: { indexMenu.show () }
      }
      Text { text: "List / Index"; anchors.centerIn: parent }
    }
    Rectangle {
      id: detailButton
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: detailButtonColor
      anchors { left: indexButton.right; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          moreSelect () 
        }
      }
      Text { text: "Detail On/Off"; anchors.centerIn: parent }
    }
    Rectangle {
      id: maintainButton
      width: parent.buttonWidth
      height: parent.height
      z: 2
      color: maintainButtonColor
      anchors { left: detailButton.right; leftMargin: 2; rightMargin: 2 }
      MouseArea {
        anchors.fill: parent
        onClicked: { 
          maintainSelect () 
        }
      }
      Text { text: "Maintenance"; anchors.centerIn: parent }
    }
  }
  DropMenu {
    id: indexMenu
    anchors { top: buttonRow.bottom; left: buttonRow.left ; leftMargin: embedMargin}
    width: indexButton.width
    height: floatHeight
    color: "transparent"
    MouseArea {
      anchors.fill: parent
      hoverEnabled: true
      onExited: { indexMenu.hide () }
    }
    ChoiceButton {
      id: closeIndexButton
      width: parent.width
      height: menuItemHeight
      color: indexMenuButtonColor
      labelText: "Close Menu"
      anchors { top: indexMenu.top; horizontalCenter: indexMenu.horizontalCenter }
      onClicked: { indexMenu.hide () }
    }
    ChoiceButton {
      id: showTopicsButton
      width: parent.width
      height: menuItemHeight
      color: indexMenuButtonColor
      labelText: "Show Topics"
      anchors { top: closeIndexButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
      onClicked: { controlPanel.showTopics(); indexMenu.hide() }
    }
    ChoiceButton {
      id: hideTopicsButton
      width: parent.width
      height: menuItemHeight
      color: indexMenuButtonColor
      anchors { top: showTopicsButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
      labelText: "Hide Topics"
      onClicked: { controlPanel.hideTopics(); indexMenu.hide () }
    }
    ChoiceButton {
      id: showNewButton
      width: parent.width
      height: menuItemHeight
      color: indexMenuButtonColor
      labelText: "Show New Stories"
      anchors { top: hideTopicsButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
      onClicked: { controlPanel.showRecent(); indexMenu.hide() }
    }
    ChoiceButton {
      id: hideNewButton
      width: parent.width
      height: menuItemHeight
      color: indexMenuButtonColor
      anchors { top: showNewButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
      labelText: "Hide New Stories"
      onClicked: { controlPanel.hideRecent(); indexMenu.hide () }
    }
  }
}