

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
  property real normalButtonHeight: 32
  property real verticalIndexHeight: 5 * indexItemHeight + embedMargin
  property real horizontalIndexHeight: 80
  property real indexHeight : verticalIndexHeight
  property real displayWidth: 500
  property real displayHeight: 400
  property real shrinkDelay: 250

  function setTheHtml (theHtml) { storyView.setTheHtml (theHtml) }
  function setTheUrl (theUrl)   { storyView.url = theUrl }

  function newestNewsRow (theRow) { streamListArea.setNewestRow (theRow) }

  function setSize (w, h) {
    console.log (" setSize " + w + " " + h)
    displayWidth = w
    displayHeight = h
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
  function displayEditFeed (feedId, feedUrl, feedTitle, siteUrl, 
                            nickText, theDescription, theTopics) {
    feedEdit.displayEditFeed (feedId, feedUrl, feedTitle, siteUrl, 
                            nickText, theDescription, theTopics)
    indexBox.height = 0
  }
  function showEdit (visi) {
    console.log ("Visi temp " + visi)
    if (visi) {
      feedEditArea.show ()
      storyView.visible = false
    } else {
      feedEditArea.hide ()
      indexBox.height = indexHeight
      storyView.visible = true
    }
    controlIF.setEditingFeed (visi)
  }

  color: "transparent"
  width: displayWidth
  height: displayHeight

  Rectangle {
    id: indexBox
    objectName: "IndexBox"
    anchors.top: controlPanel.bottom
    height: (storyView.isWeb ? 0 : indexHeight)
    visible: !storyView.isWeb
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
      onSelected: { 
        controlIF.feedClicked (idx, i,t) 
      }
      onSelectMoveUp: { controlIF.moveUp (ident) }
      onSelectMoveDown: { controlIF.moveDown (ident) }
      onSelectedLong: { showEdit (true); controlIF.editFeed (i) }
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
      itemColor: "cyan"
      clip: true
      onSelected: { 
        feedIF.storyClicked (idx, i,t) 
      }
      onHoldit: {
        feedIF.storyHold (idx, i, t)
      } 
      onQuitit: { toggleLists () }
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
    z: streamListArea.z + 1
    color: "red"
    onSelected: { controlIF.changeTopic (name) }
    onQuitit: { hide () }
  }
  StreamList {
    id: streamListArea
    objectName: "StreamListArea"
    visible: true
    itemHeight: indexItemHeight
    normalWidth: indexBox.width * 0.5
    normalHeight: indexItemHeight * 6
    leftMargin: indexBox.width * 0.4
    anchors.top: indexBox.top
    anchors.left: indexBox.left
    initialYScale: 0
    color: "transparent"
    border.color: "blue"
    radius: 6
    clip: true
    z: feedIndexArea.z + 1
    onSelectFeed: { controlIF.selectFeed (feedId) }
    onSelectStory: { controlIF.displayStory (feedId, title, hash) }
    onQuitit: { hide () }
  } 
  StoryView {
    id: storyView
    objectName: "StoryView"
    signal changedIsWeb (bool isWebNow)
    visible: true
    z: indexBox-2
    anchors { 
      top: (webNavRect.visible ? webNavRect.bottom : indexBox.bottom)
      leftMargin: 0
      rightMargin: 0
    }
    height: parent.height - controlPanel.height - indexBox.height
   
    storyHtml: "<p>"+ qsTr("No Current Story.") + "</p>"
    onIsWebChanged: changedIsWeb (isWeb)
  }
  Rectangle {
    id: webNavRect
    objectName: "WebNavBox"
    property real space: 2
    property string buttonColor: "magenta"
    property real buttonOpacity: 0.6
    property real navButtonWidth: 0.2*storyView.width
    visible: storyView.isWeb
    anchors { 
      top: indexBox.bottom 
      horizontalCenter: storyView.horizontalCenter
    }
    width: childrenRect.width
    color: "transparent"
    height: childrenRect.height
    z:feedListArea.z + 1
    ChoiceButton {
      id: allBackButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: webNavRect.left; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" << ")
      onClicked: { controlIF.popHtml () }
    }
    ChoiceButton {
      id: backButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: allBackButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" < ")
      onClicked: { storyView.back.trigger () }
    }
    ChoiceButton {
      id: copyButton
      width:parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: backButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr("Copy URL")
      onClicked: { controlIF.toCopy (storyView.url) }
    }
    ChoiceButton {
      id: forwardButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: copyButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" > ")
      onClicked: { storyView.forward.trigger() ()}
    }
  }

  Flickable {
    id: feedEditArea
    objectName: "FeedEditArea"
    width: storyView.width
    height: storyView.height
    property real rollDelay: 125
    z: indexBox.z +1
    contentWidth: feedEdit.width; contentHeight: feedEdit.height
    anchors { top: storyView.top; horizontalCenter: storyView.horizontalCenter }
    visible: true
    function hide () { rollupScale.yScale = 0 }
    function show () { rollupScale.yScale = 1 }
    
    transform: Scale {
      id: rollupScale
      xScale: 1
      yScale: 0
      Behavior  on yScale {
        NumberAnimation { duration: rollDelay }
      }
    }
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
      onLoadEditFeed: {
        controlIF.loadEditFeed (addr)
      }
      onCancelEdit: { showEdit (false) }
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
    normalWidth:parent.width -4
    height: parent.height
    anchors { top: controlPanel.bottom; leftMargin: 4}
    onUpdateConfigItem: { configIF.updateValue (theGroup, theKey, newValue) }
    onDoneConfig: { configList.hide (); indexBox.show () }
    onRestartConfig: { controlIF.restartApp () }
  }
  ControlPanel { 
    id: controlPanel
    objectName: "ControlPanel"
    visible: true
    z: feedIndexArea.z + 5
    property bool normalShowTopics: false
    property bool normalShowStream: false
    onShowTopics: { normalShowTopics = true; topicListArea.show () }
    onHideTopics: { normalShowTopics = false; topicListArea.hide () }
    onShowRecent: { normalShowStream = true; streamListArea.show () }
    onHideRecent: { normalShowStream = false; streamListArea.hide () }
    onSelectQuit: { controlIF.exitApp () }
    onSelectHelp: { controlIF.help () }
    onToggleViewSelect: {
      console.log ("Connections clicked " )
      console.log (" index width " + feedIndexArea.width)
      toggleLists() 
    }
    onNewFeed: { showEdit (true) }
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
  Connections {
    target: storyView
    onChangedIsWeb: {
      if (isWebNow ) {
        streamListArea.hide ()
        topicListArea.hide ()
      } else {
        if (controlPanel.normalShowTopics) topicListArea.show ()
        if (controlPanel.normalShowStream) streamListArea.show ()
      }
    }
  }
}
