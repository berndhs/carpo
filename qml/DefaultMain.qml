

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
  property real indexHeight : 5 * indexItemHeight + embedMargin
  property real restHeight: height - indexHeight - controlPanel.height
  property real displayWidth: 800
  property real displayHeight: 600
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
      feedEditArea.show ()
      storyView.visible = false
    } else {
      feedEditArea.hide ()
      storyView.visible = true
    }
    controlIF.setEditingFeed (visi)
  }

  color: "transparent"
  width: displayWidth
  height: displayHeight

  ControlPanel { 
    id: controlPanel
    objectName: "ControlPanel"
    visible: true
    z: feedIndexArea.z + 5
    property bool normalShowTopics: false
    property bool normalShowStream: false
    onShowTopics: { normalShowTopics = true; topicListArea.show () }
    onHideTopics: {  normalShowTopics = false; topicListArea.hide () }
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
    anchors.top: controlPanel.bottom
    height:  indexHeight
    visible: true
    property bool minimized: fals0
    width: displayArea.width
    color: "transparent"
    function hide () {
      indexBoxScale.yScale = 0
      minimized = true
      webNavRect.moveTop (true)
    }
    function show () {
      indexBoxScale.yScale = 1
      minimized = false
      webNavRect.moveTop (false)
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
      top: indexBox.bottom //(webNavRect.visible ? webNavRect.bottom : indexBox.bottom)
      topMargin: webNavRect.height
      leftMargin: 0
      rightMargin: 0
    }
    height: parent.height - controlPanel.height 
            - indexBox.height - webNavRect.height
    clip: false
   
    storyHtml: "<p>"+ qsTr("No Current Story.") + "</p>"
    onIsWebChanged: changedIsWeb (isWeb)
    onQuitit: controlIF.popHtml()
  }     

  Rectangle {
    id: webNavRect
    objectName: "WebNavBox"
    property real space: 2
    property string buttonColor: "magenta"
    property real buttonOpacity: 0.6
    property real navButtonWidth: 0.2*displayArea.width
    visible: storyView.isWeb
    function moveTop (atTop) {
      if (atTop)  anchors.top = indexBox.top
      else        anchors.top = indexBox.bottom
    } 
    anchors { 
      top: indexBox.bottom
      horizontalCenter: storyView.horizontalCenter
    }
    width: childrenRect.width
    color: "transparent"
    height: (visible ? childrenRect.height : 0)
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
      labelText: qsTr("URL...")
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
      color: parent.buttonColor
      commonMargin: parent.space
      z:parent.z + 1
      anchors {left: copyButton.right; verticalCenter: webNavRect.verticalCenter }
      labelText: qsTr(" > ")
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
    width: displayArea.width
    height: restHeight
    clip: true
    property real rollDelay: 125
    flickableDirection: Flickable.HorizontalAndVerticalFlick
    interactive: true
    boundsBehavior: Flickable.DragOverBounds
    z: indexBox.z +1
    contentWidth: feedEdit.width; contentHeight: feedEdit.height
    anchors { top: storyView.top; horizontalCenter: storyView.horizontalCenter }
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
      width: displayArea.width
      z: parent.z
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
    property string mainBackgroundColor: "#f2f2f2"
    objectName: "ConfigList"
    scale: 0
    isShown: false
    color: mainBackgroundColor
    z: indexBox.z + 1
    normalWidth:parent.width -4
    height: parent.height
    anchors { top: controlPanel.bottom; leftMargin: 4}
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
        indexBox.hide ()
      } else {
        if (controlPanel.normalShowTopics) topicListArea.show ()
        if (controlPanel.normalShowStream) streamListArea.show ()
        indexBox.show ()
      }
    }
  }
  Connections {
    target: controlIF
    onLoadingChanged: {
      mainLoadIndicator.visible = loading
    }
  }
}
