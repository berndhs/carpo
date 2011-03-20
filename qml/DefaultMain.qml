

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
  objectName: "DisplayArea"

  property real embedMargin : 2
  property real indexItemHeight: 32
  property real verticalIndexHeight: 5 * indexItemHeight + embedMargin
  property real horizontalIndexHeight: 80
  property real indexHeight : verticalIndexHeight
  property real displayWidth: 500
  property real displayHeight: 400
  property real shrinkDelay: 250

  function setTheHtml (theHtml) { storyView.storyHtml = theHtml }
  function setTheUrl (theUrl)   { storyView.url = theUrl }

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
  function displayEditFeed (feedId, feedUrl, feedTitle, siteUrl, 
                            nickText, theDescription, theTopics) {
    feedEdit.displayEditFeed (feedId, feedUrl, feedTitle, siteUrl, 
                            nickText, theDescription, theTopics)
    indexBox.height = 0
  }
  function showEdit (visi) {
    console.log ("Visi temp " + visi)
    feedEdit.visible = visi
    feedEditArea.visible = visi
    if (feedEdit.visible) { 
      feedEdit.clear() 
      storyView.visible = false
    } else {
      indexBox.height = indexHeight
      storyView.visible = true
    }
    controlIF.setEditingFeed (feedEdit.visible)
  }

  color: "transparent"
  width: displayWidth
  height: displayHeight

  ControlPanel { 
    id: controlPanel
    objectName: "ControlPanel"
    visible: true
    onShowTopics: { topicListArea.show () }
    onHideTopics: { topicListArea.hide () }
    onShowRecent: { streamListArea.show () }
    onHideRecent: { streamListArea.hide () }
  }
  Rectangle {
    id: indexBox
    objectName: "IndexBox"
    anchors.top: controlPanel.bottom
    height: indexHeight
    width: parent.width
    color: "transparent"
    function hide () {
      shrinkScale.running = true; 
    }
    function show () {
      expandScale.running = true;
    }
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
    MouseArea {
      anchors.fill: parent
      onClicked: { console.log ("clicked main index box") }
    }
    FeedList {
      id: feedListArea
      objectName: "FeedListArea"
      visible: true
      height: indexHeight
      itemHeight: indexItemHeight
      normalWidth: parent.width
      anchors.top: indexBox.top
      width: normalWidth
      scale: 1
      color: "yellow"
      clip: true
      MouseArea {
        anchors.fill: parent
        onClicked: console.log ("clicked Feed List Box " + mouseX + mouseY)
      }
      onSelected: { 
        controlIF.feedClicked (idx, i,t) 
      }
      onSelectMoveUp: { controlIF.moveUp (ident) }
      onSelectMoveDown: { controlIF.moveDown (ident) }
      onReportOrientation: { feedIF.listOrientation (orient) }
    }
    FeedIndex {
      id: feedIndexArea
      objectName: "FeedIndexArea"
      visible: true
      height: indexHeight
      itemHeight: indexItemHeight
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
      onHoldit: {
        feedIF.storyHold (idx, i, t)
      } 
    }    
  }  
  TopicList {
    id:topicListArea
    objectName: "TopicList"
    visible: true
    scale: 1
    height:indexHeight
    normalWidth: parent.width * 0.3333
    width: normalWidth
    anchors { right: indexBox.right; top: indexBox.top }
    z: feedIndexArea.z + 1
    color: "red"
    onSelected: { controlIF.changeTopic (name) }
  }
  StreamList {
    id: streamListArea
    objectName: "StreamListArea"
    visible: true
    itemHeight: indexItemHeight
    normalWidth: indexBox.width * 0.5
    normalHeight: indexBox.height
    leftMargin: indexBox.width * 0.4
    anchors.top: indexBox.top
    anchors.left: indexBox.left
    scale: 1
    color: "transparent"
    clip: true
    z: 10
    MouseArea {
      anchors.fill: parent
      onClicked: console.log ("clicked Stream Index Box " + mouseX + mouseY)
    }
    onSelectFeed: { controlIF.selectFeed (feedId) }
    onSelectStory: { controlIF.displayStory (feedId, title, hash) }
  } 
  StoryView {
    id: storyView
    objectName: "StoryView"
    visible: true
    z: indexBox-2
    anchors { 
      top: indexBox.bottom
      leftMargin: 0
      rightMargin: 0
    }
    height: parent.height - controlPanel.height - indexBox.height
   
    storyHtml: "<p>No Story.</p>"
  }
  Rectangle {
    id: webNavRect
    objectName: "WebNavBox"
    property real space: 2
    property string buttonColor: "magenta"
    property real buttonOpacity: 0.6
    anchors { 
      verticalCenter: indexBox.bottom 
      horizontalCenter: indexBox.horizontalCenter
    }
    width: backButton.width + copyButton.width + forwardButton.width + 2*space
    color: "transparent"
    height: childrenRect.height
    z:feedListArea.z + 1
    ChoiceButton {
      id: backButton
      width: 50
      height: childrenRect.height
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: webNavRect.left; verticalCenter: webNavRect.verticalCenter }
      labelText: "Back"
      onClicked: { storyView.back () }
    }
    ChoiceButton {
      id: copyButton
      width: 75
      height: childrenRect.height
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: backButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: "Copy URL"
      onClicked: { controlIF.toCopy (storyView.url) }
    }
    ChoiceButton {
      id: forwardButton
      width: 75
      height: childrenRect.height
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: copyButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: "Forward"
      onClicked: { storyView.forward ()}
    }
  }

  Flickable {
    id: feedEditArea
    objectName: "FeedEditArea"
    width: storyView.width
    height: storyView.height
    z: indexBox.z +1
    contentWidth: feedEdit.width; contentHeight: feedEdit.height
    anchors { top: storyView.top; horizontalCenter: storyView.horizontalCenter }
    visible: false
    FeedEdit {
      id: feedEdit 
      objectName: "FeedEdit"
      width: storyView.width - 4
      height: storyView.height - 4
      z: 4
      anchors {top: parent.top; horizontalCenter: parent.horizontalCenter }
      onStartNewFeed: {
        feedEdit.displayNew (url)
        indexBox.height = 0
      }
      onSaveFeed: {
        controlIF.saveFeed (ident, feedUrl, title, siteUrl, nick, descr, topics)
        showEdit (false)
      }
      onDeleteFeed: {
        controlIF.removeFeed (ident)
        showEdit (false)
      }
      onProbeFeed: {
        controlIF.probeFeed (url)
      }
    }
  }
  ConfigList {
    id: configList
    property string mainBackgroundColor: "blue"
    objectName: "ConfigList"
    scale: 0
    isShown: false
    color: mainBackgroundColor
    z: indexBox.z + 1
    width:parent.width
    height: parent.height
    anchors { top: controlPanel.bottom; }
    onUpdateConfigItem: { configIF.updateValue (theGroup, theKey, newValue) }
  }
  Connections {
    target: controlPanel
    objectName: "ControlPanelConnections"
    onToggleViewSelect: {
      console.log ("Connections clicked " )
      console.log (" index width " + feedIndexArea.width)
      toggleLists() 
    }
    onMoreSelect: {
      showEdit (!feedEdit.visible)
    }
    onMaintainSelect: {
      var visi = configList.isShown
      console.log ("Maintain Selected " + visi)
      if (visi) {
        configList.hide ();
        indexBox.show ();
      } else {
        configIF.loadView ();
        configList.show ();
        indexBox.hide ();
      }
    }
  }
}
