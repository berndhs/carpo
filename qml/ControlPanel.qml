
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
  signal selectQuit ()
  signal selectHelp ()
  signal newFeed ()
  signal editSettings ()
  signal importDRSS ()
  signal importOPML ()
  property string choiceButtonColor: "#f4a460"
  property string choiceButtonFade: "#fbdfc7"
  property string detailButtonColor: "#f4a460"
  property string detailButtonFade: "#fbdfc7"
  property string maintainButtonColor: "#f4a460"
  property string maintainButtonFade: "#fbdfc7"
  property real menuItemHeight: 32
  property real embedMargin : 0
  property string subMenuButtonColor: "#f4a460"
  property string subMenuButtonFade: "#fbdfc7"

  height: menuItemHeight
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
      controlSelect (mouseX, mouseY) 
    }
  }

  ChoiceButton {   
    id: offButton
    width: (parent.width - buttonRow.width) * 0.4
    anchors { left: buttonRow.right}
    gradient: Gradient {
      GradientStop { position: 0.00; color: "#c6e6e6" }
      GradientStop { position: 1.00; color: "#f0f0f0" }
    }
    height: parent.height
    radius: height/3
    labelText: ""
    Image {
      anchors.centerIn: parent
      anchors.fill: parent
      fillMode: Image.PreserveAspectFit
      source: "qrc:///images/quit.png"
    }
    onClicked: selectQuit()
  }
  ChoiceButton {   
    id: helpButton
    width: (parent.width - buttonRow.width) * 0.4
    anchors { right: buttonRow.left }
    gradient: Gradient {
      GradientStop { position: 0.00; color: "#c6e6e6" }
      GradientStop { position: 1.00; color: "#f0f0f0" }
    }
    height: parent.height
    radius: height/3
    labelText: ""
    Image {
      anchors.centerIn: parent
      anchors.fill: parent
      fillMode: Image.PreserveAspectFit
      source: "qrc:///images/help.png"
    }
    onClicked: selectHelp()
  }

  Gradient {
    id: subMenuButtonGradient
    GradientStop { position: 0.0; color: subMenuButtonColor }
    GradientStop { position: 1.0; color: subMenuButtonFade }
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
      z: controlPanel.z + 1
      anchors { left: parent.left }
      radius: height/2
      gradient: Gradient {
        GradientStop { position: 0.00; color: choiceButtonColor }
        GradientStop { position: 1.00; color: choiceButtonFade }
      }
      MouseArea {
        anchors.fill: parent
        onClicked: toggleViewSelect ()
      }
      Text { text: qsTr("Feeds / Stories"); anchors.centerIn: parent }
    }
    Rectangle {
      id: detailButton
      width: parent.buttonWidth
      height: parent.height
      z: controlPanel.z + 1
      gradient: Gradient {
        GradientStop { position: 0.0; color: detailButtonColor }
        GradientStop { position: 1.0; color: detailButtonFade }
      }
      anchors { left: indexButton.right }
      radius: height/2
      MouseArea {
        anchors.fill: parent
        onClicked: indexMenu.toggle ()
      }
      Text { text: qsTr("Views"); anchors.centerIn: parent }
    }
    Rectangle {
      id: maintainButton
      width: parent.buttonWidth
      height: parent.height
      z: controlPanel.z + 1
      gradient: Gradient {
        GradientStop { position: 0.00; color: maintainButtonColor }
        GradientStop { position: 1.00; color: maintainButtonFade }
      }
      anchors { left: detailButton.right }
      radius: height/2
      Image {
        anchors.centerIn: parent
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "qrc:///images/settings.png"
      }
      MouseArea {
        anchors.fill: parent
        onClicked: maintainMenu.toggle ()
      }
    }

    DropMenu {
      id: indexMenu
      anchors { top: buttonRow.bottom; left: detailButton.left ; leftMargin: embedMargin}
      width: indexButton.width
      color: "transparent"
      z: controlPanel.z + 2
      MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onExited: { indexMenu.hide () }
      }
      ChoiceButton {
        id: showTopicsButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        radius: height * 0.5
        labelText: qsTr("Topics - On")
        anchors { top: indexMenu.top; horizontalCenter: indexMenu.horizontalCenter }
       onClicked: { controlPanel.showTopics(); indexMenu.hide() }
      }
      ChoiceButton {
        id: hideTopicsButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        radius: height * 0.5
        anchors { top: showTopicsButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
        labelText: qsTr("Topics - Off")
       onClicked: { controlPanel.hideTopics(); indexMenu.hide () }
      }
      ChoiceButton {
        id: showNewButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        radius: height * 0.5
        labelText: qsTr("New Stories - On")
        anchors { top: hideTopicsButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
        onClicked: { controlPanel.showRecent(); indexMenu.hide() }
      }
      ChoiceButton {
        id: hideNewButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        radius: height * 0.5
        anchors { top: showNewButton.bottom; horizontalCenter: indexMenu.horizontalCenter }
        labelText: qsTr("New Stories - Off")
        onClicked: { controlPanel.hideRecent(); indexMenu.hide () }
      }
    }

    DropMenu {
      id: maintainMenu
      anchors { top: buttonRow.bottom; left: maintainButton.left ; leftMargin: embedMargin}
      width: indexButton.width
      height: floatHeight
      color: "transparent"
      z: controlPanel.z + 2
      MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onExited: { maintainMenu.hide () }
      }
      ChoiceButton {
        id: showFeedEditButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        labelText: qsTr("New Feed")
        radius: height * 0.5
        z: 9
        anchors { top: maintainMenu.top; horizontalCenter: maintainMenu.horizontalCenter }
        onClicked: { controlPanel.newFeed(); maintainMenu.hide() }
      }
      ChoiceButton {
        id: importDrssButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        labelText: qsTr("Import Carpo/DRSS")
        radius: height * 0.5
        z: 9
        anchors { 
          top: showFeedEditButton.bottom; 
          horizontalCenter: maintainMenu.horizontalCenter 
        }
        onClicked: { controlPanel.importDRSS(); maintainMenu.hide() }
      }
      ChoiceButton {
        id: showSettingsButton
        width: parent.width
        height: menuItemHeight
        gradient: subMenuButtonGradient
        labelText: qsTr("Settings")
        radius: height * 0.5
        z: 9
        anchors { 
          top: importDrssButton.bottom; 
          horizontalCenter: maintainMenu.horizontalCenter 
        }
        onClicked: { controlPanel.maintainSelect(); maintainMenu.hide() }
      }
    }
  }
}
