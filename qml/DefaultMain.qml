

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
import QtWebKit 1.0
import moui.geuzen.utils.static 1.0

Rectangle {
  id: displayArea
  objectName: "DisplayArea"
  color: "transparent"
  x: isPortrait ? (isInverted ? (width - height) * 0.5 : (height - width) * 0.5) : 0
  
  property real embedMargin : 2
  property real indexItemHeight: 32
  property real normalButtonHeight: isProbablyPhone ? 48 : 32
  property real indexHeight : 5 * indexItemHeight + embedMargin
  property real mainWidth: isPortrait ? height : width
  property real mainHeight: isPortrait ? width : height
  property real restHeight: mainHeight - indexHeight - controlPanel.height
  property real fullStoryHeight:  mainHeight - controlPanel.height
  property real shrinkDelay: 250
  property bool isPortrait: false
  property bool isInverted: false

  function setTheHtml (theHtml) { storyView.setTheHtml (theHtml) }
  function setTheUrl (theUrl)   { storyView.url = theUrl }

  function newestNewsRow (theRow) { streamListArea.setNewestRow (theRow) }
  function shrinkFeedIndex () {
    feedIndexArea.shrink ()
    controlPanel.showIndex = false
  }
  function expandFeedIndex () {
    feedIndexArea.expand ()
    controlPanel.showIndex = true
  }
  function shrinkFeedList () {
    feedListArea.shrink ()
    controlPanel.showFeeds = false
  }
  function expandFeedList () {
    feedListArea.expand ()
    controlPanel.showFeeds = true
  }
  function toggleLists () {
    feedIF.toggleLists ()
  }
  function wheelTurned (theX, theY, theOrientation, theDelta) {
    storyView.wheelTurned (theX, theY, theOrientation, theDelta)
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
      indexBox.minimized = true
      feedEditArea.show ()
      storyView.visible = false
    } else {
      indexBox.minimized = false
      feedEditArea.hide ()
      storyView.visible = true
    }
    controlIF.setEditingFeed (visi)
  }

  GeuzenOrientation {
    id: orientationWatcher
    onRotationChange: {
      displayArea.isPortrait = portrait
      displayArea.rotation = rotation
      displayArea.isInverted = inverted
      console.log ("new orientation port " + displayArea.isPortrait)
      console.log ("main box x " + displayArea.x + " y " + displayArea.y)
    }
    Component.onCompleted: {
      console.log ("have Orientation Watcher")
    }
  }
  ControlPanel { 
    id: controlPanel
    objectName: "ControlPanel"
    outsideWidth: mainWidth
    visible: true
    color: "transparent"
    anchors { horizontalCenter:  displayArea.horizontalCenter }
    z: feedIndexArea.z + 5
    property bool normalShowTopics: false
    property bool normalShowStream: false
    onShowTopics: { normalShowTopics = true; topicListArea.show () }
    onHideTopics: { normalShowTopics = false; topicListArea.hide () }
    onShowRecent: { normalShowStream = true; streamListArea.show () }
    onHideRecent: { normalShowStream = false; streamListArea.hide () }
    onSelectQuit: { controlIF.exitApp () }
    onSelectHelp: { controlIF.help () }
    onImportDRSS: { controlIF.importFeeds ("DRSS") }
    onImportOPML: { controlIF.importFeeds ("OPML") }
    onToggleViewSelect: {
      toggleLists() 
      indexBox.show ()
    }
    onNewFeed: { showEdit (true); feedEdit.displayNew ("") }
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

  Rectangle {
    id: indexBox
    objectName: "IndexBox"
    anchors {top: controlPanel.bottom; horizontalCenter:  displayArea.horizontalCenter }
    height:  indexHeight
    visible: true
    property bool minimized: false
    width: mainWidth
    color: "transparent"
    function hide () {
      minimized = true
    }
    function show () {
      minimized = false
    }
    transform: Scale {
      id: indexBoxScale
      yScale: 1
      Behavior on yScale { NumberAnimation { duration: shrinkDelay }}
    }
    FeedList {
      id: feedListArea
      objectName: "FeedListArea"
      visible: true
      height: indexHeight
      itemHeight: indexItemHeight
      normalWidth: indexBox.width
      anchors.top: indexBox.top
      width: normalWidth
      scale: 1
      color: "#e5ddf5"
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
      normalWidth: indexBox.width
      anchors.top: indexBox.top
      anchors.left: feedListArea.right
      width: 0
      scale: 0
      color: "#ddf5f5"
      itemColor: "#ddffff"
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
    color: "#ff9922"
    onSelected: { controlIF.changeTopic (name) }
    onQuitit: { controlPanel.normalShowTopics = false ; hide () }
  }

  StreamList {
    id: streamListArea
    objectName: "StreamListArea"
    visible: true
    itemHeight: indexItemHeight
    normalWidth: indexBox.width * 0.5
    normalHeight: indexItemHeight * 5
    leftMargin: indexBox.width * 0.5
    anchors {top: indexBox.top; left: indexBox.left }
    initialYScale: 0
    color: "transparent"
    border.color: "blue"
    radius: 6
    clip: true
    z: feedIndexArea.z + 1
    onSelectFeed: { controlIF.selectFeed (feedId) }
    onSelectStory: { controlIF.displayStory (feedId, title, hash) }
    onQuitit: { controlPanel.normalShowStream = false; hide () }
  } 

  StoryView {
    id: storyView
    objectName: "StoryView"
    signal changedIsWeb (bool isWebNow)
    visible: true
    z: indexBox-2
    anchors { 
      top: indexBox.minimized ? indexBox.top : indexBox.bottom
      topMargin: webNavRect.height
      horizontalCenter: displayArea.horizontalCenter
    }
    storyBoxHeight: mainHeight - controlPanel.height 
            - (indexBox.minimized ? 0 : indexBox.height )
            - webNavRect.height
    storyBoxWidth: mainWidth
    clip: false
   
    storyHtml: "<p>"+ qsTr("No Current Story.") + "</p>"
    onIsWebChanged: changedIsWeb (isWeb)
    onQuitit: controlIF.popHtml()
  }     

  Rectangle {
    id: webNavRect
    objectName: "WebNavBox"
    property real space: 2
    property string buttonColor: "lightgreen" 
    property real buttonOpacity: 0.66667
    property real navButtonWidth: 0.2*mainWidth
    visible: storyView.isWeb
    anchors { 
      top: indexBox.top
      horizontalCenter: storyView.horizontalCenter
    }    
    Gradient {
      id: webButtonShade
      GradientStop { position: 0.00; color: webNavRect.buttonColor }
      GradientStop { position: 1.00; color: "#f0f0f0" }
    }
    width: childrenRect.width
    color: "transparent"
    height: (visible ? normalButtonHeight : 0)
    z:feedListArea.z + 1

    ChoiceButton {
      id: allBackButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      gradient: webButtonShade
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: webNavRect.left; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" << ")
      radius: 0.5* height
      onClicked: { controlIF.popHtml () }
    }
    ChoiceButton {
      id: backButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      gradient: webButtonShade
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: allBackButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" < ")
      radius: 0.5* height
      onClicked: { storyView.back.trigger () }
    }
    ChoiceButton {
      id: copyButton
      width:parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      gradient: webButtonShade
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: backButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr("URL...")
      radius: 0.5* height
      onClicked: {
        if (urlNav.visible) {
          urlNav.close ()
        } else { 
          urlNav.open (storyView.url) 
        }
      }
    }
    ChoiceButton {
      id: forwardButton
      width: parent.navButtonWidth
      height: normalButtonHeight
      opacity: webNavRect.buttonOpacity
      gradient: webButtonShade
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: copyButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" > ")
      radius: 0.5* height
      onClicked: { storyView.forward.trigger() ()}
    }
  }

  UrlNav {
    id: urlNav
    anchors { top: webNavRect.bottom; horizontalCenter: webNavRect.horizontalCenter }
    visible: false
    width:parent.width
    border.color: "red"
    onOpenUrl: { storyView.loadUrl (url) }
    onCopyUrl: { controlIF.toCopy (url) }
    onOpenUrlExternal: { controlIF.loadUrlExternal (url) }
    onMailUrl: { controlIF.mailText (url) }
  }

  Flickable {
    id: feedEditArea
    objectName: "FeedEditArea"
    width: mainWidth
    height: fullStoryHeight
    clip: true
    property real rollDelay: 125
    flickableDirection: Flickable.HorizontalAndVerticalFlick
    interactive: true
    boundsBehavior: Flickable.DragOverBounds
    z: indexBox.z +1
    contentWidth: feedEdit.width; contentHeight: feedEdit.height
    anchors { top: indexBox.top; horizontalCenter: indexBox.horizontalCenter }
    visible: true
    function hide () { rollupScale.yScale = 0 }
    function show () { rollupScale.yScale = 1 }
    
    transform: Scale {
      id: rollupScale
      yScale: 0
      Behavior  on yScale {
        NumberAnimation { duration: feedEditArea.rollDelay }
      }
    }
    FeedEdit {
      id: feedEdit 
      objectName: "FeedEdit"
      width: mainWidth
      z: parent.z
      anchors {top: parent.top; horizontalCenter: parent.horizontalCenter }
      onStartNewFeed: {
        feedEdit.displayNew (url)
        showEdit (true)
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
    property string mainBackgroundColor: "#f2f2f2"
    objectName: "ConfigList"
    scale: 0
    isShown: false
    color: mainBackgroundColor
    z: indexBox.z + 1
    normalWidth:parent.width -4
    height: parent.height
    anchors { top: controlPanel.bottom; horizontalCenter: displayArea.horizontalCenter}
    onUpdateConfigItem: configIF.updateValue (theGroup, theKey, newValue) 
    onDoneConfig: { configList.hide (); indexBox.show () }
    onRestartConfig: controlIF.restartApp () 
    onResetConfig: controlIF.resetConfig()
  }
  Rectangle {
    id: mainLoadIndicator
    width: 52; height: 26
    radius: height * 0.25
    anchors { top: storyView.top; right: storyView.right }
    opacity: 0.5
    color: "yellow"
    Text { 
      anchors { 
        verticalCenter: parent.verticalCenter
        horizontalCenter: parent.horizontalCenter 
      }
      color: "black"
      text: qsTr ("Load") 
    }
    visible: false
  }
  Connections {
    target: storyView
    onChangedIsWeb: {
      if (isWebNow ) {
        streamListArea.hide ()
        topicListArea.hide ()
        indexBox.minimized = true
      } else {
        if (controlPanel.normalShowTopics) topicListArea.show ()
        if (controlPanel.normalShowStream) streamListArea.show ()
        indexBox.minimized = false
      }
    }
  }
  Connections {
    target: controlIF
    onLoadingChanged: {
      mainLoadIndicator.visible = loading
    }
  }
  states: [
  State {
    name: "indexHidden"
    when: indexBox.minimized && indexBox.visible
    PropertyChanges {
      target: indexBoxScale
      yScale: 0
    }
  },
  State {
    name: "indexShown"
    when: !(indexBox.minimized && indexBox.visible)
    PropertyChanges {
      target: indexBoxScale
      yScale: 1
    }
  }
  ]
  Component.onCompleted: {
    orientationWatcher.start()
    console.log ("loaded main")
    console.log ("started orientation watcher")
  }
}
