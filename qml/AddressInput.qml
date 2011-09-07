
import QtQuick 1.1


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


Rectangle {
  id: bigBox
  signal urlEntered(string url)
  property alias urlString: urlText.text
  property alias labelText: urlLabelText.text
  property alias autoScroll: urlText.autoScroll
  property alias selectByMouse: urlText.selectByMouse

  width: parent.width - parent.urlMargin
  anchors {
    horizontalCenter: parent.horizontalCenter
    top: parent.top
    topMargin: 2
  }
  color: "wheat"
  height: 48
  radius: parent.radius - 1
  Rectangle { 
    height: 32
    id: urlLabel
    width: childrenRect.width
    color:parent.color
    Text { id: urlLabelText; text: "Address:"; anchors.verticalCenter: parent.verticalCenter }
    anchors.left: parent.left 
    anchors.verticalCenter: parent.verticalCenter
  }
  Rectangle {
    id: urlTextBox
    height: urlLabel.height
    width: parent.width - urlLabel.width - anchors.leftMargin -2
    anchors.left: urlLabel.right
    anchors.leftMargin: 2
    anchors.verticalCenter: parent.verticalCenter
    color: "white"
    radius: parent.radius - 1
    TextInput {
      id: urlText
      horizontalAlignment: TextEdit.AlignLeft
      autoScroll: true
      selectByMouse: true
      onAccepted: {
        bigBox.urlEntered (urlText.text)
      }
      anchors {
        left: parent.left; right: parent.right; leftMargin: 2; rightMargin:2 
        verticalCenter: parent.verticalCenter
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          urlText.forceActiveFocus()()
        }
        onPressAndHold: {
          copyPasteAddress.visible = !copyPasteAddress.visible
        }
      }
    }
    CopyPasteMenu {
      id: copyPasteAddress
      z: urlTextBox.z + 1
      visible: false
      height: urlLabel.height
      width: urlTextBox.width
      anchors { 
        bottom: urlTextBox.top
        horizontalCenter: urlTextBox.horizontalCenter 
      }
      onWantCopy: {
        urlText.copy ()
      }
      onWantSelect: {
        urlText.selectAll ()
      }
      onWantPaste: {
        urlText.paste ()
      }
    }
  }
}
