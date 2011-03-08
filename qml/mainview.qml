

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
  property real indexHeight : 200

  function setTheHtml (theHtml) {
    storyView.html = theHtml
    specialLabel.refreshCount() 
    specialLabel.color = "red"
  }
  function setSize (w, h) {
    width = w
    height = h
  }
  function turnIndex () {
    feedIF.report ("turnIndex called")
    feedIndexArea.flipOrientation()
    feedIF.report ("turnIndex finishing")
  }
  function shrinkFeedIndex () {
    feedIndexArea.shrink ()
  }
  function expandFeedIndex () {
    feedIndexArea.expand ()
  }
  function shrinkFeedList () {
    feedListArea.shrink ()
  }
  function expandFeedList () {
    feedListArea.expand ()
  }
  function toggleLists () {
    feedIF.toggleLists ()
  }

  width: 400; height: 400
  color: "transparent"
  border.color: "black"

  FeedIndex {
    id: feedIndexArea
    height: indexHeight
    normalWidth: 0.5*parent.width
    width: 0.5*parent.width
    border.color: "blue"
    border.width: 3
    onSelected: { 
      feedIF.clickedOn (idx, i,t) ; 
      feedIF.report (" list current " + feedIndexArea.reportCurrent() )
    }
    onReportOrientation: { feedIF.listOrientation (orient) }
  }
  FeedList {
    id: feedListArea
    anchors.left: feedIndexArea.right
    height: indexHeight
    normalWidth: 0.5*parent.width
    width: 0.5*parent.width
    border.color: "red"
    border.width: 3
    onSelected: { 
      feedIF.clickedOn (idx, i,t) ; 
      feedIF.report (" list current " + feedIndexArea.reportCurrent() )
    }
    onReportOrientation: { feedIF.listOrientation (orient) }
  }
  Rectangle {
    id: specialLabel
    height: 20
    width: parent.width - 2*embedMargin
    color: "cyan"
    border.color: "black"
    anchors.top: feedIndexArea.bottom
    anchors.leftMargin: embedMargin
    anchors.rightMargin: embedMargin
    Text {
      id: letters
      text: "Toggle Feed-List / Feed-Index view --- Rows: " + displayModel.rowCountText()
    }
    function refreshCount () {
      letters.text = "Rows now " + displayModel.rowCountText()
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { toggleLists () }
    }
  }
  WebView {
    id: storyView
    anchors.top: specialLabel.bottom
    anchors.leftMargin: embedMargin
    anchors.rightMargin: embedMargin
   
    preferredHeight: 500
    preferredWidth:  parent.width
    settings.autoLoadImages: true
    html: "<p>default <b>html</b>.</p>"
  }
  
 }