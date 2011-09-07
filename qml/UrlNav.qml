

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
  id: urlNavRect
  height: 36
  width: 300
  property real buttonHeight: 36

  signal openUrl (string url)
  signal copyUrl (string url)
  signal openUrlExternal (string url)
  signal mailUrl (string url)

  function open (theUrl) {
    console.log ("open URL Nav " + theUrl)
    urlText.text = theUrl
    visible = true
  }
  function close () {
     visible = false
  }

  function doOpenUrl () {
    console.log ("UrlNal open " + urlText.text)
    urlNavRect.close ()
    urlNavRect.openUrl (urlText.text)
  }
  function doCopyUrl () {
    console.log ("UrlNav copy " + urlText.text)
    urlNavRect.close ()
    urlNavRect.copyUrl (urlText.text)
  }
  function doOpenUrlExternal () {
    console.log ("UrlNav external " + urlText.text)
    urlNavRect.close ()
    urlNavRect.openUrlExternal (urlText.text)
  }
  function doMailUrl () {
    urlNavRect.close ()
    urlNavRect.mailUrl (urlText.text)
  }
  Rectangle {
    id: textRect
    anchors { top: urlNavRect.top; horizontalCenter: urlNavRect.horizontalCenter }
    height: 48
    width: parent.width
    color: "transparent"
    TextInput {
      anchors { centerIn: parent}
      autoScroll: true
      selectByMouse: true
      id: urlText
      Keys.onReturnPressed: {
        urlNavRect.doOpenUrl ()
      }
      Keys.onEnterPressed: {
        urlNavRect.doOpenUrl ()
      }
      Keys.onEscapePressed: {
        urlNavRect.close ()
      }
    }
  }
  Flow {
    id: controlButtons
    spacing: 4
    anchors { top: textRect.bottom; horizontalCenter: urlNavRect.horizontalCenter }
    Gradient {
      id: buttonShade
      GradientStop { position: 0.00; color: "#c6e6e6" }
      GradientStop { position: 1.00; color: "#f0f0f0" }
    }
    ChoiceButton {
      id: loadButton
      height: urlNavRect.buttonHeight
      labelText: qsTr ("Load")
      radius: 0.5*height
      gradient: buttonShade
      onClicked: urlNavRect.doOpenUrl ()
    }
    ChoiceButton {
      id: copyButton
      height: urlNavRect.buttonHeight
      labelText: qsTr ("Copy")
      radius: 0.5*height
      gradient: buttonShade
      onClicked: urlNavRect.doCopyUrl ()
    }
    ChoiceButton {
      id: browseButton
      height: urlNavRect.buttonHeight
      labelText: qsTr ("To Browser")
      radius: 0.5*height
      gradient: buttonShade
      onClicked: urlNavRect.doOpenUrlExternal ()
    }
    ChoiceButton {
      id: mailButton
      height: urlNavRect.buttonHeight
      labelText: qsTr ("EMail")
      radius: 0.5*height
      gradient: buttonShade
      onClicked: urlNavRect.doMailUrl ()
    }
  }
}
