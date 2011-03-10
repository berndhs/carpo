

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

  property real embedMargin : 2
  property real verticalIndexHeight: 240
  property real horizontalIndexHeight: 80
  property real indexHeight : verticalIndexHeight
  property real displayWidth: 500
  property real displayHeight: 400

  function setTheHtml (theHtml) {
    storyView.storyHtml = theHtml
  }
  function setSize (w, h) {
    console.log (" setSize " + w + " " + h)
    displayWidth = w
    displayHeight = h
  }
  function turnIndex () {
    feedIndexArea.flipOrientation()
    feedListArea.flipOrientation()
    if (feedIndexArea.currentOrientation() == ListView.Horizontal) {
      indexHeight = horizontalIndexHeight 
    } else {
      indexHeight = verticalIndexHeight
    }
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
    console.log ("toggleLists")
    console.log ("Widths: Index " + feedIndexArea.width + " list " + feedListArea.width)
    feedIF.toggleLists ()
  }

  color: "transparent"
  border.color: "black"
  width: displayWidth
  height: displayHeight

  ControlPanel { 
    id: controlPanel
  }
  Rectangle {
    id: indexBox
    anchors.top: controlPanel.bottom
    height: indexHeight
    width: parent.width
    color: "transparent"
    MouseArea {
      anchors.fill: parent
      onClicked: { console.log ("clicked main index box") }
    }
    FeedList {
      id: feedListArea
      height: indexHeight
      normalWidth: parent.width
      anchors.top: indexBox.top
      width: normalWidth
      scale: 1
      color: "yellow"
      z:5
      border.color: "transparent"
      border.width: 3
      clip: true
      MouseArea {
        anchors.fill: parent
        onClicked: console.log ("clicked Feed List Box " + mouseX + mouseY)
      }
      onSelected: { 
        feedIF.feedClicked (idx, i,t) 
        feedIF.report (" list current " + feedIndexArea.reportCurrent() )
      }
      onReportOrientation: { feedIF.listOrientation (orient) }
    }
    FeedIndex {
      id: feedIndexArea
      height: indexHeight
      normalWidth: parent.width
      anchors.top: indexBox.top
      anchors.left: feedListArea.right
      width: 0
      scale: 0
      color: "cyan"
      border.color: "transparent"
      border.width: 3
      clip: true
      MouseArea {
        anchors.fill: parent
        onClicked: console.log ("clicked Feed Index Box " + mouseX + mouseY)
      }
      onSelected: { 
        feedIF.storyClicked (idx, i,t) 
        feedIF.report (" list current " + feedIndexArea.reportCurrent() )
      }
      onReportOrientation: { feedIF.listOrientation (orient) }
    }
  }
  StoryView {
    id: storyView
    z: 3
    anchors.top: indexBox.bottom
    anchors.leftMargin: embedMargin
    anchors.rightMargin: embedMargin
    height: 500
   
    storyHtml: "<p>default <b>html</b>.</p>"
  }
  Connections {
    target: controlPanel
    onToggleViewSelect: {
      console.log ("Connections clicked " + whereX + " " + whereY)
      console.log (" index width " + feedIndexArea.width)
      toggleLists() 
    }
    onFlipSelect: {
      console.log ("Flip selected " + whereX + " " + whereY)
      turnIndex ()
    }
  }
  
 }