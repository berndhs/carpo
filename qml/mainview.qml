

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
 import QtWebKit 1.0


 Rectangle {
  id: displayArea

  property string urlString : "http://fiji.reflective-computing.com"
  property real embedMargin : 2

  width: 400; height: 400
  color: "transparent"
  border.color: "black"


  Component {
    id: contactDelegate
    Item {
      width: 300; height: 60
      Column {
        Text { text: 'The meal for <b>' + day + ' </b> is <i> ' + meal + '</i>' }
      }
    }
  }
  Rectangle {
    id: feedIndexArea
    z: -1
    color: "transparent"
    border.color: "red"
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.topMargin: embedMargin
    anchors.leftMargin: embedMargin
    height: parent.height - 2*embedMargin
    width: parent.width - 2*embedMargin 
    ListView {
      id: storyList
      z: 2
      width: parent.width - 2*embedMargin
      height: parent.width - 2*embedMargin
      clip: true
      contentWidth: childrenRect.width; contentHeight: childrenRect.height
      anchors.top:  feedIndexArea.top
      anchors.topMargin: embedMargin
      orientation: ListView.Vertical
      model: displayModel
      highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
      delegate: contactDelegate
    }
  }

  Rectangle {
    id: specialLabel
    height: 32
    width: 80
    color: "green"
    border.color: "black"
    anchors.top: feedIndexArea.bottom
    Text {
      id: letters
      text: "Rows: " + displayModel.rowCountText()
    }
    function refreshCount () {
      letters.text = "Rows now " + displayModel.rowCountText()
    }
  }
  WebView {
    id: storyView
    anchors.top: specialLabel.bottom
    anchors.topMargin: 8
   
    preferredHeight: 500
    preferredWidth:  300
    settings.autoLoadImages: true
    html: "<p>This is old <b>html</b>.</p>"
  }
  function setTheHtml (theHtml) {
    storyView.html = theHtml
    specialLabel.refreshCount() 
    specialLabel.color = "red"
  }
  
 }