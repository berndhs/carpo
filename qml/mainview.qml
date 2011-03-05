

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

  function setTheHtml (theHtml) {
    storyView.html = theHtml
    specialLabel.refreshCount() 
    specialLabel.color = "red"
    feedIndexArea.nextStory ()
  }

  width: 400; height: 400
  color: "transparent"
  border.color: "black"

  FeedIndex {
    id: feedIndexArea
    onSelected: {
      feedIF.clickedOn (d,m)
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
  
 }