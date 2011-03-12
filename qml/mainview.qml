

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
  function displayEditFeed (feedUrl, feedTitle, siteUrl, nickText) {
    feedEdit.displayEditFeed (feedUrl, feedTitle, siteUrl, nickText)
  }

  color: "transparent"
  width: displayWidth
  height: displayHeight

  ControlPanel { 
    id: controlPanel
    visible: true
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
      visible: true
      height: indexHeight
      normalWidth: parent.width
      anchors.top: indexBox.top
      width: normalWidth
      scale: 1
      color: "yellow"
      z:5
      clip: true
      MouseArea {
        anchors.fill: parent
        onClicked: console.log ("clicked Feed List Box " + mouseX + mouseY)
      }
      onSelected: { 
        controlIF.feedClicked (idx, i,t) 
      }
      onReportOrientation: { feedIF.listOrientation (orient) }
    }
    FeedIndex {
      id: feedIndexArea
      visible: true
      height: indexHeight
      normalWidth: parent.width
      anchors.top: indexBox.top
      anchors.left: feedListArea.right
      width: 0
      scale: 0
      color: "cyan"
      clip: true
      MouseArea {
        anchors.fill: parent
        onClicked: console.log ("clicked Feed Index Box " + mouseX + mouseY)
      }
      onSelected: { 
        feedIF.storyClicked (idx, i,t) 
      }
      onReportOrientation: { feedIF.listOrientation (orient) }
    }
  }
  StoryView {
    id: storyView
    visible: true
    z: 3
    anchors.top: indexBox.bottom
    anchors.leftMargin: embedMargin
    anchors.rightMargin: embedMargin
    height: 500
   
    storyHtml: "<p>default <b>html</b>.</p>"
  }
  Flickable {
    id: feedEditArea
    width: storyView.width
    height: storyView.height
    z: storyView.z +1
    contentWidth: feedEdit.width; contentHeight: feedEdit.height
    anchors { top: storyView.top; horizontalCenter: storyView.horizontalCenter }
    visible: false
    FeedEdit {
      id: feedEdit 
      width: storyView.width - 4
      height: storyView.height - 4
      z: 4
      anchors {top: parent.top; horizontalCenter: parent.horizontalCenter }
    }
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
    onMoreSelect: {
      var visi = !feedEdit.visible
      console.log ("Visi temp " + visi)
      feedEdit.visible = visi
      feedEditArea.visible = visi
      console.log ("area w " + feedEditArea.width + " visi " + feedEditArea.visible )
      console.log (" edit visi " + feedEdit.visible + " width " + feedEdit.width)
      console.log (" area " + feedEditArea + " edit " + feedEdit)
      if (feedEdit.visible) { feedEdit.clear() }
      controlIF.setEditingFeed (feedEdit.visible)
    }
  }
  
 }